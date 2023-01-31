#include "preprocess/preprocessor.h"

#include "env.h"
#include "solving_context.h"
#include "util/logger.h"

namespace bzla::preprocess {

#ifndef NDEBUG
namespace {
void
count_nodes(const Node& node, std::unordered_set<Node>& cache)
{
  node::node_ref_vector visit{node};
  do
  {
    const Node& cur = visit.back();
    visit.pop_back();
    auto [it, inserted] = cache.insert(cur);
    if (inserted)
    {
      visit.insert(visit.end(), cur.begin(), cur.end());
    }
  } while (!visit.empty());
}
}  // namespace
#endif

Preprocessor::Preprocessor(SolvingContext& context)
    : d_env(context.env()),
      d_logger(d_env.logger()),
      d_assertions(context.assertions()),
      d_global_backtrack_mgr(*context.backtrack_mgr()),
      d_pop_callback(context.backtrack_mgr(), &d_backtrack_mgr),
      d_pass_rewrite(d_env, &d_backtrack_mgr),
      d_pass_contr_ands(d_env, &d_backtrack_mgr),
      d_pass_elim_lambda(d_env, &d_backtrack_mgr),
      d_pass_elim_uninterpreted(d_env, &d_backtrack_mgr),
      d_pass_embedded_constraints(d_env, &d_backtrack_mgr),
      d_pass_variable_substitution(d_env, &d_backtrack_mgr),
      d_pass_flatten_and(d_env, &d_backtrack_mgr),
      d_pass_skeleton_preproc(d_env, &d_backtrack_mgr),
      d_pass_normalize(d_env, &d_backtrack_mgr),
      d_stats(d_env.statistics())
{
}

Result
Preprocessor::preprocess()
{
  util::Timer timer(d_stats.time_preprocess);

  // No assertions to process, return.
  if (d_assertions.empty())
  {
    return Result::UNKNOWN;
  }

  // Process assertions by level
  while (!d_assertions.empty())
  {
    size_t level = d_assertions.level(d_assertions.begin());

    // Sync backtrack manager to level. This is required if there are levels
    // that do not contain any assertions.
    sync_scope(level);

    // Create vector for current level
    AssertionVector assertions(d_assertions);
    assert(assertions.d_level == level);

    // Apply preprocessing passes until fixed-point
    apply(assertions);

    // Advance assertions to next level
    d_assertions.set_index(d_assertions.begin() + assertions.size());
  }
  assert(d_assertions.empty());

  // Sync backtrack manager to level. This is required if there are levels
  // that do not contain any assertions.
  sync_scope(d_global_backtrack_mgr.num_levels());

  return Result::UNKNOWN;
}

Node
Preprocessor::process(const Node& term)
{
  util::Timer timer(d_stats.time_process);
  // TODO: add more passes
  Node processed = d_pass_elim_lambda.process(term);
  processed      = d_pass_variable_substitution.process(processed);
  return d_pass_rewrite.process(processed);
}

/* --- Preprocessor private ------------------------------------------------- */

void
Preprocessor::apply(AssertionVector& assertions)
{
  Msg(1) << "Preprocessing " << assertions.size() << " assertions";
  if (assertions.size() == 0)
  {
    return;
  }

#ifndef NDEBUG
  std::unordered_set<Node> cache_pre;
  if (d_env.options().dbg_pp_node_inc())
  {
    for (size_t i = 0; i < assertions.size(); ++i)
    {
      count_nodes(assertions[i], cache_pre);
    }
  }
#endif

  auto& options  = d_env.options();
  bool skel_done = false;
  // fixed-point passes
  do
  {
    // Reset changed flag.
    assertions.reset_modified();
    ++d_stats.num_iterations;

    size_t cnt;
    cnt = assertions.num_modified();
    d_pass_rewrite.apply(assertions);
    Msg(2) << assertions.num_modified() - cnt << " after rewriting";

    if (options.pp_flatten_and())
    {
      cnt = assertions.num_modified();
      d_pass_flatten_and.apply(assertions);
      Msg(2) << assertions.num_modified() - cnt << " after and flattening";
    }

    if (options.pp_variable_subst())
    {
      cnt = assertions.num_modified();
      d_pass_variable_substitution.apply(assertions);
      Msg(2) << assertions.num_modified() - cnt
             << " after variable substitution";
    }

    if (options.pp_skeleton_preproc() && !skel_done)
    {
      cnt = assertions.num_modified();
      d_pass_skeleton_preproc.apply(assertions);
      skel_done = true;
      Msg(2) << assertions.num_modified() - cnt
             << " after skeleton simplification";
    }

    if (options.pp_embedded_constr())
    {
      cnt = assertions.num_modified();
      d_pass_embedded_constraints.apply(assertions);
      Msg(2) << assertions.num_modified() - cnt
             << " after embedded constraints";
    }

    if (options.pp_contr_ands())
    {
      cnt = assertions.num_modified();
      d_pass_contr_ands.apply(assertions);
      Msg(2) << assertions.num_modified() - cnt << " after contradicting ands";
    }

    cnt = assertions.num_modified();
    d_pass_elim_lambda.apply(assertions);
    Msg(2) << assertions.num_modified() - cnt << " after lambda elimination";

    cnt = assertions.num_modified();
    d_pass_elim_uninterpreted.apply(assertions);
    Msg(2) << assertions.num_modified() - cnt
           << " after uninterpreted const/var elimination";

    if (options.pp_normalize())
    {
      cnt = assertions.num_modified();
      d_pass_normalize.apply(assertions);
      Msg(2) << assertions.num_modified() - cnt << " after normalization";
    }

  } while (assertions.modified());

#ifndef NDEBUG
  if (d_env.options().dbg_pp_node_inc())
  {
    double thresh = 1 + d_env.options().dbg_pp_node_inc() / 100.0;
    std::unordered_set<Node> cache_post;
    for (size_t i = 0; i < assertions.size(); ++i)
    {
      count_nodes(assertions[i], cache_post);
    }
    double ratio = cache_post.size() / static_cast<double>(cache_pre.size());
    Warn(ratio >= thresh) << "Preprocessed assertions contain "
                          << std::setprecision(3) << (ratio - 1) * 100
                          << "% more nodes than original assertions ("
                          << cache_pre.size() << " vs. " << cache_post.size()
                          << ")";
  }
#endif
}

void
Preprocessor::sync_scope(size_t level)
{
  while (d_backtrack_mgr.num_levels() < level)
  {
    d_backtrack_mgr.push();
  }
}

Preprocessor::Statistics::Statistics(util::Statistics& stats)
    : time_preprocess(
        stats.new_stat<util::TimerStatistic>("preprocessor::time_preprocess")),
      time_process(
          stats.new_stat<util::TimerStatistic>("preprocessor::time_process")),
      num_iterations(stats.new_stat<uint64_t>("preprocessor::num_iterations"))
{
}

}  // namespace bzla::preprocess

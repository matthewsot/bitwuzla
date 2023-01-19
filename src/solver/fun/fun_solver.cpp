#include "solver/fun/fun_solver.h"

#include "node/node_manager.h"
#include "node/node_utils.h"

namespace bzla::fun {

using namespace node;

/* --- FunSolver public ----------------------------------------------------- */

bool
FunSolver::is_theory_leaf(const Node& term)
{
  Kind k = term.kind();
  return k == Kind::APPLY || (k == Kind::EQUAL && (term[0].type().is_fun()));
}

FunSolver::FunSolver(Env& env, SolverState& state)
    : Solver(env, state), d_applies(state.backtrack_mgr())
{
}

FunSolver::~FunSolver() {}

bool
FunSolver::check()
{
  d_fun_models.clear();
  // Do not cache size here since d_applies may grow while iterating.
  for (size_t i = 0; i < d_applies.size(); ++i)
  {
    const Node& apply = d_applies[i];
    const Node& fun = apply[0];
    auto& fun_model = d_fun_models[fun];

    Apply app(apply, d_solver_state);
    auto [it, inserted] = fun_model.insert(app);
    if (!inserted)
    {
      // Function congruence conflict
      if (it->value() != app.value())
      {
        add_function_congruence_lemma(apply, it->get());
      }
    }
  }
  return true;
}

Node
FunSolver::value(const Node& term)
{
  assert(term.type().is_fun());
  auto it = d_fun_models.find(term);
  if (it != d_fun_models.end())
  {
    NodeManager& nm                = NodeManager::get();
    const std::vector<Type>& types = term.type().fun_types();
    std::vector<Node> vars;
    for (size_t i = 0, size = types.size() - 1; i < size; ++i)
    {
      vars.push_back(nm.mk_var(types[i]));
    }

    Node res              = utils::mk_default_value(types.back());
    const auto& fun_model = it->second;

    // Construct nested ITEs for function model
    for (const auto& apply : fun_model)
    {
      const std::vector<Node>& values = apply.values();
      assert(vars.size() == values.size());
      std::vector<Node> eqs;
      for (size_t i = 0, size = vars.size(); i < size; ++i)
      {
        eqs.push_back(nm.mk_node(Kind::EQUAL, {vars[i], values[i]}));
      }
      Node cond = utils::mk_nary(Kind::AND, eqs);
      res       = nm.mk_node(Kind::ITE, {cond, apply.value(), res});
    }
    vars.push_back(res);
    return utils::mk_binder(Kind::LAMBDA, vars);
  }
  return utils::mk_default_value(term.type());
}

void
FunSolver::register_term(const Node& term)
{
  // For now we only expect function applications
  assert(term.kind() == Kind::APPLY);
  d_applies.push_back(term);
}

/* --- FunSolver private ---------------------------------------------------- */

void
FunSolver::add_function_congruence_lemma(const Node& a, const Node& b)
{
  assert(a.num_children() == b.num_children());
  assert(a.kind() == Kind::APPLY);
  assert(b.kind() == Kind::APPLY);
  assert(a != b);

  NodeManager& nm = NodeManager::get();
  std::vector<Node> premise;
  for (size_t i = 1, size = a.num_children(); i < size; ++i)
  {
    premise.emplace_back(nm.mk_node(Kind::EQUAL, {a[i], b[i]}));
  }
  Node conclusion = nm.mk_node(Kind::EQUAL, {a, b});
  Node lemma      = nm.mk_node(Kind::IMPLIES,
                          {utils::mk_nary(Kind::AND, premise), conclusion});
  d_solver_state.lemma(lemma);
}

/* --- Apply public --------------------------------------------------------- */

FunSolver::Apply::Apply(const Node& apply, SolverState& state)
    : d_apply(apply), d_hash(0)
{
  // Compute hash value of function applications based on the current function
  // argument model values.
  for (size_t i = 1, size = apply.num_children(); i < size; ++i)
  {
    d_values.emplace_back(state.value(apply[i]));
    d_hash += std::hash<Node>{}(d_values.back());
  }
  // Cache value of function application
  d_value = state.value(apply);
}

const Node&
FunSolver::Apply::get() const
{
  return d_apply;
}

const Node&
FunSolver::Apply::value() const
{
  return d_value;
}

const std::vector<Node>&
FunSolver::Apply::values() const
{
  return d_values;
}

bool
FunSolver::Apply::operator==(const Apply& other) const
{
  assert(d_values.size() == other.d_values.size());
  for (size_t i = 0, size = d_values.size(); i < size; ++i)
  {
    if (d_values[i] != other.d_values[i])
    {
      return false;
    }
  }
  return true;
}

size_t
FunSolver::Apply::hash() const
{
  return d_hash;
}

}  // namespace bzla::fun

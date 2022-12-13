#ifndef BZLA_SOLVING_CONTEXT_H_INCLUDED
#define BZLA_SOLVING_CONTEXT_H_INCLUDED

#include <unordered_set>
#include <vector>

#include "backtrack/assertion_stack.h"
#include "backtrack/backtrackable.h"
#include "env.h"
#include "node/node.h"
#include "preprocess/preprocessor.h"
#include "solver/result.h"
#include "solver/solver_engine.h"

namespace bzla {

class SolvingContext
{
 public:
  SolvingContext(const option::Options& options);

  /** Solve the current set of assertions in the context. */
  Result solve();

  /** Preprocess current set of assertions. */
  Result preprocess();

  /** Assert formula to context. */
  void assert_formula(const Node& formula);

  /**
   * Get the value of `term`.
   *
   * @note: Only valid if last solve() call returned Result::SAT.
   *
   * @param term The term to compute the value for.
   * @return The value of `term` in the current model.
   */
  Node get_value(const Node& term);

  void push();
  void pop();

  // std::vector<Node>& get_unsat_core();
  // bool is_in_unsat_core(const Node& term) const;

  /** @return Context options object. */
  const option::Options& options() const;

  backtrack::AssertionView& assertions();

  /** @return The solving context backtrack manager. */
  backtrack::BacktrackManager* backtrack_mgr();

  /** @return The solving context preprocessor. */
  preprocess::Preprocessor& preprocessor() { return d_preprocessor; };

  /** @return The solving context rewriter. */
  Rewriter& rewriter();

  /** @return The solving context environment. */
  Env& env();

 private:
  /** Solving context environment. */
  Env d_env;

  /** Manages push()/pop() requests. */
  backtrack::BacktrackManager d_backtrack_mgr;

  /** Assertion stack of this solving context. */
  backtrack::AssertionStack d_assertions;

  /** The solving context preprocessor. */
  preprocess::Preprocessor d_preprocessor;

  /** Solver engine that manages all solvers. */
  SolverEngine d_solver_engine;

  /** Result of last solve() call. */
  Result d_sat_state = Result::UNKNOWN;
};

}  // namespace bzla

#endif

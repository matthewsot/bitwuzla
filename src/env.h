#ifndef BZLA_ENV_H_INCLUDED
#define BZLA_ENV_H_INCLUDED

#include "option/option.h"
#include "rewrite/rewriter.h"
#include "util/logger.h"
#include "util/statistics.h"

namespace bzla {

class Terminator;

class Env
{
 public:
  /**
   * Constructor.
   * @param options The associated configuration options.
   */
  Env(const option::Options& options = option::Options());

  /** @return The associated options instance. */
  const option::Options& options() const;

  /** @return The associated statistics instance. */
  util::Statistics& statistics();

  /** @return The associated rewriter instance. */
  Rewriter& rewriter();

  /** @return The associated logger instance. */
  util::Logger& logger();

  /**
   * Configure associated termination configuration instance.
   * @note Only one terminator can be configured at a time. This will
   *       disconnect a previously configured terminator before configuring
   *       the given one.
   * @param terminator The terminator instance. Nullptr will disconnect the
   *                   terminator.
   */
  void configure_terminator(Terminator* terminator);

  /**
   * Terminate solving context instance if termination function `f_terminate`
   * has been configured.
   * @return True if instance has been terminated.
   */
  bool terminate() const;

 private:
  /** The configured options. */
  const option::Options d_options;
  /** The statistics. */
  util::Statistics d_statistics;
  /** The associated rewriter instance. */
  Rewriter d_rewriter;
  /** The associated terminator. */
  Terminator* d_terminator = nullptr;
  /** The associated logger class. */
  util::Logger d_logger;
};

}  // namespace bzla

#endif

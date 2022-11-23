#include "api/cpp/bitwuzla.h"

#include <array>
#include <sstream>

#include "bv/bitvector.h"
#include "node/node.h"
#include "node/node_kind.h"
#include "node/node_manager.h"
#include "option/option.h"
#include "solver/fp/floating_point.h"
#include "solver/fp/rounding_mode.h"
#include "solver/result.h"
#include "solving_context.h"

namespace bitwuzla {

/* -------------------------------------------------------------------------- */

/** Map api options to internal options. */
const std::unordered_map<Option, bzla::option::Option> s_internal_options = {
    {BITWUZLA_OPT_ENGINE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_EXIT_CODES, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_INPUT_FORMAT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_INCREMENTAL, bzla::option::Option::INCREMENTAL},
    {BITWUZLA_OPT_LOGLEVEL, bzla::option::Option::LOG_LEVEL},
    {BITWUZLA_OPT_OUTPUT_FORMAT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_OUTPUT_NUMBER_FORMAT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PRETTY_PRINT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PRINT_DIMACS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PRODUCE_MODELS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PRODUCE_UNSAT_CORES, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SAT_ENGINE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SEED, bzla::option::Option::SEED},
    {BITWUZLA_OPT_VERBOSITY, bzla::option::Option::VERBOSITY},
    {BITWUZLA_OPT_PP_ACKERMANN, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_BETA_REDUCE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_ELIMINATE_EXTRACTS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_ELIMINATE_ITES, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_EXTRACT_LAMBDAS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_MERGE_LAMBDAS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_NONDESTR_SUBST, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_NORMALIZE_ADD, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_SKELETON_PREPROC, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_UNCONSTRAINED_OPTIMIZATION,
     bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PP_VAR_SUBST, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_EXTRACT_ARITH, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_LEVEL, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_NORMALIZE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_NORMALIZE_ADD, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_SIMPLIFY_CONSTRAINTS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_SLT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_SORT_AIG, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_SORT_AIGVEC, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_RW_SORT_EXP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_DUAL_PROP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_DUAL_PROP_QSORT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_EAGER_LEMMAS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_LAZY_SYNTHESIZE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_JUST, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_JUST_HEURISTIC, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_PREPROP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_PREPROPOLD, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_PRESLS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_FUN_STORE_LAMBDAS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_JUST, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_GW, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_INC_MOVE_TEST, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_PROP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_PROP_FORCE_RW, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_PROP_NPROPS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_PROP_NSLSS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_RAND_ALL, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_RAND_RANGE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_RAND_WALK, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_RANGE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_MOVE_SEGMENT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_PROB_MOVE_RAND_WALK, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_NFLIPS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_STRATEGY, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_USE_RESTARTS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SLS_USE_BANDIT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_ASHR, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_CONST_BITS, bzla::option::Option::PROP_CONST_BITS},
    {BITWUZLA_OPT_PROP_CONST_DOMAINS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_ENTAILED, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_FLIP_COND_CONST_DELTA, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_FLIP_COND_CONST_NPATHSEL,
     bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_INFER_INEQ_BOUNDS,
     bzla::option::Option::PROP_INEQ_BOUNDS},
    {BITWUZLA_OPT_PROP_NO_MOVE_ON_CONFLICT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_NPROPS, bzla::option::Option::PROP_NPROPS},
    {BITWUZLA_OPT_PROP_NUPDATES, bzla::option::Option::PROP_NUPDATES},
    {BITWUZLA_OPT_PROP_PATH_SEL, bzla::option::Option::PROP_PATH_SEL},
    {BITWUZLA_OPT_PROP_PROB_FALLBACK_RANDOM_VALUE,
     bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_AND_FLIP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_EQ_FLIP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_FLIP_COND, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_FLIP_COND_CONST, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_RANDOM_INPUT,
     bzla::option::Option::PROP_PROB_PICK_RANDOM_INPUT},
    {BITWUZLA_OPT_PROP_PROB_SLICE_FLIP, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_SLICE_KEEP_DC, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_PROB_USE_INV_VALUE,
     bzla::option::Option::PROP_PROB_PICK_INV_VALUE},
    {BITWUZLA_OPT_PROP_USE_BANDIT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_USE_INV_LT_CONCAT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_USE_RESTARTS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_SEXT, bzla::option::Option::PROP_SEXT},
    {BITWUZLA_OPT_PROP_SKIP_NO_PROGRESS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PROP_XOR, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_AIGPROP_NPROPS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_AIGPROP_USE_BANDIT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_AIGPROP_USE_RESTARTS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_QUANT_SYNTH_SK, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_QUANT_SYNTH_QI, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_QUANT_SKOLEM_UF, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_QUANT_EAGER_SKOLEM, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_QUANT_MBQI, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_QUANT_MODE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_CHECK_MODEL, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_CHECK_UNCONSTRAINED, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_CHECK_UNSAT_ASSUMPTIONS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_DECLSORT_BV_WIDTH, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_LS_SHARE_SAT, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_PARSE_INTERACTIVE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SAT_ENGINE_CADICAL_FREEZE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SAT_ENGINE_LGL_FORK, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SAT_ENGINE_N_THREADS, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_SMT_COMP_MODE, bzla::option::Option::UNDEFINED},
    {BITWUZLA_OPT_NUM_OPTS, bzla::option::Option::UNDEFINED},
};

/** Map internal options to api options. */
const std::unordered_map<bzla::option::Option, Option> s_options = {
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_ENGINE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_EXIT_CODES},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_INPUT_FORMAT},
    {bzla::option::Option::INCREMENTAL, BITWUZLA_OPT_INCREMENTAL},
    {bzla::option::Option::LOG_LEVEL, BITWUZLA_OPT_LOGLEVEL},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_OUTPUT_FORMAT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_OUTPUT_NUMBER_FORMAT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PRETTY_PRINT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PRINT_DIMACS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PRODUCE_MODELS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PRODUCE_UNSAT_CORES},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SAT_ENGINE},
    {bzla::option::Option::SEED, BITWUZLA_OPT_SEED},
    {bzla::option::Option::VERBOSITY, BITWUZLA_OPT_VERBOSITY},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_ACKERMANN},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_BETA_REDUCE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_ELIMINATE_EXTRACTS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_ELIMINATE_ITES},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_EXTRACT_LAMBDAS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_MERGE_LAMBDAS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_NONDESTR_SUBST},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_NORMALIZE_ADD},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_SKELETON_PREPROC},
    //{bzla::option::Option::UNDEFINED,
    // BITWUZLA_OPT_PP_UNCONSTRAINED_OPTIMIZATION},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PP_VAR_SUBST},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_EXTRACT_ARITH},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_LEVEL},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_NORMALIZE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_NORMALIZE_ADD},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_SIMPLIFY_CONSTRAINTS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_SLT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_SORT_AIG},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_SORT_AIGVEC},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_RW_SORT_EXP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_DUAL_PROP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_DUAL_PROP_QSORT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_EAGER_LEMMAS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_LAZY_SYNTHESIZE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_JUST},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_JUST_HEURISTIC},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_PREPROP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_PREPROPOLD},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_PRESLS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_FUN_STORE_LAMBDAS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_JUST},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_GW},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_INC_MOVE_TEST},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_PROP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_PROP_FORCE_RW},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_PROP_NPROPS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_PROP_NSLSS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_RAND_ALL},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_RAND_RANGE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_RAND_WALK},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_RANGE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_MOVE_SEGMENT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_PROB_MOVE_RAND_WALK},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_NFLIPS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_STRATEGY},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_USE_RESTARTS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SLS_USE_BANDIT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_ASHR},
    {bzla::option::Option::PROP_CONST_BITS, BITWUZLA_OPT_PROP_CONST_BITS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_CONST_DOMAINS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_ENTAILED},
    //{bzla::option::Option::UNDEFINED,
    //BITWUZLA_OPT_PROP_FLIP_COND_CONST_DELTA},
    //{bzla::option::Option::UNDEFINED,
    // BITWUZLA_OPT_PROP_FLIP_COND_CONST_NPATHSEL},
    {bzla::option::Option::PROP_INEQ_BOUNDS,
     BITWUZLA_OPT_PROP_INFER_INEQ_BOUNDS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_NO_MOVE_ON_CONFLICT},
    {bzla::option::Option::PROP_NPROPS, BITWUZLA_OPT_PROP_NPROPS},
    {bzla::option::Option::PROP_NUPDATES, BITWUZLA_OPT_PROP_NUPDATES},
    {bzla::option::Option::PROP_PATH_SEL, BITWUZLA_OPT_PROP_PATH_SEL},
    //{bzla::option::Option::UNDEFINED,
    // BITWUZLA_OPT_PROP_PROB_FALLBACK_RANDOM_VALUE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_PROB_AND_FLIP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_PROB_EQ_FLIP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_PROB_FLIP_COND},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_PROB_FLIP_COND_CONST},
    {bzla::option::Option::PROP_PROB_PICK_RANDOM_INPUT,
     BITWUZLA_OPT_PROP_PROB_RANDOM_INPUT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_PROB_SLICE_FLIP},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_PROB_SLICE_KEEP_DC},
    {bzla::option::Option::PROP_PROB_PICK_INV_VALUE,
     BITWUZLA_OPT_PROP_PROB_USE_INV_VALUE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_USE_BANDIT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_USE_INV_LT_CONCAT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_USE_RESTARTS},
    {bzla::option::Option::PROP_SEXT, BITWUZLA_OPT_PROP_SEXT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_SKIP_NO_PROGRESS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PROP_XOR},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_AIGPROP_NPROPS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_AIGPROP_USE_BANDIT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_AIGPROP_USE_RESTARTS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_QUANT_SYNTH_SK},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_QUANT_SYNTH_QI},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_QUANT_SKOLEM_UF},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_QUANT_EAGER_SKOLEM},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_QUANT_MBQI},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_QUANT_MODE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_CHECK_MODEL},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_CHECK_UNCONSTRAINED},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_CHECK_UNSAT_ASSUMPTIONS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_DECLSORT_BV_WIDTH},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_LS_SHARE_SAT},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_PARSE_INTERACTIVE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SAT_ENGINE_CADICAL_FREEZE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SAT_ENGINE_LGL_FORK},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SAT_ENGINE_N_THREADS},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_SMT_COMP_MODE},
    //{bzla::option::Option::UNDEFINED, BITWUZLA_OPT_NUM_OPTS},
};

/** Map api result to internal result. */
const std::unordered_map<Result, bzla::Result> s_internal_results = {
    {Result::SAT, bzla::Result::SAT},
    {Result::UNSAT, bzla::Result::UNSAT},
    {Result::UNKNOWN, bzla::Result::UNKNOWN},
};

/** Map internal result to api result. */
const std::unordered_map<bzla::Result, Result> s_results = {
    {bzla::Result::SAT, Result::SAT},
    {bzla::Result::UNSAT, Result::UNSAT},
    {bzla::Result::UNKNOWN, Result::UNKNOWN},
};

/** Map api rounding mode to internal rounding mode. */
const std::unordered_map<RoundingMode, bzla::RoundingMode> s_internal_rms = {
    {RoundingMode::RNA, bzla::RoundingMode::RNA},
    {RoundingMode::RNE, bzla::RoundingMode::RNE},
    {RoundingMode::RTN, bzla::RoundingMode::RTN},
    {RoundingMode::RTP, bzla::RoundingMode::RTP},
    {RoundingMode::RTZ, bzla::RoundingMode::RTZ},
};

/** Map internal rounding mode to api rounding mode. */
const std::unordered_map<bzla::RoundingMode, RoundingMode> s_rms = {
    {bzla::RoundingMode::RNA, RoundingMode::RNA},
    {bzla::RoundingMode::RNE, RoundingMode::RNE},
    {bzla::RoundingMode::RTN, RoundingMode::RTN},
    {bzla::RoundingMode::RTP, RoundingMode::RTP},
    {bzla::RoundingMode::RTZ, RoundingMode::RTZ},
};

/** Map api node kind to internal node kind. */
const std::unordered_map<Kind, bzla::node::Kind> s_internal_kinds = {
    {Kind::CONSTANT, bzla::node::Kind::CONSTANT},
    {Kind::CONST_ARRAY, bzla::node::Kind::CONST_ARRAY},
    {Kind::VALUE, bzla::node::Kind::VALUE},
    {Kind::VARIABLE, bzla::node::Kind::VARIABLE},

    {Kind::AND, bzla::node::Kind::AND},
    {Kind::DISTINCT, bzla::node::Kind::DISTINCT},
    {Kind::EQUAL, bzla::node::Kind::EQUAL},
    {Kind::IFF, bzla::node::Kind::EQUAL},
    {Kind::IMPLIES, bzla::node::Kind::IMPLIES},
    {Kind::NOT, bzla::node::Kind::NOT},
    {Kind::OR, bzla::node::Kind::OR},
    {Kind::XOR, bzla::node::Kind::XOR},

    {Kind::ITE, bzla::node::Kind::ITE},

    {Kind::EXISTS, bzla::node::Kind::EXISTS},
    {Kind::FORALL, bzla::node::Kind::FORALL},

    {Kind::APPLY, bzla::node::Kind::APPLY},
    {Kind::LAMBDA, bzla::node::Kind::LAMBDA},

    {Kind::ARRAY_SELECT, bzla::node::Kind::SELECT},
    {Kind::ARRAY_STORE, bzla::node::Kind::STORE},

    {Kind::BV_ADD, bzla::node::Kind::BV_ADD},
    {Kind::BV_AND, bzla::node::Kind::BV_AND},
    {Kind::BV_ASHR, bzla::node::Kind::BV_ASHR},
    {Kind::BV_COMP, bzla::node::Kind::BV_COMP},
    {Kind::BV_CONCAT, bzla::node::Kind::BV_CONCAT},
    {Kind::BV_DEC, bzla::node::Kind::BV_DEC},
    {Kind::BV_INC, bzla::node::Kind::BV_INC},
    {Kind::BV_MUL, bzla::node::Kind::BV_MUL},
    {Kind::BV_NAND, bzla::node::Kind::BV_NAND},
    {Kind::BV_NEG, bzla::node::Kind::BV_NEG},
    {Kind::BV_NOR, bzla::node::Kind::BV_NOR},
    {Kind::BV_NOT, bzla::node::Kind::BV_NOT},
    {Kind::BV_OR, bzla::node::Kind::BV_OR},
    {Kind::BV_REDAND, bzla::node::Kind::BV_REDAND},
    {Kind::BV_REDOR, bzla::node::Kind::BV_REDOR},
    {Kind::BV_REDXOR, bzla::node::Kind::BV_REDXOR},
    {Kind::BV_ROL, bzla::node::Kind::BV_ROL},
    {Kind::BV_ROR, bzla::node::Kind::BV_ROR},
    {Kind::BV_SADD_OVERFLOW, bzla::node::Kind::BV_SADDO},
    {Kind::BV_SDIV_OVERFLOW, bzla::node::Kind::BV_SDIVO},
    {Kind::BV_SDIV, bzla::node::Kind::BV_SDIV},
    {Kind::BV_SGE, bzla::node::Kind::BV_SGE},
    {Kind::BV_SGT, bzla::node::Kind::BV_SGT},
    {Kind::BV_SHL, bzla::node::Kind::BV_SHL},
    {Kind::BV_SHR, bzla::node::Kind::BV_SHR},
    {Kind::BV_SLE, bzla::node::Kind::BV_SLE},
    {Kind::BV_SLT, bzla::node::Kind::BV_SLT},
    {Kind::BV_SMOD, bzla::node::Kind::BV_SMOD},
    {Kind::BV_SMUL_OVERFLOW, bzla::node::Kind::BV_SMULO},
    {Kind::BV_SREM, bzla::node::Kind::BV_SREM},
    {Kind::BV_SSUB_OVERFLOW, bzla::node::Kind::BV_SSUBO},
    {Kind::BV_SUB, bzla::node::Kind::BV_SUB},
    {Kind::BV_UADD_OVERFLOW, bzla::node::Kind::BV_UADDO},
    {Kind::BV_UDIV, bzla::node::Kind::BV_UDIV},
    {Kind::BV_UGE, bzla::node::Kind::BV_UGE},
    {Kind::BV_UGT, bzla::node::Kind::BV_UGT},
    {Kind::BV_ULE, bzla::node::Kind::BV_ULE},
    {Kind::BV_ULT, bzla::node::Kind::BV_ULT},
    {Kind::BV_UMUL_OVERFLOW, bzla::node::Kind::BV_UMULO},
    {Kind::BV_UREM, bzla::node::Kind::BV_UREM},
    {Kind::BV_USUB_OVERFLOW, bzla::node::Kind::BV_USUBO},
    {Kind::BV_XNOR, bzla::node::Kind::BV_XNOR},
    {Kind::BV_XOR, bzla::node::Kind::BV_XOR},
    {Kind::BV_EXTRACT, bzla::node::Kind::BV_EXTRACT},
    {Kind::BV_REPEAT, bzla::node::Kind::BV_REPEAT},
    {Kind::BV_ROLI, bzla::node::Kind::BV_ROLI},
    {Kind::BV_RORI, bzla::node::Kind::BV_RORI},
    {Kind::BV_SIGN_EXTEND, bzla::node::Kind::BV_SIGN_EXTEND},
    {Kind::BV_ZERO_EXTEND, bzla::node::Kind::BV_ZERO_EXTEND},

    {Kind::FP_ABS, bzla::node::Kind::FP_ABS},
    {Kind::FP_ADD, bzla::node::Kind::FP_ADD},
    {Kind::FP_DIV, bzla::node::Kind::FP_DIV},
    {Kind::FP_EQUAL, bzla::node::Kind::FP_EQUAL},
    {Kind::FP_FMA, bzla::node::Kind::FP_FMA},
    {Kind::FP_FP, bzla::node::Kind::FP_FP},
    {Kind::FP_GE, bzla::node::Kind::FP_GE},
    {Kind::FP_GT, bzla::node::Kind::FP_GT},
    {Kind::FP_IS_INF, bzla::node::Kind::FP_IS_INF},
    {Kind::FP_IS_NAN, bzla::node::Kind::FP_IS_NAN},
    {Kind::FP_IS_NEG, bzla::node::Kind::FP_IS_NEG},
    {Kind::FP_IS_NORMAL, bzla::node::Kind::FP_IS_NORMAL},
    {Kind::FP_IS_POS, bzla::node::Kind::FP_IS_POS},
    {Kind::FP_IS_SUBNORMAL, bzla::node::Kind::FP_IS_SUBNORMAL},
    {Kind::FP_IS_ZERO, bzla::node::Kind::FP_IS_ZERO},
    {Kind::FP_LEQ, bzla::node::Kind::FP_LE},
    {Kind::FP_LT, bzla::node::Kind::FP_LT},
    {Kind::FP_MAX, bzla::node::Kind::FP_MAX},
    {Kind::FP_MIN, bzla::node::Kind::FP_MIN},
    {Kind::FP_MUL, bzla::node::Kind::FP_MUL},
    {Kind::FP_NEG, bzla::node::Kind::FP_NEG},
    {Kind::FP_REM, bzla::node::Kind::FP_REM},
    {Kind::FP_RTI, bzla::node::Kind::FP_RTI},
    {Kind::FP_SQRT, bzla::node::Kind::FP_SQRT},
    {Kind::FP_SUB, bzla::node::Kind::FP_SUB},
    {Kind::FP_TO_FP_FROM_BV, bzla::node::Kind::FP_TO_FP_FROM_BV},
    {Kind::FP_TO_FP_FROM_FP, bzla::node::Kind::FP_TO_FP_FROM_FP},
    {Kind::FP_TO_FP_FROM_SBV, bzla::node::Kind::FP_TO_FP_FROM_SBV},
    {Kind::FP_TO_FP_FROM_UBV, bzla::node::Kind::FP_TO_FP_FROM_UBV},
    {Kind::FP_TO_SBV, bzla::node::Kind::FP_TO_SBV},
    {Kind::FP_TO_UBV, bzla::node::Kind::FP_TO_UBV},
};

/** Map internal node kind to api node kind. */
const std::unordered_map<bzla::node::Kind, Kind> s_kinds{
    {bzla::node::Kind::CONSTANT, Kind::CONSTANT},
    {bzla::node::Kind::CONST_ARRAY, Kind::CONST_ARRAY},
    {bzla::node::Kind::VALUE, Kind::VALUE},
    {bzla::node::Kind::VARIABLE, Kind::VARIABLE},

    {bzla::node::Kind::AND, Kind::AND},
    {bzla::node::Kind::DISTINCT, Kind::DISTINCT},
    {bzla::node::Kind::EQUAL, Kind::EQUAL},
    {bzla::node::Kind::EQUAL, Kind::IFF},
    {bzla::node::Kind::IMPLIES, Kind::IMPLIES},
    {bzla::node::Kind::NOT, Kind::NOT},
    {bzla::node::Kind::OR, Kind::OR},
    {bzla::node::Kind::XOR, Kind::XOR},

    {bzla::node::Kind::ITE, Kind::ITE},

    {bzla::node::Kind::EXISTS, Kind::EXISTS},
    {bzla::node::Kind::FORALL, Kind::FORALL},

    {bzla::node::Kind::APPLY, Kind::APPLY},
    {bzla::node::Kind::LAMBDA, Kind::LAMBDA},

    {bzla::node::Kind::SELECT, Kind::ARRAY_SELECT},
    {bzla::node::Kind::STORE, Kind::ARRAY_STORE},

    {bzla::node::Kind::BV_ADD, Kind::BV_ADD},
    {bzla::node::Kind::BV_AND, Kind::BV_AND},
    {bzla::node::Kind::BV_ASHR, Kind::BV_ASHR},
    {bzla::node::Kind::BV_COMP, Kind::BV_COMP},
    {bzla::node::Kind::BV_CONCAT, Kind::BV_CONCAT},
    {bzla::node::Kind::BV_DEC, Kind::BV_DEC},
    {bzla::node::Kind::BV_INC, Kind::BV_INC},
    {bzla::node::Kind::BV_MUL, Kind::BV_MUL},
    {bzla::node::Kind::BV_NAND, Kind::BV_NAND},
    {bzla::node::Kind::BV_NEG, Kind::BV_NEG},
    {bzla::node::Kind::BV_NOR, Kind::BV_NOR},
    {bzla::node::Kind::BV_NOT, Kind::BV_NOT},
    {bzla::node::Kind::BV_OR, Kind::BV_OR},
    {bzla::node::Kind::BV_REDAND, Kind::BV_REDAND},
    {bzla::node::Kind::BV_REDOR, Kind::BV_REDOR},
    {bzla::node::Kind::BV_REDXOR, Kind::BV_REDXOR},
    {bzla::node::Kind::BV_ROL, Kind::BV_ROL},
    {bzla::node::Kind::BV_ROR, Kind::BV_ROR},
    {bzla::node::Kind::BV_SADDO, Kind::BV_SADD_OVERFLOW},
    {bzla::node::Kind::BV_SDIVO, Kind::BV_SDIV_OVERFLOW},
    {bzla::node::Kind::BV_SDIV, Kind::BV_SDIV},
    {bzla::node::Kind::BV_SGE, Kind::BV_SGE},
    {bzla::node::Kind::BV_SGT, Kind::BV_SGT},
    {bzla::node::Kind::BV_SHL, Kind::BV_SHL},
    {bzla::node::Kind::BV_SHR, Kind::BV_SHR},
    {bzla::node::Kind::BV_SLE, Kind::BV_SLE},
    {bzla::node::Kind::BV_SLT, Kind::BV_SLT},
    {bzla::node::Kind::BV_SMOD, Kind::BV_SMOD},
    {bzla::node::Kind::BV_SMULO, Kind::BV_SMUL_OVERFLOW},
    {bzla::node::Kind::BV_SREM, Kind::BV_SREM},
    {bzla::node::Kind::BV_SSUBO, Kind::BV_SSUB_OVERFLOW},
    {bzla::node::Kind::BV_SUB, Kind::BV_SUB},
    {bzla::node::Kind::BV_UADDO, Kind::BV_UADD_OVERFLOW},
    {bzla::node::Kind::BV_UDIV, Kind::BV_UDIV},
    {bzla::node::Kind::BV_UGE, Kind::BV_UGE},
    {bzla::node::Kind::BV_UGT, Kind::BV_UGT},
    {bzla::node::Kind::BV_ULE, Kind::BV_ULE},
    {bzla::node::Kind::BV_ULT, Kind::BV_ULT},
    {bzla::node::Kind::BV_UMULO, Kind::BV_UMUL_OVERFLOW},
    {bzla::node::Kind::BV_UREM, Kind::BV_UREM},
    {bzla::node::Kind::BV_USUBO, Kind::BV_USUB_OVERFLOW},
    {bzla::node::Kind::BV_XNOR, Kind::BV_XNOR},
    {bzla::node::Kind::BV_XOR, Kind::BV_XOR},
    {bzla::node::Kind::BV_EXTRACT, Kind::BV_EXTRACT},
    {bzla::node::Kind::BV_REPEAT, Kind::BV_REPEAT},
    {bzla::node::Kind::BV_ROLI, Kind::BV_ROLI},
    {bzla::node::Kind::BV_RORI, Kind::BV_RORI},
    {bzla::node::Kind::BV_SIGN_EXTEND, Kind::BV_SIGN_EXTEND},
    {bzla::node::Kind::BV_ZERO_EXTEND, Kind::BV_ZERO_EXTEND},

    {bzla::node::Kind::FP_ABS, Kind::FP_ABS},
    {bzla::node::Kind::FP_ADD, Kind::FP_ADD},
    {bzla::node::Kind::FP_DIV, Kind::FP_DIV},
    {bzla::node::Kind::FP_EQUAL, Kind::FP_EQUAL},
    {bzla::node::Kind::FP_FMA, Kind::FP_FMA},
    {bzla::node::Kind::FP_FP, Kind::FP_FP},
    {bzla::node::Kind::FP_GE, Kind::FP_GE},
    {bzla::node::Kind::FP_GT, Kind::FP_GT},
    {bzla::node::Kind::FP_IS_INF, Kind::FP_IS_INF},
    {bzla::node::Kind::FP_IS_NAN, Kind::FP_IS_NAN},
    {bzla::node::Kind::FP_IS_NEG, Kind::FP_IS_NEG},
    {bzla::node::Kind::FP_IS_NORMAL, Kind::FP_IS_NORMAL},
    {bzla::node::Kind::FP_IS_POS, Kind::FP_IS_POS},
    {bzla::node::Kind::FP_IS_SUBNORMAL, Kind::FP_IS_SUBNORMAL},
    {bzla::node::Kind::FP_IS_ZERO, Kind::FP_IS_ZERO},
    {bzla::node::Kind::FP_LE, Kind::FP_LEQ},
    {bzla::node::Kind::FP_LT, Kind::FP_LT},
    {bzla::node::Kind::FP_MAX, Kind::FP_MAX},
    {bzla::node::Kind::FP_MIN, Kind::FP_MIN},
    {bzla::node::Kind::FP_MUL, Kind::FP_MUL},
    {bzla::node::Kind::FP_NEG, Kind::FP_NEG},
    {bzla::node::Kind::FP_REM, Kind::FP_REM},
    {bzla::node::Kind::FP_RTI, Kind::FP_RTI},
    {bzla::node::Kind::FP_SQRT, Kind::FP_SQRT},
    {bzla::node::Kind::FP_SUB, Kind::FP_SUB},
    {bzla::node::Kind::FP_TO_FP_FROM_BV, Kind::FP_TO_FP_FROM_BV},
    {bzla::node::Kind::FP_TO_FP_FROM_FP, Kind::FP_TO_FP_FROM_FP},
    {bzla::node::Kind::FP_TO_FP_FROM_SBV, Kind::FP_TO_FP_FROM_SBV},
    {bzla::node::Kind::FP_TO_FP_FROM_UBV, Kind::FP_TO_FP_FROM_UBV},
    {bzla::node::Kind::FP_TO_SBV, Kind::FP_TO_SBV},
    {bzla::node::Kind::FP_TO_UBV, Kind::FP_TO_UBV},
};

/* -------------------------------------------------------------------------- */

std::ostream &
operator<<(std::ostream &out, Result result)
{
  out << s_internal_results.at(result);
  return out;
}

std::ostream &
operator<<(std::ostream &out, Kind kind)
{
  out << s_internal_kinds.at(kind);
  return out;
}

std::ostream &
operator<<(std::ostream &out, RoundingMode rm)
{
  out << s_internal_rms.at(rm);
  return out;
}

/* Options public ----------------------------------------------------------- */

void
Options::set(Option option, uint64_t value)
{
  // TODO check
  d_options->set_option_numeric(s_internal_options.at(option), value);
}

void
Options::set(Option option, const std::string &mode)
{
  // TODO check
  d_options->set_option_enum(s_internal_options.at(option), mode);
}

uint64_t
Options::get_numeric(Option option) const
{
  // TODO check;
  return d_options->get_option_numeric(s_internal_options.at(option));
}

bool
Options::get_bool(Option option) const
{
  // TODO check
  return d_options->get_option_bool(s_internal_options.at(option));
}

const std::string &
Options::get_mode(Option option) const
{
  // TODO check
  return d_options->get_option_enum(s_internal_options.at(option));
}

const OptionInfo &
Options::get_info(Option option) const
{
  // TODO
  (void) option;
}

/* Term public -------------------------------------------------------------- */

Term::Term() : d_node(nullptr) {}

Term::~Term() {}

bool
Term::is_null() const
{
  return d_node == nullptr;
}

Kind
Term::kind() const
{
  // TODO check
  return s_kinds.at(d_node->kind());
}

Sort
Term::sort() const
{
  // TODO check
  return d_node->type();
}

size_t
Term::num_children() const
{
  // TODO check
  return d_node->num_children();
}

std::vector<Term>
Term::children() const
{
  // TODO check
  std::vector<Term> res;
  for (const auto &node : *d_node)
  {
    res.push_back(node);
  }
  return res;
}

size_t
Term::num_indices() const
{
  // TODO check
  return d_node->num_indices();
}

std::vector<uint64_t>
Term::indices() const
{
  // TODO check
  return d_node->indices();
}

std::optional<std::reference_wrapper<const std::string>>
Term::symbol() const
{
  // TODO check
  return d_node->symbol();
}

#if 0
void
Term::set_symbol(
    std::optional<std::reference_wrapper<const std::string>> symbol)
{
  // TODO
}
#endif

bool
Term::is_const() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::CONSTANT;
}

bool
Term::is_var() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VARIABLE;
}

bool
Term::is_value() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE;
}

bool
Term::is_bv_value_zero() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_bv()
         && d_node->value<bzla::BitVector>().is_zero();
}

bool
Term::is_bv_value_one() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_bv()
         && d_node->value<bzla::BitVector>().is_one();
}

bool
Term::is_bv_value_ones() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_bv()
         && d_node->value<bzla::BitVector>().is_ones();
}

bool
Term::is_bv_value_min_signed() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_bv()
         && d_node->value<bzla::BitVector>().is_min_signed();
}

bool
Term::is_bv_value_max_signed() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_bv()
         && d_node->value<bzla::BitVector>().is_max_signed();
}

bool
Term::is_fp_value_pos_zero() const
{
  // TODO check
  bool res =
      d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_fp();
  if (res)
  {
    const bzla::FloatingPoint &fp = d_node->value<bzla::FloatingPoint>();
    res                           = fp.fpispos() && fp.fpiszero();
  }
  return res;
}

bool
Term::is_fp_value_neg_zero() const
{
  // TODO chck
  bool res =
      d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_fp();
  if (res)
  {
    const bzla::FloatingPoint &fp = d_node->value<bzla::FloatingPoint>();
    res                           = fp.fpisneg() && fp.fpiszero();
  }
  return res;
}

bool
Term::is_fp_value_pos_inf() const
{
  // TODO check
  bool res =
      d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_fp();
  if (res)
  {
    const bzla::FloatingPoint &fp = d_node->value<bzla::FloatingPoint>();
    res                           = fp.fpispos() && fp.fpisinf();
  }
  return res;
}

bool
Term::is_fp_value_neg_inf() const
{
  // TODO check
  bool res =
      d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_fp();
  if (res)
  {
    const bzla::FloatingPoint &fp = d_node->value<bzla::FloatingPoint>();
    res                           = fp.fpisneg() && fp.fpisinf();
  }
  return res;
}

bool
Term::is_fp_value_nan() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_fp()
         && d_node->value<bzla::FloatingPoint>().fpisnan();
}

bool
Term::is_rm_value_rna() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_rm()
         && d_node->value<bzla::RoundingMode>() == bzla::RoundingMode::RNA;
}

bool
Term::is_rm_value_rne() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_rm()
         && d_node->value<bzla::RoundingMode>() == bzla::RoundingMode::RNE;
}

bool
Term::is_rm_value_rtn() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_rm()
         && d_node->value<bzla::RoundingMode>() == bzla::RoundingMode::RTN;
}

bool
Term::is_rm_value_rtp() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_rm()
         && d_node->value<bzla::RoundingMode>() == bzla::RoundingMode::RTP;
}

bool
Term::is_rm_value_rtz() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::VALUE && d_node->type().is_rm()
         && d_node->value<bzla::RoundingMode>() == bzla::RoundingMode::RTZ;
}

bool
Term::is_const_array() const
{
  // TODO check
  return d_node->kind() == bzla::node::Kind::CONST_ARRAY;
}

std::string
Term::str()
{
  // TODO check
  return d_node->str();
}

bool
operator==(const Term &a, const Term &b)
{
  // TODO check
  return *a.d_node == *b.d_node;
}

std::ostream &
operator<<(std::ostream &out, const Term &term)
{
  out << term.d_node;
  return out;
}

/* Sort public -------------------------------------------------------------- */

Sort::Sort() : d_type(nullptr) {}

Sort::~Sort() {}

bool
Sort::is_null() const
{
  return d_type == nullptr;
}

uint64_t
Sort::bv_size() const
{
  // TODO check
  return d_type->bv_size();
}

uint64_t
Sort::fp_exp_size() const
{
  // TODO check
  return d_type->fp_exp_size();
}

uint64_t
Sort::fp_sig_size() const
{
  // TODO check
  return d_type->fp_sig_size();
}

Sort
Sort::array_get_index() const
{
  // TODO check
  return d_type->array_index();
}

Sort
Sort::array_get_element() const
{
  // TODO check
  return d_type->array_element();
}

std::vector<Sort>
Sort::fun_get_domain() const
{
  // TODO check
  const std::vector<bzla::Type> types = d_type->fun_types();
  assert(types.size() > 0);
  std::vector<Sort> res;
  for (size_t i = 0, n = types.size() - 1; i < n; ++i)
  {
    res.push_back(types[i]);
  }
  return res;
}

Sort
Sort::fun_get_codomain() const
{
  // TODO check
  const std::vector<bzla::Type> types = d_type->fun_types();
  assert(types.size() > 0);
  return types.back();
}

size_t
Sort::fun_arity() const
{
  // TODO check
  return d_type->fun_arity();
}

bool
Sort::is_array() const
{
  // TODO check
  return d_type->is_array();
}

bool
Sort::is_bool() const
{
  // TODO check
  return d_type->is_bool();
}

bool
Sort::is_bv() const
{
  // TODO check
  return d_type->is_bv();
}

bool
Sort::is_fp() const
{
  // TODO check
  return d_type->is_fp();
}

bool
Sort::is_fun() const
{
  // TODO check
  return d_type->is_fun();
}

bool
Sort::is_rm() const
{
  // TODO check
  return d_type->is_rm();
}

std::string
Sort::str() const
{
  // TODO check
  return d_type->str();
}

bool
operator==(const Sort &a, const Sort &b)
{
  return a.d_type == b.d_type;
}

std::ostream &
operator<<(std::ostream &out, const Sort &sort)
{
  out << *sort.d_type;
  return out;
}

/* Bitwuzla public ---------------------------------------------------------- */

Bitwuzla::Bitwuzla(const Options &options)
    : d_ctx(new bzla::SolvingContext(*options.d_options))
{
}

Bitwuzla::~Bitwuzla() {}

bool
Bitwuzla::terminate()
{
  // TODO
  return false;
}

void
Bitwuzla::set_termination_callback(std::function<int32_t(void *)> fun,
                                   void *state)
{
  // TODO
  (void) fun;
  (void) state;
}

void *
Bitwuzla::get_termination_callback_state()
{
  // TODO
  return nullptr;
}

void
Bitwuzla::set_abort_callback(std::function<void(const std::string &)> fun)
{
  // TODO
  (void) fun;
}

void
Bitwuzla::push(uint32_t nlevels)
{
  // TODO check
  for (uint32_t i = 0; i < nlevels; ++i)
  {
    d_ctx->push();
  }
}

void
Bitwuzla::pop(uint32_t nlevels)
{
  // TODO check
  for (uint32_t i = 0; i < nlevels; ++i)
  {
    d_ctx->pop();
  }
}

void
Bitwuzla::assert_formula(const Term &term)
{
  // TODO check
  d_ctx->assert_formula(*term.d_node);
}

#if 0
void
Bitwuzla::assume_formula(const Term &term)
{
  // TODO
}
#endif

bool
Bitwuzla::is_unsat_assumption(const Term &term)
{
  // TODO (not implemented yet)
  (void) term;
  return false;
}

std::vector<Term>
Bitwuzla::bitwuzla_get_unsat_assumptions()
{
  // TODO (not implemented yet)
  return {};
}

std::vector<Term>
Bitwuzla::bitwuzla_get_unsat_core()
{
  // TODO (not implemented yet)
  return {};
}

#if 0
void
Bitwuzla::reset_assumptions()
{
  // TODO
}
#endif

Result
Bitwuzla::simplify()
{
  // TODO (not implemented yet)
  return Result::UNKNOWN;
}

Result
Bitwuzla::check_sat(const std::vector<Term> &assumptions)
{
  // TODO check
  assert(
      assumptions.empty());  // TODO assumption handling (not implemented yet)
  return s_results.at(d_ctx->solve());
}

Term
Bitwuzla::get_value(const Term &term)
{
  // TODO check
  return d_ctx->get_value(*term.d_node);
}

std::string
Bitwuzla::get_bv_value(const Term &term, uint8_t base)
{
  // TODO check
  bzla::Node value = d_ctx->get_value(*term.d_node);
  assert(value.is_value());
  return value.value<bzla::BitVector>().to_string(base);
}

std::string
Bitwuzla::get_fp_value(const Term &term, uint8_t base)
{
  // TODO check
  const bzla::FloatingPoint &fp_value =
      d_ctx->get_value(*term.d_node).value<bzla::FloatingPoint>();
  bzla::BitVector sign, exp, sig;
  bzla::FloatingPoint::ieee_bv_as_bvs(
      term.d_node->type(), fp_value.as_bv(), sign, exp, sig);
  std::string prefix = base == 2 ? "#b" : (base == 16 ? "#x" : "");
  return "(fp " + prefix + sign.to_string(base) + " " + prefix
         + exp.to_string(base) + " " + prefix + sig.to_string(base);
}

RoundingMode
Bitwuzla::get_rm_value(const Term &term)
{
  // TODO check
  return s_rms.at(d_ctx->get_value(*term.d_node).value<bzla::RoundingMode>());
}

#if 0
void
Bitwuzla::get_array_value(const Term &term,
                          std::vector<Term> &indices,
                          std::vector<Term> &values,
                          Term &default_value)
{
  // TODO
}

void
Bitwuzla::get_fun_value(const Term &term,
                        std::vector<std::vector<Term>> args,
                        std::vector<Term> values)
{
  // TODO
}

void
Bitwuzla::print_model(std::ostream &out, const std::string &format)
{
  // TODO
}
#endif

void
Bitwuzla::dump_formula(std::ostream &out, const std::string &format)
{
  // TODO
  (void) out;
  (void) format;
}

Result
Bitwuzla::parse(std::ifstream &infile,
                const std::string &infile_name,
                std::string &error_msg,
                Result &status,
                bool &is_smt2)
{
  // TODO
  (void) infile;
  (void) infile_name;
  (void) error_msg;
  (void) status;
  (void) is_smt2;
  return Result::UNKNOWN;
}

Result
Bitwuzla::parse(const std::string &format,
                std::ifstream &infile,
                const std::string &infile_name,
                std::string &error_msg,
                Result &status,
                bool &is_smt2)
{
  // TODO
  (void) format;
  (void) infile;
  (void) infile_name;
  (void) error_msg;
  (void) status;
  (void) is_smt2;
  return Result::UNKNOWN;
}

/* -------------------------------------------------------------------------- */
Sort
mk_array_sort(const Sort &index, const Sort &element)
{
  // TODO check
  return bzla::NodeManager::get().mk_array_type(*index.d_type, *element.d_type);
}

Sort
mk_bool_sort()
{
  // TODO check
  return bzla::NodeManager::get().mk_bool_type();
}

Sort
mk_bv_sort(uint64_t size)
{
  // TODO check
  return bzla::NodeManager::get().mk_bv_type(size);
}

Sort
mk_fp_sort(uint64_t exp_size, uint64_t sig_size)
{
  // TODO check
  return bzla::NodeManager::get().mk_fp_type(exp_size, sig_size);
}

Sort
mk_fun_sort(const std::vector<Sort> &domain, const Sort &codomain)
{
  // TODO check
  std::vector<bzla::Type> types = Sort::sort_vector_to_types(domain);
  types.push_back(*codomain.d_type);
  return bzla::NodeManager::get().mk_fun_type(types);
}

Sort
mk_rm_sort()
{
  // TODO check
  return bzla::NodeManager::get().mk_rm_type();
}

Term
mk_true()
{
  // TODO check
  return bzla::NodeManager::get().mk_value(true);
}

Term
mk_false()
{
  // TODO check
  return bzla::NodeManager::get().mk_value(false);
}

Term
mk_bv_zero(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::mk_zero(sort.d_type->bv_size()));
}

Term
mk_bv_one(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::mk_one(sort.d_type->bv_size()));
}

Term
mk_bv_ones(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::mk_ones(sort.d_type->bv_size()));
}

Term
mk_bv_min_signed(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::mk_min_signed(sort.d_type->bv_size()));
}

Term
mk_bv_max_signed(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::mk_max_signed(sort.d_type->bv_size()));
}

Term
mk_bv_value(const Sort &sort, const std::string &value, uint8_t base)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector(sort.d_type->bv_size(), value, base));
}

Term
mk_bv_value_uint64(const Sort &sort, uint64_t value)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::from_ui(sort.d_type->bv_size(), value));
}

Term
mk_bv_value_int64(const Sort &sort, int64_t value)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::BitVector::from_si(sort.d_type->bv_size(), value));
}

Term
mk_fp_pos_zero(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::FloatingPoint::fpzero(*sort.d_type, false));
}

Term
mk_fp_neg_zero(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::FloatingPoint::fpzero(*sort.d_type, true));
}

Term
mk_fp_pos_inf(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::FloatingPoint::fpinf(*sort.d_type, false));
}

Term
mk_fp_neg_inf(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::FloatingPoint::fpinf(*sort.d_type, true));
}

Term
mk_fp_nan(const Sort &sort)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(
      bzla::FloatingPoint::fpnan(*sort.d_type));
}

Term
mk_fp_value(const Term &bv_sign,
            const Term &bv_exponent,
            const Term &bv_significand)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(bzla::FloatingPoint(
      bzla::NodeManager::get().mk_fp_type(
          bv_exponent.d_node->type().bv_size(),
          bv_significand.d_node->type().bv_size() + 1),
      bv_sign.d_node->value<bzla::BitVector>()
          .bvconcat(bv_exponent.d_node->value<bzla::BitVector>())
          .ibvconcat(bv_significand.d_node->value<bzla::BitVector>())));
}

Term
mk_fp_value_from_real(const Sort &sort, const Term &rm, const std::string &real)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(bzla::FloatingPoint::from_real(
      *sort.d_type, rm.d_node->value<bzla::RoundingMode>(), real));
}

Term
mk_fp_value_from_rational(const Sort &sort,
                          const Term &rm,
                          const std::string &num,
                          const std::string &den)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(bzla::FloatingPoint::from_rational(
      *sort.d_type, rm.d_node->value<bzla::RoundingMode>(), num, den));
}

Term
mk_rm_value(RoundingMode rm)
{
  // TODO check
  return bzla::NodeManager::get().mk_value(s_internal_rms.at(rm));
}

Term
mk_term(Kind kind,
        const std::vector<Term> &args,
        const std::vector<uint64_t> indices)
{
  // TODO check
  return bzla::NodeManager::get().mk_node(
      s_internal_kinds.at(kind), Term::term_vector_to_nodes(args), indices);
}

Term
mk_const(const Sort &sort,
         std::optional<std::reference_wrapper<const std::string>> symbol)
{
  // TODO check
  return bzla::NodeManager::get().mk_const(*sort.d_type, symbol);
}

Term
mk_var(const Sort &sort,
       std::optional<std::reference_wrapper<const std::string>> symbol)
{
  // TODO check
  return bzla::NodeManager::get().mk_var(*sort.d_type, symbol);
}

Term
substitute_term(const Term &term, const std::unordered_map<Term, Term> map)
{
  // TODO
  (void) term;
  (void) map;
  return Term();
}

void
substitute_terms(std::vector<Term> &terms,
                 const std::unordered_map<Term, Term> map)
{
  // TODO
  (void) terms;
  (void) map;
}

/* Term private ------------------------------------------------------------- */

Term::Term(const bzla::Node &node) : d_node(new bzla::Node(node)) {}

std::vector<bzla::Node>
Term::term_vector_to_nodes(const std::vector<Term> &terms)
{
  std::vector<bzla::Node> res;
  for (const auto &term : terms)
  {
    res.push_back(*term.d_node);
  }
  return res;
}

/* Sort private ------------------------------------------------------------- */

Sort::Sort(const bzla::Type &type) : d_type(new bzla::Type(type)) {}

std::vector<bzla::Type>
Sort::sort_vector_to_types(const std::vector<Sort> &sorts)
{
  std::vector<bzla::Type> res;
  for (const auto &sort : sorts)
  {
    res.push_back(*sort.d_type);
  }
  return res;
}

/* -------------------------------------------------------------------------- */
}  // namespace bitwuzla
/* -------------------------------------------------------------------------- */

namespace std {

size_t
std::hash<bitwuzla::Sort>::operator()(const bitwuzla::Sort &sort) const
{
  // TODO check
  return std::hash<bzla::Type>{}(*sort.d_type);
}

size_t
std::hash<bitwuzla::Term>::operator()(const bitwuzla::Term &term) const
{
  // TODO check
  return std::hash<bzla::Node>{}(*term.d_node);
}

}  // namespace std
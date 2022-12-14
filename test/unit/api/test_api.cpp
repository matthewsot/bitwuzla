#include "api/checks.h"
#include "api/cpp/bitwuzla.h"
#include "test/unit/test.h"

namespace bzla::test {

class TestApi : public ::testing::Test
{
 protected:
  // sorts
  bitwuzla::Sort d_bv_sort1  = bitwuzla::mk_bv_sort(1);
  bitwuzla::Sort d_bv_sort23 = bitwuzla::mk_bv_sort(23);
  bitwuzla::Sort d_bv_sort32 = bitwuzla::mk_bv_sort(32);
  bitwuzla::Sort d_bv_sort8  = bitwuzla::mk_bv_sort(8);
  bitwuzla::Sort d_fp_sort16 = bitwuzla::mk_fp_sort(5, 11);
  bitwuzla::Sort d_fp_sort32 = bitwuzla::mk_fp_sort(8, 24);
  bitwuzla::Sort d_arr_sort_bv =
      bitwuzla::mk_array_sort(d_bv_sort32, d_bv_sort8);
  bitwuzla::Sort d_arr_sort_bvfp =
      bitwuzla::mk_array_sort(d_bv_sort8, d_fp_sort16);
  bitwuzla::Sort d_arr_sort_fpbv =
      bitwuzla::mk_array_sort(d_fp_sort16, d_bv_sort8);
  std::vector<bitwuzla::Sort> d_fun_domain_sorts{
      d_bv_sort8, d_fp_sort16, d_bv_sort32};
  bitwuzla::Sort d_fun_sort =
      bitwuzla::mk_fun_sort(d_fun_domain_sorts, d_bv_sort8);
  bitwuzla::Sort d_fun_sort_fp =
      bitwuzla::mk_fun_sort(d_fun_domain_sorts, d_fp_sort16);
  bitwuzla::Sort d_rm_sort = bitwuzla::mk_rm_sort();
  // terms
  bitwuzla::Term d_true       = bitwuzla::mk_true();
  bitwuzla::Term d_bv_one1    = bitwuzla::mk_bv_one(d_bv_sort1);
  bitwuzla::Term d_bv_ones23  = bitwuzla::mk_bv_ones(d_bv_sort23);
  bitwuzla::Term d_bv_zero8   = bitwuzla::mk_bv_zero(d_bv_sort8);
  bitwuzla::Term d_bv_mins8   = bitwuzla::mk_bv_min_signed(d_bv_sort8);
  bitwuzla::Term d_bv_maxs8   = bitwuzla::mk_bv_max_signed(d_bv_sort8);
  bitwuzla::Term d_fp_pzero32 = bitwuzla::mk_fp_pos_zero(d_fp_sort32);
  bitwuzla::Term d_fp_nzero32 = bitwuzla::mk_fp_neg_zero(d_fp_sort32);
  bitwuzla::Term d_fp_pinf32  = bitwuzla::mk_fp_pos_inf(d_fp_sort32);
  bitwuzla::Term d_fp_ninf32  = bitwuzla::mk_fp_neg_inf(d_fp_sort32);
  bitwuzla::Term d_fp_nan32   = bitwuzla::mk_fp_nan(d_fp_sort32);

  bitwuzla::Term d_bool_const = bitwuzla::mk_const(bitwuzla::mk_bool_sort());
  bitwuzla::Term d_bv_const1  = bitwuzla::mk_const(d_bv_sort1);
  bitwuzla::Term d_bv_const8  = bitwuzla::mk_const(d_bv_sort8);
  bitwuzla::Term d_fp_const16 = bitwuzla::mk_const(d_fp_sort16);
  bitwuzla::Term d_rm_const   = bitwuzla::mk_const(d_rm_sort);

  bitwuzla::Term d_rm_rna = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RNA);
  bitwuzla::Term d_rm_rne = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RNE);
  bitwuzla::Term d_rm_rtn = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RTN);
  bitwuzla::Term d_rm_rtp = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RTP);
  bitwuzla::Term d_rm_rtz = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RTZ);

  bitwuzla::Term d_fun        = bitwuzla::mk_const(d_fun_sort);
  bitwuzla::Term d_fun_fp     = bitwuzla::mk_const(d_fun_sort_fp);
  bitwuzla::Term d_array_fpbv = bitwuzla::mk_const(d_arr_sort_fpbv);
  bitwuzla::Term d_array      = bitwuzla::mk_const(d_arr_sort_bv);
  bitwuzla::Term d_store =
      bitwuzla::mk_term(bitwuzla::Kind::ARRAY_STORE,
                        {d_array, bitwuzla::mk_const(d_bv_sort32), d_bv_zero8});

  bitwuzla::Term d_var1      = bitwuzla::mk_var(d_bv_sort8);
  bitwuzla::Term d_var2      = bitwuzla::mk_var(d_bv_sort8);
  bitwuzla::Term d_bound_var = bitwuzla::mk_var(d_bv_sort8);
  bitwuzla::Term d_bool_var  = bitwuzla::mk_var(bitwuzla::mk_bool_sort());

  bitwuzla::Term d_not_bv_const1 = bitwuzla::mk_term(
      bitwuzla::Kind::EQUAL,
      {d_bv_one1, bitwuzla::mk_term(bitwuzla::Kind::BV_NOT, {d_bv_const1})});
  bitwuzla::Term d_and_bv_const1 = bitwuzla::mk_term(
      bitwuzla::Kind::EQUAL,
      {d_bv_one1,
       bitwuzla::mk_term(bitwuzla::Kind::BV_AND, {d_bv_one1, d_bv_const1})});
  bitwuzla::Term d_eq_bv_const8 =
      bitwuzla::mk_term(bitwuzla::Kind::EQUAL, {d_bv_const8, d_bv_zero8});

  bitwuzla::Term d_lambda = bitwuzla::mk_term(
      bitwuzla::Kind::LAMBDA,
      {d_bound_var,
       bitwuzla::mk_term(bitwuzla::Kind::BV_ADD, {d_bound_var, d_bv_const8})});
  bitwuzla::Term d_bool_lambda_body =
      bitwuzla::mk_term(bitwuzla::Kind::EQUAL, {d_bool_var, d_true});
  bitwuzla::Term d_bool_lambda = bitwuzla::mk_term(
      bitwuzla::Kind::LAMBDA, {d_bool_var, d_bool_lambda_body});
  bitwuzla::Term d_bool_apply =
      bitwuzla::mk_term(bitwuzla::Kind::APPLY, {d_bool_lambda, d_true});

  bitwuzla::Term d_exists_var = bitwuzla::mk_var(d_bv_sort8);
  bitwuzla::Term d_exists     = bitwuzla::mk_term(
      bitwuzla::Kind::EXISTS,
      {d_exists_var,
           bitwuzla::mk_term(bitwuzla::Kind::EQUAL,
                         {d_bv_zero8,
                              bitwuzla::mk_term(bitwuzla::Kind::BV_MUL,
                                            {d_bv_const8, d_exists_var})})});
};

/* -------------------------------------------------------------------------- */
/* Kind                                                                       */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, kind_to_string)
{
  ASSERT_EQ(std::to_string(bitwuzla::Kind::CONSTANT), std::string("CONSTANT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::CONST_ARRAY),
            std::string("CONST_ARRAY"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::VARIABLE), std::string("VARIABLE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::VALUE), std::string("VALUE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::AND), std::string("AND"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::APPLY), std::string("APPLY"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::ARRAY_SELECT),
            std::string("SELECT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::ARRAY_STORE), std::string("STORE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ADD), std::string("BV_ADD"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_AND), std::string("BV_AND"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ASHR), std::string("BV_ASHR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_COMP), std::string("BV_COMP"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_CONCAT),
            std::string("BV_CONCAT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_DEC), std::string("BV_DEC"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_INC), std::string("BV_INC"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_MUL), std::string("BV_MUL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_NAND), std::string("BV_NAND"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_NEG), std::string("BV_NEG"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_NOR), std::string("BV_NOR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_NOT), std::string("BV_NOT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_OR), std::string("BV_OR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_REDAND),
            std::string("BV_REDAND"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_REDOR), std::string("BV_REDOR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_REDXOR),
            std::string("BV_REDXOR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ROL), std::string("BV_ROL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ROR), std::string("BV_ROR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SADD_OVERFLOW),
            std::string("BV_SADDO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SDIV_OVERFLOW),
            std::string("BV_SDIVO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SDIV), std::string("BV_SDIV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SGE), std::string("BV_SGE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SGT), std::string("BV_SGT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SHL), std::string("BV_SHL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SHR), std::string("BV_SHR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SLE), std::string("BV_SLE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SLT), std::string("BV_SLT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SMOD), std::string("BV_SMOD"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SMUL_OVERFLOW),
            std::string("BV_SMULO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SREM), std::string("BV_SREM"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SSUB_OVERFLOW),
            std::string("BV_SSUBO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SUB), std::string("BV_SUB"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_UADD_OVERFLOW),
            std::string("BV_UADDO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_UDIV), std::string("BV_UDIV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_UGE), std::string("BV_UGE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_UGT), std::string("BV_UGT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ULE), std::string("BV_ULE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ULT), std::string("BV_ULT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_UMUL_OVERFLOW),
            std::string("BV_UMULO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_UREM), std::string("BV_UREM"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_USUB_OVERFLOW),
            std::string("BV_USUBO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_XNOR), std::string("BV_XNOR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_XOR), std::string("BV_XOR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::DISTINCT), std::string("DISTINCT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::EQUAL), std::string("EQUAL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::EXISTS), std::string("EXISTS"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FORALL), std::string("FORALL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_ABS), std::string("FP_ABS"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_ADD), std::string("FP_ADD"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_DIV), std::string("FP_DIV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_EQUAL), std::string("FP_EQUAL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_FMA), std::string("FP_FMA"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_FP), std::string("FP_FP"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_GEQ), std::string("FP_GEQ"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_GT), std::string("FP_GT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_INF),
            std::string("FP_IS_INF"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_NAN),
            std::string("FP_IS_NAN"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_NEG),
            std::string("FP_IS_NEG"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_NORMAL),
            std::string("FP_IS_NORMAL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_POS),
            std::string("FP_IS_POS"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_SUBNORMAL),
            std::string("FP_IS_SUBNORMAL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_IS_ZERO),
            std::string("FP_IS_ZERO"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_LEQ), std::string("FP_LEQ"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_LT), std::string("FP_LT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_MAX), std::string("FP_MAX"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_MIN), std::string("FP_MIN"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_MUL), std::string("FP_MUL"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_NEG), std::string("FP_NEG"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_REM), std::string("FP_REM"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_RTI), std::string("FP_RTI"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_SQRT), std::string("FP_SQRT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_SUB), std::string("FP_SUB"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::IFF), std::string("IFF"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::IMPLIES), std::string("IMPLIES"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::ITE), std::string("ITE"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::LAMBDA), std::string("LAMBDA"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::NOT), std::string("NOT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::OR), std::string("OR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::XOR), std::string("XOR"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_EXTRACT),
            std::string("BV_EXTRACT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_REPEAT),
            std::string("BV_REPEAT"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ROLI), std::string("BV_ROLI"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_RORI), std::string("BV_RORI"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_SIGN_EXTEND),
            std::string("BV_SIGN_EXTEND"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::BV_ZERO_EXTEND),
            std::string("BV_ZERO_EXTEND"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_TO_FP_FROM_BV),
            std::string("FP_TO_FP_FROM_BV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_TO_FP_FROM_FP),
            std::string("FP_TO_FP_FROM_FP"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_TO_FP_FROM_SBV),
            std::string("FP_TO_FP_FROM_SBV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_TO_FP_FROM_UBV),
            std::string("FP_TO_FP_FROM_UBV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_TO_SBV),
            std::string("FP_TO_SBV"));
  ASSERT_EQ(std::to_string(bitwuzla::Kind::FP_TO_UBV),
            std::string("FP_TO_UBV"));
}

/* -------------------------------------------------------------------------- */
/* RoundingMode                                                               */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, rm_to_string)
{
  ASSERT_EQ(std::to_string(bitwuzla::RoundingMode::RNA), std::string("RNA"));
  ASSERT_EQ(std::to_string(bitwuzla::RoundingMode::RNE), std::string("RNE"));
  ASSERT_EQ(std::to_string(bitwuzla::RoundingMode::RTN), std::string("RTN"));
  ASSERT_EQ(std::to_string(bitwuzla::RoundingMode::RTP), std::string("RTP"));
  ASSERT_EQ(std::to_string(bitwuzla::RoundingMode::RTZ), std::string("RTZ"));
}

/* -------------------------------------------------------------------------- */
/* BitwuzlaResult                                                             */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, result_to_string)
{
  ASSERT_EQ(std::to_string(bitwuzla::Result::SAT), std::string("sat"));
  ASSERT_EQ(std::to_string(bitwuzla::Result::UNSAT), std::string("unsat"));
  ASSERT_EQ(std::to_string(bitwuzla::Result::UNKNOWN), std::string("unknown"));
}

/* -------------------------------------------------------------------------- */
/* Bitwuzla                                                                   */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, set_option)
{
  {
    bitwuzla::Options opts;
    opts.set(bitwuzla::Option::INCREMENTAL, true);
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), true);
    opts.set("incremental", "false");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), false);
    opts.set("incremental", "true");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), true);
    opts.set("incremental", "fAlse");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), false);
    opts.set("incremental", "True");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), true);
    opts.set("incremental", "  faLsE  ");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), false);
    opts.set("incremental", "  tRUe   ");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), true);
    opts.set("incremental", "0");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), false);
    opts.set("incremental", "1");
    ASSERT_EQ(opts.get(bitwuzla::Option::INCREMENTAL), true);
    ASSERT_THROW(opts.set("incremental", "asdf"), bitwuzla::BitwuzlaException);
    ASSERT_THROW(opts.set("incremental", "2"), bitwuzla::BitwuzlaException);
    ASSERT_THROW(opts.set("incrremental", "true"), bitwuzla::BitwuzlaException);
    ASSERT_THROW(opts.set(bitwuzla::Option::VERBOSITY, 5),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(opts.set("VERBOSITY", "5"), bitwuzla::BitwuzlaException);
    //  ASSERT_THROW(
    //      opts.set(bitwuzla::Option::PP_UNCONSTRAINED_OPTIMIZATION, true),
    //      BitwuzlaException);
  }
  //{
  //  bitwuzla::Options opts;
  //  opts.set(bitwuzla::Option::FUN_DUAL_PROP, true);
  //  ASSERT_THROW(opts.set(bitwuzla::Option::FUN_JUST, true),
  //               "enabling multiple optimization techniques is not allowed");
  //  ASSERT_THROW(opts.set(bitwuzla::Option::PP_NONDESTR_SUBST, true),
  //  BitwuzlaException);
  //}
  //{
  //  bitwuzla::Options opts;
  //  opts.set(bitwuzla::Option::FUN_JUST, true);
  //  ASSERT_THROW(opts.set(bitwuzla::Option::FUN_DUAL_PROP, true),
  //  BitwuzlaException);
  //}
  {
    bitwuzla::Options opts;
    opts.set(bitwuzla::Option::PRODUCE_MODELS, true);
    // ASSERT_THROW(
    //     opts.set(bitwuzla::Option::PP_UNCONSTRAINED_OPTIMIZATION, true),
    //     BitwuzlaException);
  }
  //{
  //  bitwuzla::Options opts;
  //  opts.set(bitwuzla::Option::PP_NONDESTR_SUBST, true);
  //  ASSERT_THROW(
  //      opts.set(bitwuzla::Option::FUN_DUAL_PROP, true),
  //    BitwuzlaException);
  //}
  //{
  //  bitwuzla::Options opts;
  //  opts.set(bitwuzla::Option::PP_UNCONSTRAINED_OPTIMIZATION, true);
  //  ASSERT_THROW(opts.set(bitwuzla::Option::INCREMENTAL, true),
  //  BitwuzlaException);
  //  ASSERT_THROW(opts.set(bitwuzla::Option::PRODUCE_MODELS, true),
  //  BitwuzlaException);
  //}
  {
    bitwuzla::Options opts;
    ASSERT_EQ(opts.get(bitwuzla::Option::PRODUCE_UNSAT_CORES), 0);
    opts.set(bitwuzla::Option::PRODUCE_UNSAT_CORES, true);
    ASSERT_EQ(opts.get(bitwuzla::Option::PRODUCE_UNSAT_CORES), 1);

    ASSERT_EQ(opts.get(bitwuzla::Option::VERBOSITY), 0);
    opts.set(bitwuzla::Option::VERBOSITY, 2);
    ASSERT_EQ(opts.get(bitwuzla::Option::VERBOSITY), 2);
    opts.set("verbosity", "3");
    ASSERT_EQ(opts.get(bitwuzla::Option::VERBOSITY), 3);
    ASSERT_THROW(opts.set("verbositi", "3"), bitwuzla::BitwuzlaException);

    ASSERT_EQ(opts.get_mode(bitwuzla::Option::BV_SOLVER), "bitblast");
    opts.set(bitwuzla::Option::BV_SOLVER, "prop");
    ASSERT_EQ(opts.get_mode(bitwuzla::Option::BV_SOLVER), "prop");
    opts.set(bitwuzla::Option::BV_SOLVER, "bitblast");
    ASSERT_EQ(opts.get_mode(bitwuzla::Option::BV_SOLVER), "bitblast");
    opts.set(bitwuzla::Option::SAT_SOLVER, "cadical");
    ASSERT_EQ(opts.get_mode(bitwuzla::Option::SAT_SOLVER), "cadical");
    opts.set("sat-solver", "kissat");
    ASSERT_EQ(opts.get_mode(bitwuzla::Option::SAT_SOLVER), "kissat");
    ASSERT_THROW(opts.set("sat--solver", "kissat"),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(opts.set(bitwuzla::Option::BV_SOLVER, "asdf"),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(opts.set(bitwuzla::Option::INCREMENTAL, "true"),
                 bitwuzla::BitwuzlaException);
  }
}

TEST_F(TestApi, option_info)
{
  for (int32_t i = 0; i < static_cast<int32_t>(bitwuzla::Option::NUM_OPTS); ++i)
  {
    bitwuzla::Option opt = static_cast<bitwuzla::Option>(i);
    try  // TODO: temporary, until set of options is finalized
    {
      bitwuzla::Options options;
      bitwuzla::OptionInfo info(options, opt);
      if (info.kind == bitwuzla::OptionInfo::Kind::BOOL)
      {
        ASSERT_EQ(options.get(opt),
                  std::get<bitwuzla::OptionInfo::Bool>(info.values).cur);
      }
      else if (info.kind == bitwuzla::OptionInfo::Kind::NUMERIC)
      {
        ASSERT_EQ(options.get(opt),
                  std::get<bitwuzla::OptionInfo::Numeric>(info.values).cur);
      }
      else
      {
        ASSERT_EQ(options.get_mode(opt),
                  std::get<bitwuzla::OptionInfo::Mode>(info.values).cur);
      }
    }
    catch (...)
    {
    }
  }
}

TEST_F(TestApi, mk_array_sort)
{
  ASSERT_THROW(bitwuzla::mk_array_sort(bitwuzla::Sort(), d_bv_sort8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_array_sort(d_bv_sort1, bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_array_sort(d_arr_sort_bv, d_bv_sort8),
               bitwuzla::BitwuzlaException);

  ASSERT_NO_THROW(bitwuzla::mk_array_sort(d_bv_sort8, d_arr_sort_bv));
  ASSERT_NO_THROW(bitwuzla::mk_array_sort(d_fun_sort, d_bv_sort8));
  ASSERT_NO_THROW(bitwuzla::mk_array_sort(d_bv_sort8, d_fun_sort));
}

TEST_F(TestApi, mk_bv_sort)
{
  ASSERT_THROW(bitwuzla::mk_bv_sort(0), bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_sort)
{
  ASSERT_THROW(bitwuzla::mk_fp_sort(0, 8), bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_sort(5, 0), bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_sort(1, 2), bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_sort(2, 1), bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fun_sort)
{
  ASSERT_THROW(bitwuzla::mk_fun_sort({}, d_bv_sort8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fun_sort(d_fun_domain_sorts, bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_zero)
{
  ASSERT_THROW(bitwuzla::mk_bv_zero(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_zero(d_fp_sort16), bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_one)
{
  ASSERT_THROW(bitwuzla::mk_bv_one(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_one(d_fp_sort16), bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_ones)
{
  ASSERT_THROW(bitwuzla::mk_bv_ones(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_ones(d_fp_sort16), bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_min_signed)
{
  ASSERT_THROW(bitwuzla::mk_bv_min_signed(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_min_signed(d_fp_sort16),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_max_signed)
{
  ASSERT_THROW(bitwuzla::mk_bv_max_signed(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_max_signed(d_fp_sort16),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_pos_zero)
{
  ASSERT_THROW(bitwuzla::mk_fp_pos_zero(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_pos_zero(d_bv_sort8),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_neg_zero)
{
  ASSERT_THROW(bitwuzla::mk_fp_neg_zero(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_neg_zero(d_bv_sort8),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_pos_inf)
{
  ASSERT_THROW(bitwuzla::mk_fp_pos_inf(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_pos_inf(d_bv_sort8),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_neg_inf)
{
  ASSERT_THROW(bitwuzla::mk_fp_neg_inf(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_neg_inf(d_bv_sort8),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_nan)
{
  ASSERT_THROW(bitwuzla::mk_fp_nan(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_nan(d_bv_sort8), bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_value)
{
  ASSERT_NO_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "127", 10));
  ASSERT_NO_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "-128", 10));
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "256", 10),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "-129", 10),
               bitwuzla::BitwuzlaException);

  ASSERT_THROW(bitwuzla::mk_bv_value(bitwuzla::Sort(), "010", 2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "", 2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "", 2),
               bitwuzla::BitwuzlaException);

  ASSERT_THROW(bitwuzla::mk_bv_value(d_fp_sort16, "010", 2),
               bitwuzla::BitwuzlaException);

  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "11111111010", 2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "1234567890", 10),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "1234567890aAbBcCdDeEfF", 16),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "1234567890", 2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "12z4567890", 10),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value(d_bv_sort8, "12z4567890", 16),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_bv_value_uint64)
{
  ASSERT_THROW(bitwuzla::mk_bv_value_uint64(bitwuzla::Sort(), 23),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_bv_value_uint64(d_fp_sort16, 23),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_fp_value)
{
  ASSERT_THROW(bitwuzla::mk_fp_value(bitwuzla::Term(), d_bv_zero8, d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_one1, bitwuzla::Term(), d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_one1, d_bv_zero8, bitwuzla::Term()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_zero8, d_bv_zero8, d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_fp_const16, d_bv_zero8, d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_one1, d_fp_const16, d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_one1, d_bv_zero8, d_fp_const16),
               bitwuzla::BitwuzlaException);

  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_const1, d_bv_zero8, d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_one1, d_bv_const8, d_bv_zero8),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_fp_value(d_bv_one1, d_bv_zero8, d_bv_const8),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_term_check_null)
{
  bitwuzla::Term null;
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NOT, {null}),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_AND, {d_bv_zero8, null}),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ADD,
                                 {d_rm_const, null, d_fp_const16}),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_term_check_cnt)
{
  std::vector<bitwuzla::Term> apply_args1 = {d_bv_one1};
  std::vector<bitwuzla::Term> apply_args2 = {d_fun, d_bv_const8};
  std::vector<bitwuzla::Term> array_args1 = {d_array_fpbv};
  std::vector<bitwuzla::Term> bool_args1  = {d_true};
  std::vector<bitwuzla::Term> bool_args2  = {d_true, d_true};
  std::vector<bitwuzla::Term> bv_args1    = {d_bv_one1};
  std::vector<bitwuzla::Term> bv_args1_rm = {d_rm_const};
  std::vector<bitwuzla::Term> bv_args2    = {d_bv_zero8, d_bv_const8};
  std::vector<bitwuzla::Term> ite_args2   = {d_true, d_bv_const8};
  std::vector<bitwuzla::Term> fp_args1    = {d_fp_const16};
  std::vector<bitwuzla::Term> fp_args1_rm = {d_rm_const};
  std::vector<bitwuzla::Term> fp_args2    = {d_fp_const16, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args2_rm = {d_rm_const, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args3_rm = {
      d_rm_const, d_fp_const16, d_fp_const16};
  std::vector<bitwuzla::Term> fun_args1 = {d_var1};

  std::vector<uint64_t> idxs1    = {1};
  std::vector<uint64_t> idxs2    = {2, 0};
  std::vector<uint64_t> fp_idxs1 = {5, 8};

  // bool
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::AND, bool_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::IFF, bool_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::IMPLIES, bool_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::NOT, bool_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::OR, bool_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::XOR, bool_args1),
               bitwuzla::BitwuzlaException);

  // bit-vectors
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::APPLY, apply_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::APPLY, apply_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_SELECT, array_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_STORE, array_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ADD, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_AND, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ASHR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_CONCAT, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_DEC, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_INC, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_MUL, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NAND, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NEG, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NOR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NOT, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_OR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REDAND, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REDOR, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REDXOR, bv_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_OR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROL, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SADD_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SGE, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SGT, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SHL, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SHR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SLE, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SLT, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SMOD, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SMUL_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SREM, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SSUB_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SUB, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UADD_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UDIV, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UGE, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UGT, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ULE, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ULT, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UMUL_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UREM, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_USUB_OVERFLOW, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_XNOR, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_XOR, bv_args1),
               bitwuzla::BitwuzlaException);

  // floating-point
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ABS, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ADD, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_DIV, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_EQUAL, fp_args1_rm),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FMA, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FP, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_GEQ, fp_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_GT, fp_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_INF, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_NAN, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_NEG, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_NORMAL, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_POS, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_SUBNORMAL, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_ZERO, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_LEQ, fp_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_LT, fp_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MAX, fp_args3_rm),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MIN, fp_args3_rm),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_ZERO, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MUL, fp_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_REM, fp_args3_rm),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_RTI, fp_args3_rm),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SQRT, fp_args3_rm),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SUB, fp_args2),
               bitwuzla::BitwuzlaException);

  // others
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::DISTINCT, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EQUAL, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EXISTS, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FORALL, bv_args1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ITE, ite_args2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::LAMBDA, fun_args1),
               bitwuzla::BitwuzlaException);

  // indexed
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_EXTRACT, bv_args2, idxs2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REPEAT, bv_args2, idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROLI, bv_args2, idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_RORI, bv_args2, idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SIGN_EXTEND, bv_args2, idxs1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_ZERO_EXTEND, bv_args2, idxs1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::FP_TO_FP_FROM_BV, bv_args2, fp_idxs1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_FP_FROM_FP, fp_args3_rm, fp_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_FP_FROM_SBV, bv_args1_rm, fp_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_FP_FROM_UBV, bv_args1_rm, fp_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_TO_SBV, fp_args1, idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_TO_UBV, fp_args1, idxs1),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_term_check_args)
{
  std::vector<bitwuzla::Term> array_select_args2_invalid_1 = {d_fp_const16,
                                                              d_array_fpbv};
  std::vector<bitwuzla::Term> array_select_args2_invalid_2 = {d_array_fpbv,
                                                              d_bv_const8};

  std::vector<bitwuzla::Term> array_store_args3_invalid_1 = {
      d_fp_const16, d_array_fpbv, d_bv_const8};
  std::vector<bitwuzla::Term> array_store_args3_invalid_2 = {
      d_array_fpbv, d_bv_const8, d_bv_const8};
  std::vector<bitwuzla::Term> array_store_args3_invalid_3 = {
      d_array_fpbv, d_fp_const16, d_fp_const16};

  std::vector<bitwuzla::Term> apply_args3_invalid_1 = {
      d_fun, d_bv_const8, d_fun};
  std::vector<bitwuzla::Term> apply_args3_invalid_2 = {
      d_fun, d_bv_const8, d_bv_const8, d_fp_pzero32};

  std::vector<bitwuzla::Term> bool_args1_invalid = {d_bv_const8};
  std::vector<bitwuzla::Term> bool_args2_invalid = {d_fp_pzero32, d_fp_pzero32};
  std::vector<bitwuzla::Term> bool_args2_mis     = {d_true, d_bv_const8};

  std::vector<bitwuzla::Term> bv_args1         = {d_bv_const8};
  std::vector<bitwuzla::Term> bv_args1_invalid = {d_fp_const16};
  std::vector<bitwuzla::Term> bv_args2_invalid = {d_fp_const16, d_fp_const16};
  std::vector<bitwuzla::Term> bv_args2_mis     = {d_bv_one1, d_bv_const8};
  std::vector<bitwuzla::Term> bv_args2_rm      = {d_rm_const, d_bv_const8};
  std::vector<bitwuzla::Term> bv_args2_rm_invalid_1 = {d_bv_const8,
                                                       d_bv_const8};
  std::vector<bitwuzla::Term> bv_args2_rm_invalid_2 = {d_rm_const,
                                                       d_fp_const16};

  std::vector<bitwuzla::Term> ite_THROW_args3_1 = {
      d_true, d_bv_const8, d_bv_one1};
  std::vector<bitwuzla::Term> ite_args3_invalid_2 = {
      d_bv_const8, d_bv_const8, d_bv_const8};

  std::vector<bitwuzla::Term> lambda_args2_invalid_1 = {d_bv_const8,
                                                        d_bv_const8};
  std::vector<bitwuzla::Term> lambda_args2 = {d_bound_var, d_bv_const8};
  std::vector<bitwuzla::Term> lambda_args3_invalid_1 = {
      d_var1, d_var1, d_bv_const8};

  std::vector<bitwuzla::Term> lambda_args3 = {
      d_var1,
      d_var2,
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_UBV,
          {d_rm_const,
           bitwuzla::mk_term(bitwuzla::Kind::BV_ADD, {d_var2, d_bv_const8})},
          {5, 8})};

  std::vector<bitwuzla::Term> fp_args2_rm = {d_rm_const, d_fp_const16};

  std::vector<bitwuzla::Term> fp_args1_invalid = {d_bv_one1};
  std::vector<bitwuzla::Term> fp_args2_invalid = {d_bv_zero8, d_bv_const8};
  std::vector<bitwuzla::Term> fp_args2_mis     = {d_fp_pzero32, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args2_rm_invalid_1 = {d_bv_const8,
                                                       d_fp_const16};
  std::vector<bitwuzla::Term> fp_args2_rm_invalid_2 = {d_rm_const, d_bv_const8};
  std::vector<bitwuzla::Term> fp_args3_rm_mis       = {
            d_rm_const, d_fp_pzero32, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args3_rm_invalid_1 = {
      d_fp_const16, d_fp_const16, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args3_rm_invalid_2 = {
      d_rm_const, d_bv_zero8, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args4_mis = {
      d_rm_const, d_fp_pzero32, d_fp_const16, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args4_rm_invalid_1 = {
      d_rm_const, d_bv_zero8, d_fp_const16, d_fp_const16};
  std::vector<bitwuzla::Term> fp_args4_rm_invalid_2 = {
      d_fp_const16, d_fp_const16, d_fp_const16, d_fp_const16};

  std::vector<bitwuzla::Term> fp_fp_args3_invalid_1 = {
      d_bv_const8, d_bv_zero8, d_bv_ones23};
  std::vector<bitwuzla::Term> fp_fp_args3_invalid_2 = {
      d_bv_one1, d_fp_pzero32, d_bv_ones23};
  std::vector<bitwuzla::Term> fp_fp_args3_invalid_3 = {
      d_bv_one1, d_bv_zero8, d_fp_pzero32};
  std::vector<bitwuzla::Term> fp_fp_args3_invalid_4 = {
      d_fp_pzero32, d_bv_zero8, d_bv_ones23};

  std::vector<bitwuzla::Term> quant_args2_invalid_1 = {d_true, d_true};
  std::vector<bitwuzla::Term> quant_args2_invalid_2 = {d_var1, d_bv_const8};
  std::vector<bitwuzla::Term> quant_args2_invalid_3 = {d_bound_var,
                                                       d_bv_const8};
  std::vector<bitwuzla::Term> quant_args3_invalid   = {
        d_var1, d_var1, d_bv_const8};

  std::vector<uint64_t> bv_idxs1                   = {3};
  std::vector<uint64_t> bv_idxs2                   = {2, 0};
  std::vector<uint64_t> bv_extract_idxs2_invalid_1 = {0, 2};
  std::vector<uint64_t> bv_extract_idxs2_invalid_2 = {9, 0};
  std::vector<uint64_t> bv_repeat_idxs_invalid_1   = {2305843009213693953};
  std::vector<uint64_t> bv_extend_idxs_invalid_1   = {UINT64_MAX};
  std::vector<uint64_t> fp_idxs2                   = {5, 8};
  std::vector<uint64_t> fp_idxs2_invalid_1         = {1, 8};
  std::vector<uint64_t> fp_idxs2_invalid_2         = {5, 1};

  // bool
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::AND, bool_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::AND, bool_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::IFF, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::IFF, bool_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::IMPLIES, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::IMPLIES, bool_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::NOT, bool_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::OR, bool_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::XOR, bool_args2_invalid),
               bitwuzla::BitwuzlaException);
  // bit-vectors
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ADD, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ADD, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_AND, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_AND, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ASHR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ASHR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_DEC, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_INC, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_MUL, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_MUL, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NAND, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NAND, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NEG, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NOR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NOR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_NOT, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_OR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_OR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REDAND, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REDOR, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_REDXOR, bv_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_OR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_OR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROL, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROL, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ROR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SADD_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SADD_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SGE, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SGE, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SGT, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SGT, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SHL, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SHL, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SHR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SHR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SLE, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SLE, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SLT, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SLT, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SMOD, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SMOD, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SMUL_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SMUL_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SREM, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SREM, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SSUB_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_SSUB_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SUB, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_SUB, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_UADD_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_UADD_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UDIV, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UDIV, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UGE, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UGE, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UGT, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UGT, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ULE, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ULE, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ULT, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_ULT, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_UMUL_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_UMUL_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UREM, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_UREM, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_USUB_OVERFLOW, bv_args2_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_USUB_OVERFLOW, bv_args2_mis),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_XNOR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_XNOR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_XOR, bv_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::BV_XOR, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  // floating-point
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ABS, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ADD, fp_args3_rm_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ADD, fp_args3_rm_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_ADD, fp_args3_rm_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_DIV, fp_args3_rm_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_DIV, fp_args3_rm_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_DIV, fp_args3_rm_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_EQUAL, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_EQUAL, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FMA, fp_args4_rm_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FMA, fp_args4_rm_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FMA, fp_args4_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FP, fp_fp_args3_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FP, fp_fp_args3_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FP, fp_fp_args3_invalid_3),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_FP, fp_fp_args3_invalid_4),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_GEQ, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_GEQ, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_GT, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_GT, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_INF, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_NAN, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_NEG, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::FP_IS_NORMAL, fp_args1_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_POS, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::FP_IS_SUBNORMAL, fp_args1_invalid),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_ZERO, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_LEQ, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_LEQ, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_LT, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_LT, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MAX, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MAX, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MIN, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MIN, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_IS_ZERO, fp_args1_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MUL, fp_args3_rm_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MUL, fp_args3_rm_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_MUL, fp_args3_rm_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_REM, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_REM, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_RTI, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_RTI, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_RTI, fp_args2_rm_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_RTI, fp_args2_rm_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SQRT, fp_args2_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SQRT, fp_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::FP_SQRT, fp_args2_rm_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::FP_SQRT, fp_args2_rm_invalid_2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SUB, fp_args3_rm_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SUB, fp_args3_rm_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FP_SUB, fp_args3_rm_mis),
               bitwuzla::BitwuzlaException);
  // others
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::APPLY, apply_args3_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::APPLY, apply_args3_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_SELECT,
                                 array_select_args2_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_SELECT,
                                 array_select_args2_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_STORE,
                                 array_store_args3_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_STORE,
                                 array_store_args3_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ARRAY_STORE,
                                 array_store_args3_invalid_3),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::DISTINCT, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EQUAL, bv_args2_mis),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EXISTS, quant_args2_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EXISTS, quant_args2_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EXISTS, quant_args2_invalid_3),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::EXISTS, quant_args3_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FORALL, quant_args2_invalid_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FORALL, quant_args2_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FORALL, quant_args2_invalid_3),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::FORALL, quant_args3_invalid),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ITE, ite_args3_invalid_2),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::ITE, ite_THROW_args3_1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::LAMBDA, lambda_args2_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::LAMBDA, lambda_args3_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_NO_THROW(bitwuzla::mk_term(bitwuzla::Kind::LAMBDA, lambda_args3));
  ASSERT_NO_THROW(bitwuzla::mk_term(bitwuzla::Kind::LAMBDA, lambda_args2));
  // indexed
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_EXTRACT, bv_args1_invalid, bv_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::BV_EXTRACT, bv_args1, bv_extract_idxs2_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::BV_EXTRACT, bv_args1, bv_extract_idxs2_invalid_2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_REPEAT, bv_args1_invalid, bv_idxs1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::BV_REPEAT, bv_args1, bv_repeat_idxs_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_ROLI, bv_args1_invalid, bv_idxs1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(bitwuzla::Kind::BV_RORI, bv_args1_invalid, bv_idxs1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::BV_SIGN_EXTEND, bv_args1_invalid, bv_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::BV_SIGN_EXTEND, bv_args1, bv_extend_idxs_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::BV_ZERO_EXTEND, bv_args1_invalid, bv_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::BV_ZERO_EXTEND, bv_args1, bv_extend_idxs_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_BV, bv_args1_invalid, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_BV, bv_args1, fp_idxs2_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_BV, bv_args1, fp_idxs2_invalid_2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_FP, fp_args2_rm_invalid_1, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_FP, fp_args2_rm_invalid_2, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_FP, fp_args2_rm, fp_idxs2_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_FP, fp_args2_rm, fp_idxs2_invalid_2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_SBV, bv_args2_rm_invalid_1, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_SBV, bv_args2_rm_invalid_2, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_SBV, bv_args2_rm, fp_idxs2_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_SBV, bv_args2_rm, fp_idxs2_invalid_2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_UBV, bv_args2_rm_invalid_1, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_UBV, bv_args2_rm_invalid_2, fp_idxs2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_UBV, bv_args2_rm, fp_idxs2_invalid_1),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(
      bitwuzla::mk_term(
          bitwuzla::Kind::FP_TO_FP_FROM_UBV, bv_args2_rm, fp_idxs2_invalid_2),
      bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_SBV, fp_args2_rm_invalid_1, bv_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_SBV, fp_args2_rm_invalid_2, bv_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_UBV, fp_args2_rm_invalid_1, bv_idxs1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_term(
                   bitwuzla::Kind::FP_TO_UBV, fp_args2_rm_invalid_2, bv_idxs1),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_const)
{
  ASSERT_NO_THROW(bitwuzla::mk_const(d_bv_sort8));
  ASSERT_NO_THROW(bitwuzla::mk_const(d_bv_sort8, ""));
  ASSERT_THROW(bitwuzla::mk_const(bitwuzla::Sort(), "asdf"),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, mk_const_array)
{
  GTEST_SKIP();  // TODO enable when implemented
  ASSERT_THROW(bitwuzla::mk_const_array(bitwuzla::Sort(), d_bv_one1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_const_array(d_arr_sort_bv, bitwuzla::Term()),
               bitwuzla::BitwuzlaException);

  ASSERT_THROW(bitwuzla::mk_const_array(d_bv_sort8, d_bv_one1),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_const_array(d_arr_sort_bv, d_array),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::mk_const_array(d_arr_sort_bvfp, d_fp_pzero32),
               bitwuzla::BitwuzlaException);

  ASSERT_NO_THROW(bitwuzla::mk_const_array(d_arr_sort_bvfp, d_fp_const16));
}

TEST_F(TestApi, mk_var)
{
  ASSERT_NO_THROW(bitwuzla::mk_var(d_bv_sort8));
  ASSERT_NO_THROW(bitwuzla::mk_var(d_bv_sort8, ""));
  ASSERT_THROW(bitwuzla::mk_var(bitwuzla::Sort(), "asdf"),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, push)
{
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, false);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.push(2), bitwuzla::BitwuzlaException);
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_NO_THROW(bitwuzla.push(0));
    ASSERT_NO_THROW(bitwuzla.push(2));
  }
}

TEST_F(TestApi, pop)
{
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, false);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.pop(0), bitwuzla::BitwuzlaException);
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.pop(2), bitwuzla::BitwuzlaException);
    ASSERT_NO_THROW(bitwuzla.pop(0));
    bitwuzla.push(2);
    ASSERT_NO_THROW(bitwuzla.pop(2));
  }
}

TEST_F(TestApi, assert_formula)
{
  bitwuzla::Options options;
  bitwuzla::Bitwuzla bitwuzla(options);
  ASSERT_THROW(bitwuzla.assert_formula(bitwuzla::Term()),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla.assert_formula(d_bv_const8),
               bitwuzla::BitwuzlaException);

  ASSERT_THROW(bitwuzla.assert_formula(d_bv_one1), bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla.assert_formula(d_bool_var),
               bitwuzla::BitwuzlaException);
  // TODO: this should throw, not implemented yet
  // ASSERT_THROW(bitwuzla.assert_formula(d_bool_lambda),
  // bitwuzla::BitwuzlaException);
  // ASSERT_THROW(bitwuzla.assert_formula(d_bool_lambda_body),
  // bitwuzla::BitwuzlaException);

  ASSERT_NO_THROW(bitwuzla.assert_formula(d_bool_apply));
  ASSERT_NO_THROW(bitwuzla.assert_formula(d_bool_const));
}

TEST_F(TestApi, is_unsat_assumption)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_bool_const),
                 bitwuzla::BitwuzlaException);
  }
  {
    GTEST_SKIP();  // TODO enable when implemented
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    bitwuzla::Bitwuzla bitwuzla(options);

    ASSERT_THROW(bitwuzla.is_unsat_assumption(bitwuzla::Term()),
                 bitwuzla::BitwuzlaException);

    bitwuzla.assert_formula(d_true);
    bitwuzla.check_sat({d_bool_const});
    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_bool_const),
                 bitwuzla::BitwuzlaException);

    bitwuzla.check_sat(
        {d_bool_const,
         bitwuzla::mk_term(bitwuzla::Kind::BV_NOT, {d_bool_const})});

    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_bv_const8),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_true),
                 bitwuzla::BitwuzlaException);

    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_bool_var),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_bool_lambda),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(bitwuzla.is_unsat_assumption(d_bool_lambda_body),
                 bitwuzla::BitwuzlaException);

    ASSERT_NO_THROW(bitwuzla.is_unsat_assumption(d_bool_const));
  }
}

TEST_F(TestApi, get_unsat_assumptions)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_unsat_assumptions(), bitwuzla::BitwuzlaException);
  }
  {
    GTEST_SKIP();  // TODO enable when implemented
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    bitwuzla::Bitwuzla bitwuzla(options);

    bitwuzla.assert_formula(d_true);
    bitwuzla.check_sat({d_bool_const});
    ASSERT_THROW(bitwuzla.get_unsat_assumptions(), bitwuzla::BitwuzlaException);

    bitwuzla.check_sat(
        {d_bool_const, d_not_bv_const1, d_and_bv_const1, d_eq_bv_const8});
    ASSERT_TRUE(bitwuzla.is_unsat_assumption(d_bool_const));
    ASSERT_TRUE(bitwuzla.is_unsat_assumption(d_not_bv_const1));
    ASSERT_TRUE(bitwuzla.is_unsat_assumption(d_and_bv_const1));
    ASSERT_FALSE(bitwuzla.is_unsat_assumption(d_eq_bv_const8));
    auto unsat_ass = bitwuzla.get_unsat_assumptions();
    ASSERT_EQ(unsat_ass.size(), 3);
    for (const auto& a : unsat_ass)
    {
      ASSERT_TRUE(bitwuzla.is_unsat_assumption(a));
      bitwuzla.assert_formula(a);
    }
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::UNSAT);
  }
}

TEST_F(TestApi, get_unsat_core)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_unsat_core(), bitwuzla::BitwuzlaException);
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_unsat_core(), bitwuzla::BitwuzlaException);
  }
  {
    GTEST_SKIP();  // TODO enable when implemented
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    options.set(bitwuzla::Option::PRODUCE_UNSAT_CORES, true);
    bitwuzla::Bitwuzla bitwuzla(options);

    bitwuzla.assert_formula(d_true);
    bitwuzla.check_sat({d_bv_const1});
    ASSERT_THROW(bitwuzla.get_unsat_core(), bitwuzla::BitwuzlaException);

    bitwuzla.check_sat(
        {d_bv_const1, d_not_bv_const1, d_and_bv_const1, d_eq_bv_const8});

    ASSERT_TRUE(bitwuzla.is_unsat_assumption(d_bv_const1));
    ASSERT_TRUE(bitwuzla.is_unsat_assumption(d_and_bv_const1));
    auto unsat_core = bitwuzla.get_unsat_core();
    ASSERT_EQ(unsat_core.size(), 1);
    ASSERT_EQ(unsat_core[0], d_not_bv_const1);

    auto unsat_ass = bitwuzla.get_unsat_assumptions();
    ASSERT_EQ(unsat_ass.size(), 2);
    ASSERT_EQ(unsat_ass[0], d_bv_const1);
    ASSERT_EQ(unsat_ass[1], d_and_bv_const1);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::SAT);
    bitwuzla.assert_formula(unsat_ass[0]);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::UNSAT);
  }
}

TEST_F(TestApi, simplify)
{
  GTEST_SKIP();  // TODO enable when implemented
  bitwuzla::Options options;
  bitwuzla::Bitwuzla bitwuzla(options);
  bitwuzla.assert_formula(d_bool_const);
  bitwuzla.assert_formula(d_and_bv_const1);
  ASSERT_EQ(bitwuzla.simplify(), bitwuzla::Result::SAT);
}

TEST_F(TestApi, check_sat)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_NO_THROW(bitwuzla.check_sat());
    ASSERT_THROW(bitwuzla.check_sat(), bitwuzla::BitwuzlaException);
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_NO_THROW(bitwuzla.check_sat());
    ASSERT_NO_THROW(bitwuzla.check_sat());
  }
}

TEST_F(TestApi, get_value)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_value(d_bv_const8), bitwuzla::BitwuzlaException);
  }
  GTEST_SKIP();  // TODO enable when implemented
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::INCREMENTAL, true);
    options.set(bitwuzla::Option::PRODUCE_MODELS, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_value(bitwuzla::Term()),
                 bitwuzla::BitwuzlaException);
    bitwuzla.assert_formula(d_bool_const);
    ASSERT_EQ(bitwuzla.check_sat({d_not_bv_const1}), bitwuzla::Result::UNSAT);
    ASSERT_THROW(bitwuzla.get_value(d_bool_const), bitwuzla::BitwuzlaException);
    bitwuzla.check_sat();
    ASSERT_NO_THROW(bitwuzla.get_value(d_bool_const));
    ASSERT_NO_THROW(bitwuzla.get_value(d_not_bv_const1));
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::PRODUCE_MODELS, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    bitwuzla.assert_formula(d_exists);
    ASSERT_THROW(bitwuzla.get_value(d_bv_const8), bitwuzla::BitwuzlaException);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::SAT);
    ASSERT_THROW(bitwuzla.get_value(d_bv_const8), bitwuzla::BitwuzlaException);
  }
}

TEST_F(TestApi, get_bv_value)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_bv_value(d_bv_one1), bitwuzla::BitwuzlaException);
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::PRODUCE_MODELS, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_bv_value(d_bv_zero8),
                 bitwuzla::BitwuzlaException);

    bitwuzla::Result res = bitwuzla.check_sat();
    ASSERT_EQ(res, bitwuzla::Result::SAT);
    ASSERT_THROW(bitwuzla.get_bv_value(bitwuzla::Term()),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(bitwuzla.get_bv_value(d_fp_nan32),
                 bitwuzla::BitwuzlaException);
    ASSERT_THROW(bitwuzla.get_bv_value(d_bv_zero8, 6),
                 bitwuzla::BitwuzlaException);
    ASSERT_EQ("1", bitwuzla.get_bv_value(d_bv_one1));

    bitwuzla::Term bv_maxs32 = bitwuzla::mk_bv_max_signed(d_bv_sort32);
    ASSERT_EQ("01111111111111111111111111111111",
              bitwuzla.get_bv_value(bv_maxs32));
    ASSERT_EQ("2147483647", bitwuzla.get_bv_value(bv_maxs32, 10));
    ASSERT_EQ("7fffffff", bitwuzla.get_bv_value(bv_maxs32, 16));
    ASSERT_EQ(
        bitwuzla.get_bv_value(bitwuzla::mk_bv_value(d_bv_sort8, "-1", 10)),
        "11111111");
    ASSERT_EQ(
        bitwuzla.get_bv_value(bitwuzla::mk_bv_value(d_bv_sort8, "-1", 10), 10),
        "255");
    ASSERT_EQ(
        bitwuzla.get_bv_value(bitwuzla::mk_bv_value(d_bv_sort8, "-1", 10), 16),
        "ff");
    ASSERT_EQ(
        bitwuzla.get_bv_value(bitwuzla::mk_bv_value(d_bv_sort8, "-123", 10)),
        "10000101");
    ASSERT_EQ(bitwuzla.get_bv_value(
                  bitwuzla::mk_bv_value(d_bv_sort8, "-123", 10), 10),
              "133");
    ASSERT_EQ(bitwuzla.get_bv_value(
                  bitwuzla::mk_bv_value(d_bv_sort8, "-123", 10), 16),
              "85");
    ASSERT_EQ(
        bitwuzla.get_bv_value(bitwuzla::mk_bv_value(d_bv_sort8, "-128", 10)),
        "10000000");
    ASSERT_EQ(bitwuzla.get_bv_value(
                  bitwuzla::mk_bv_value(d_bv_sort8, "-128", 10), 10),
              "128");
    ASSERT_EQ(bitwuzla.get_bv_value(
                  bitwuzla::mk_bv_value(d_bv_sort8, "-128", 10), 16),
              "80");
  }
}

TEST_F(TestApi, get_rm_value)
{
  {
    bitwuzla::Options options;
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_bv_value(d_rm_const),
                 bitwuzla::BitwuzlaException);
  }
  {
    bitwuzla::Options options;
    options.set(bitwuzla::Option::PRODUCE_MODELS, true);
    bitwuzla::Bitwuzla bitwuzla(options);
    ASSERT_THROW(bitwuzla.get_rm_value(bitwuzla::Term()),
                 bitwuzla::BitwuzlaException);
    bitwuzla.check_sat();
    ASSERT_THROW(bitwuzla.get_rm_value(d_fp_nan32),
                 bitwuzla::BitwuzlaException);
    ASSERT_EQ(bitwuzla::RoundingMode::RNA, bitwuzla.get_rm_value(d_rm_rna));
    ASSERT_EQ(bitwuzla::RoundingMode::RNE, bitwuzla.get_rm_value(d_rm_rne));
    ASSERT_EQ(bitwuzla::RoundingMode::RTN, bitwuzla.get_rm_value(d_rm_rtn));
    ASSERT_EQ(bitwuzla::RoundingMode::RTP, bitwuzla.get_rm_value(d_rm_rtp));
    ASSERT_EQ(bitwuzla::RoundingMode::RTZ, bitwuzla.get_rm_value(d_rm_rtz));
  }
}

TEST_F(TestApi, dump_formula1)
{
  bitwuzla::Options options;
  bitwuzla::Bitwuzla bitwuzla(options);

  ASSERT_THROW(bitwuzla.dump_formula(std::cout, ""),
               bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla.dump_formula(std::cout, "asdf"),
               bitwuzla::BitwuzlaException);

  GTEST_SKIP();  // TODO enable when implemented
  bitwuzla.assert_formula(d_bool_const);
  bitwuzla.assert_formula(bitwuzla::mk_term(
      bitwuzla::Kind::EQUAL,
      {bitwuzla::mk_term(bitwuzla::Kind::APPLY, {d_lambda, d_bv_const8}),
       d_bv_zero8}));

  ASSERT_NO_THROW(bitwuzla.dump_formula(std::cout, "btor"));
  ASSERT_NO_THROW(bitwuzla.dump_formula(std::cout, "smt2"));

  bitwuzla.assert_formula(d_exists);
  ASSERT_NO_THROW(bitwuzla.dump_formula(std::cout, "btor"));
  ASSERT_NO_THROW(bitwuzla.dump_formula(std::cout, "smt2"));
  // TODO test incremental
}

/* -------------------------------------------------------------------------- */
/* Sort                                                                       */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, sort_hash)
{
  ASSERT_THROW(std::hash<bitwuzla::Sort>{}(bitwuzla::Sort()),
               bitwuzla::BitwuzlaException);
  ASSERT_NO_THROW(std::hash<bitwuzla::Sort>{}(d_bv_sort8));
}

TEST_F(TestApi, sort_bv_size)
{
  ASSERT_THROW(bitwuzla::Sort().bv_size(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_fp_sort16.bv_size(), bitwuzla::BitwuzlaException);
  ASSERT_EQ(d_bv_sort8.bv_size(), 8);
}

TEST_F(TestApi, sort_fp_exp_size)
{
  ASSERT_THROW(bitwuzla::Sort().fp_exp_size(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort8.fp_exp_size(), bitwuzla::BitwuzlaException);
  ASSERT_EQ(d_fp_sort16.fp_exp_size(), 5);
}

TEST_F(TestApi, sort_fp_sig_size)
{
  ASSERT_THROW(bitwuzla::Sort().fp_sig_size(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort8.fp_sig_size(), bitwuzla::BitwuzlaException);
  ASSERT_EQ(d_fp_sort16.fp_sig_size(), 11);
}

TEST_F(TestApi, sort_array_index)
{
  ASSERT_THROW(bitwuzla::Sort().array_index(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort23.array_index(), bitwuzla::BitwuzlaException);
  ASSERT_TRUE(d_arr_sort_bvfp.array_index().is_bv());
}

TEST_F(TestApi, sort_array_element)
{
  ASSERT_THROW(bitwuzla::Sort().array_element(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort23.array_element(), bitwuzla::BitwuzlaException);
  ASSERT_TRUE(d_arr_sort_bvfp.array_element().is_fp());
}

TEST_F(TestApi, sort_fun_domain_sorts)
{
  ASSERT_THROW(bitwuzla::Sort().fun_domain(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort8.fun_domain(), bitwuzla::BitwuzlaException);
  auto domain_sorts = d_fun_sort.fun_domain();
  ASSERT_EQ(domain_sorts.size(), 3);
  ASSERT_EQ(d_bv_sort8, domain_sorts[0]);
  ASSERT_EQ(d_fp_sort16, domain_sorts[1]);
  ASSERT_EQ(d_bv_sort32, domain_sorts[2]);
}

TEST_F(TestApi, sort_fun_codomain)
{
  ASSERT_THROW(bitwuzla::Sort().fun_codomain(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort8.fun_codomain(), bitwuzla::BitwuzlaException);
  ASSERT_EQ(d_fun_sort.fun_codomain(), d_bv_sort8);
}

TEST_F(TestApi, sort_fun_arity)
{
  ASSERT_THROW(bitwuzla::Sort().fun_arity(), bitwuzla::BitwuzlaException);
  ASSERT_THROW(d_bv_sort8.fun_arity(), bitwuzla::BitwuzlaException);
  ASSERT_EQ(d_fun_sort.fun_arity(), 3);
}

TEST_F(TestApi, sort_is_equal)
{
  ASSERT_EQ(bitwuzla::Sort(), bitwuzla::Sort());
  ASSERT_NE(bitwuzla::Sort(), d_bv_sort8);
  ASSERT_NE(d_bv_sort8, bitwuzla::Sort());
  ASSERT_EQ(d_bv_sort8, bitwuzla::mk_bv_sort(8));
  ASSERT_NE(d_bv_sort8, bitwuzla::mk_bv_sort(9));
}

TEST_F(TestApi, sort_is_array)
{
  ASSERT_TRUE(d_arr_sort_bv.is_array());
  ASSERT_TRUE(d_arr_sort_bvfp.is_array());
  ASSERT_TRUE(d_arr_sort_fpbv.is_array());
  ASSERT_FALSE(d_fun_sort.is_array());
  ASSERT_FALSE(d_fun_sort_fp.is_array());
  ASSERT_FALSE(d_bv_sort8.is_array());
  ASSERT_FALSE(d_fp_sort16.is_array());
}

TEST_F(TestApi, sort_is_bv)
{
  ASSERT_TRUE(d_bv_sort1.is_bv());
  ASSERT_TRUE(d_bv_sort8.is_bv());
  ASSERT_TRUE(d_bv_sort23.is_bv());
  ASSERT_TRUE(d_bv_sort32.is_bv());
  ASSERT_FALSE(d_fp_sort16.is_bv());
  ASSERT_FALSE(d_arr_sort_bv.is_bv());
  ASSERT_FALSE(d_arr_sort_bvfp.is_bv());
  ASSERT_FALSE(d_arr_sort_fpbv.is_bv());
  ASSERT_FALSE(d_fun_sort.is_bv());
}

TEST_F(TestApi, sort_is_fp)
{
  ASSERT_TRUE(d_fp_sort16.is_fp());
  ASSERT_TRUE(d_fp_sort32.is_fp());
  ASSERT_FALSE(d_bv_sort8.is_fp());
  ASSERT_FALSE(d_arr_sort_bv.is_fp());
  ASSERT_FALSE(d_arr_sort_bvfp.is_fp());
  ASSERT_FALSE(d_fun_sort_fp.is_fp());
}

TEST_F(TestApi, sort_is_fun)
{
  ASSERT_TRUE(d_fun_sort.is_fun());
  ASSERT_TRUE(d_fun_sort_fp.is_fun());
  ASSERT_FALSE(d_arr_sort_bv.is_fun());
  ASSERT_FALSE(d_arr_sort_bvfp.is_fun());
  ASSERT_FALSE(d_arr_sort_fpbv.is_fun());
  ASSERT_FALSE(d_bv_sort8.is_fun());
  ASSERT_FALSE(d_fp_sort16.is_fun());
}

TEST_F(TestApi, sort_is_rm)
{
  ASSERT_TRUE(d_rm_sort.is_rm());
  ASSERT_FALSE(d_bv_sort8.is_rm());
  ASSERT_FALSE(d_fp_sort16.is_rm());
  ASSERT_FALSE(d_arr_sort_bv.is_rm());
}

TEST_F(TestApi, sort_print)
{
  // TODO
  // ASSERT_THROW(bitwuzla_sort_dump(nullptr, "btor", stdout),
  // d_error_not_null); ASSERT_THROW(bitwuzla_sort_dump(d_bv_sort1, nullptr,
  // stdout),
  //             d_error_exp_str);
  // ASSERT_THROW(bitwuzla_sort_dump(d_bv_sort1, "smt2", nullptr),
  //             d_error_not_null);
  // ASSERT_THROW(bitwuzla_sort_dump(d_bv_sort1, "asdf", stdout),
  // d_error_format); ASSERT_NO_FATAL_FAILURE(bitwuzla_sort_dump(d_bv_sort1,
  // "btor", stdout)); ASSERT_NO_FATAL_FAILURE(bitwuzla_sort_dump(d_bv_sort8,
  // "smt2", stdout)); ASSERT_NO_FATAL_FAILURE(bitwuzla_sort_dump(d_rm_sort,
  // "smt2", stdout)); ASSERT_NO_FATAL_FAILURE(bitwuzla_sort_dump(d_fp_sort32,
  // "smt2", stdout)); std::cout << std::endl;
}

TEST_F(TestApi, regr1)
{
  std::vector<bitwuzla::Sort> domain({d_bv_sort8});
  bitwuzla::Sort fun_sort = bitwuzla::mk_fun_sort(domain, d_bv_sort8);
  ASSERT_NO_THROW(bitwuzla::mk_array_sort(d_bv_sort8, d_bv_sort8));
  std::vector<bitwuzla::Term> args(
      {bitwuzla::mk_const(d_bv_sort8, "x"), bitwuzla::mk_const(fun_sort, "f")});
  ASSERT_THROW(bitwuzla::mk_term(bitwuzla::Kind::APPLY, args),
               bitwuzla::BitwuzlaException);
}

TEST_F(TestApi, regr2)
{
  std::vector<bitwuzla::Sort> domain({d_bv_sort8});
  bitwuzla::Sort fun_sort   = bitwuzla::mk_fun_sort(domain, d_bv_sort8);
  bitwuzla::Sort array_sort = bitwuzla::mk_array_sort(d_bv_sort8, d_bv_sort8);
  ASSERT_NE(fun_sort, array_sort);
  bitwuzla::Term fun   = bitwuzla::mk_const(fun_sort);
  bitwuzla::Term array = bitwuzla::mk_const(array_sort);
  ASSERT_EQ(array_sort, array.sort());
  ASSERT_EQ(fun_sort, fun.sort());
  ASSERT_NE(fun.sort(), array.sort());
  ASSERT_TRUE(fun.sort().is_fun());
  ASSERT_TRUE(array.sort().is_array());
}

/* -------------------------------------------------------------------------- */
/* Term                                                                       */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, term_hash)
{
  ASSERT_THROW(std::hash<bitwuzla::Term>{}(bitwuzla::Term()),
               bitwuzla::BitwuzlaException);
  ASSERT_NO_THROW(std::hash<bitwuzla::Term>{}(d_bv_const8));
}

TEST_F(TestApi, term_sort)
{
  ASSERT_THROW(bitwuzla::Term().sort(), bitwuzla::BitwuzlaException);
  ASSERT_EQ(d_bv_const8.sort(), d_bv_sort8);
}

TEST_F(TestApi, term_symbol)
{
  ASSERT_THROW(bitwuzla::Term().symbol(), bitwuzla::BitwuzlaException);
  bitwuzla::Term x = bitwuzla::mk_const(d_bv_sort8, "x");
  ASSERT_TRUE(!x.symbol() || x.symbol()->get() == "x");
}

TEST_F(TestApi, term_is_const)
{
  ASSERT_FALSE(bitwuzla::Term().is_const());
  ASSERT_TRUE(d_array.is_const());
  ASSERT_TRUE(d_fun.is_const());
  ASSERT_TRUE(d_bv_const1.is_const());
  ASSERT_TRUE(d_fp_const16.is_const());
  ASSERT_TRUE(d_rm_const.is_const());
  ASSERT_FALSE(d_bv_one1.is_const());
  ASSERT_FALSE(d_store.is_const());
}

TEST_F(TestApi, term_is_var)
{
  ASSERT_FALSE(bitwuzla::Term().is_variable());
  ASSERT_TRUE(d_var1.is_variable());
  ASSERT_TRUE(d_bound_var.is_variable());
  ASSERT_FALSE(d_fp_pzero32.is_variable());
}

TEST_F(TestApi, term_is_value)
{
  ASSERT_FALSE(bitwuzla::Term().is_value());
  ASSERT_TRUE(d_bv_one1.is_value());
  ASSERT_TRUE(d_fp_nan32.is_value());
  ASSERT_FALSE(d_fp_const16.is_value());
  ASSERT_FALSE(d_exists.is_value());
}

TEST_F(TestApi, term_is_bv_value_zero)
{
  ASSERT_FALSE(bitwuzla::Term().is_bv_value_zero());
  ASSERT_TRUE(d_bv_zero8.is_bv_value_zero());
  ASSERT_FALSE(d_bv_one1.is_bv_value_zero());
  ASSERT_FALSE(d_bv_ones23.is_bv_value_zero());
  ASSERT_FALSE(d_bv_mins8.is_bv_value_zero());
  ASSERT_FALSE(d_bv_maxs8.is_bv_value_zero());
}

TEST_F(TestApi, term_is_bv_value_one)
{
  ASSERT_FALSE(bitwuzla::Term().is_bv_value_one());
  ASSERT_TRUE(d_bv_one1.is_bv_value_one());
  ASSERT_FALSE(d_bv_ones23.is_bv_value_one());
  ASSERT_FALSE(d_bv_mins8.is_bv_value_one());
  ASSERT_FALSE(d_bv_maxs8.is_bv_value_one());
  ASSERT_FALSE(d_bv_zero8.is_bv_value_one());
}

TEST_F(TestApi, term_is_bv_value_ones)
{
  ASSERT_FALSE(bitwuzla::Term().is_bv_value_ones());
  ASSERT_TRUE(d_bv_ones23.is_bv_value_ones());
  ASSERT_TRUE(d_bv_one1.is_bv_value_ones());
  ASSERT_FALSE(d_bv_mins8.is_bv_value_ones());
  ASSERT_FALSE(d_bv_maxs8.is_bv_value_ones());
  ASSERT_FALSE(d_bv_zero8.is_bv_value_ones());
}

TEST_F(TestApi, term_is_bv_value_min_signed)
{
  ASSERT_FALSE(bitwuzla::Term().is_bv_value_min_signed());
  ASSERT_TRUE(d_bv_mins8.is_bv_value_min_signed());
  ASSERT_TRUE(d_bv_one1.is_bv_value_min_signed());
  ASSERT_FALSE(d_bv_ones23.is_bv_value_min_signed());
  ASSERT_FALSE(d_bv_maxs8.is_bv_value_min_signed());
  ASSERT_FALSE(d_bv_zero8.is_bv_value_min_signed());
}

TEST_F(TestApi, term_is_bv_value_max_signed)
{
  ASSERT_FALSE(bitwuzla::Term().is_bv_value_max_signed());
  ASSERT_TRUE(d_bv_maxs8.is_bv_value_max_signed());
  ASSERT_FALSE(d_bv_mins8.is_bv_value_max_signed());
  ASSERT_FALSE(d_bv_one1.is_bv_value_max_signed());
  ASSERT_FALSE(d_bv_ones23.is_bv_value_max_signed());
  ASSERT_FALSE(d_bv_zero8.is_bv_value_max_signed());
}

TEST_F(TestApi, term_is_fp_value_pos_zero)
{
  ASSERT_FALSE(bitwuzla::Term().is_fp_value_pos_zero());
  ASSERT_TRUE(d_fp_pzero32.is_fp_value_pos_zero());
  ASSERT_FALSE(d_fp_nzero32.is_fp_value_pos_zero());
  ASSERT_FALSE(d_fp_pinf32.is_fp_value_pos_zero());
  ASSERT_FALSE(d_fp_ninf32.is_fp_value_pos_zero());
  ASSERT_FALSE(d_fp_nan32.is_fp_value_pos_zero());
}

TEST_F(TestApi, term_is_fp_value_neg_zero)
{
  ASSERT_FALSE(bitwuzla::Term().is_fp_value_neg_zero());
  ASSERT_TRUE(d_fp_nzero32.is_fp_value_neg_zero());
  ASSERT_FALSE(d_fp_pzero32.is_fp_value_neg_zero());
  ASSERT_FALSE(d_fp_pinf32.is_fp_value_neg_zero());
  ASSERT_FALSE(d_fp_ninf32.is_fp_value_neg_zero());
  ASSERT_FALSE(d_fp_nan32.is_fp_value_neg_zero());
}

TEST_F(TestApi, term_is_fp_value_pos_inf)
{
  ASSERT_FALSE(bitwuzla::Term().is_fp_value_pos_inf());
  ASSERT_TRUE(d_fp_pinf32.is_fp_value_pos_inf());
  ASSERT_FALSE(d_fp_pzero32.is_fp_value_pos_inf());
  ASSERT_FALSE(d_fp_nzero32.is_fp_value_pos_inf());
  ASSERT_FALSE(d_fp_ninf32.is_fp_value_pos_inf());
  ASSERT_FALSE(d_fp_nan32.is_fp_value_pos_inf());
}

TEST_F(TestApi, term_is_fp_value_neg_inf)
{
  ASSERT_FALSE(bitwuzla::Term().is_fp_value_neg_inf());
  ASSERT_TRUE(d_fp_ninf32.is_fp_value_neg_inf());
  ASSERT_FALSE(d_fp_pzero32.is_fp_value_neg_inf());
  ASSERT_FALSE(d_fp_nzero32.is_fp_value_neg_inf());
  ASSERT_FALSE(d_fp_pinf32.is_fp_value_neg_inf());
  ASSERT_FALSE(d_fp_nan32.is_fp_value_neg_inf());
}

TEST_F(TestApi, term_is_fp_value_nan)
{
  ASSERT_FALSE(bitwuzla::Term().is_fp_value_nan());
  ASSERT_TRUE(d_fp_nan32.is_fp_value_nan());
  ASSERT_FALSE(d_fp_pzero32.is_fp_value_nan());
  ASSERT_FALSE(d_fp_nzero32.is_fp_value_nan());
  ASSERT_FALSE(d_fp_pinf32.is_fp_value_nan());
  ASSERT_FALSE(d_fp_ninf32.is_fp_value_nan());
}

TEST_F(TestApi, term_is_rm_value_rna)
{
  ASSERT_FALSE(bitwuzla::Term().is_rm_value_rna());
  ASSERT_TRUE(d_rm_rna.is_rm_value_rna());
  ASSERT_FALSE(d_fp_pzero32.is_rm_value_rna());
  ASSERT_FALSE(d_rm_rne.is_rm_value_rna());
  ASSERT_FALSE(d_rm_rtn.is_rm_value_rna());
  ASSERT_FALSE(d_rm_rtp.is_rm_value_rna());
  ASSERT_FALSE(d_rm_rtz.is_rm_value_rna());
}

TEST_F(TestApi, term_is_rm_value_rne)
{
  ASSERT_FALSE(bitwuzla::Term().is_rm_value_rne());
  ASSERT_TRUE(d_rm_rne.is_rm_value_rne());
  ASSERT_FALSE(d_fun.is_rm_value_rne());
  ASSERT_FALSE(d_rm_rna.is_rm_value_rne());
  ASSERT_FALSE(d_rm_rtn.is_rm_value_rne());
  ASSERT_FALSE(d_rm_rtp.is_rm_value_rne());
  ASSERT_FALSE(d_rm_rtz.is_rm_value_rne());
}

TEST_F(TestApi, term_is_rm_value_rtn)
{
  ASSERT_FALSE(bitwuzla::Term().is_rm_value_rne());
  ASSERT_TRUE(d_rm_rtn.is_rm_value_rtn());
  ASSERT_FALSE(d_true.is_rm_value_rtn());
  ASSERT_FALSE(d_rm_rna.is_rm_value_rtn());
  ASSERT_FALSE(d_rm_rne.is_rm_value_rtn());
  ASSERT_FALSE(d_rm_rtp.is_rm_value_rtn());
  ASSERT_FALSE(d_rm_rtz.is_rm_value_rtn());
}

TEST_F(TestApi, term_is_rm_value_rtp)
{
  ASSERT_FALSE(bitwuzla::Term().is_rm_value_rtp());
  ASSERT_TRUE(d_rm_rtp.is_rm_value_rtp());
  ASSERT_FALSE(d_var2.is_rm_value_rtp());
  ASSERT_FALSE(d_rm_rna.is_rm_value_rtp());
  ASSERT_FALSE(d_rm_rne.is_rm_value_rtp());
  ASSERT_FALSE(d_rm_rtn.is_rm_value_rtp());
  ASSERT_FALSE(d_rm_rtz.is_rm_value_rtp());
}

TEST_F(TestApi, term_is_rm_value_rtz)
{
  ASSERT_FALSE(bitwuzla::Term().is_rm_value_rtz());
  ASSERT_TRUE(d_rm_rtz.is_rm_value_rtz());
  ASSERT_FALSE(d_lambda.is_rm_value_rtz());
  ASSERT_FALSE(d_rm_rna.is_rm_value_rtz());
  ASSERT_FALSE(d_rm_rne.is_rm_value_rtz());
  ASSERT_FALSE(d_rm_rtn.is_rm_value_rtz());
  ASSERT_FALSE(d_rm_rtp.is_rm_value_rtz());
}

TEST_F(TestApi, term_is_const_array)
{
  GTEST_SKIP();  // TODO enable when implemented
  ASSERT_FALSE(bitwuzla::Term().is_const_array());
  ASSERT_TRUE(
      bitwuzla::mk_const_array(d_arr_sort_bv, d_bv_zero8).is_const_array());
  ASSERT_FALSE(d_array.is_const_array());
  ASSERT_FALSE(d_array_fpbv.is_const_array());
}

TEST_F(TestApi, term_print)
{
  GTEST_SKIP();  // TODO enable when implemented
  // ASSERT_THROW(bitwuzla_term_dump(nullptr, "btor", stdout),
  // d_error_not_null); ASSERT_THROW(bitwuzla_term_dump(d_and_bv_const1,
  // nullptr, stdout),
  //              d_error_exp_str);
  // ASSERT_THROW(bitwuzla_term_dump(d_and_bv_const1, "smt2", nullptr),
  //              d_error_not_null);
  // ASSERT_THROW(bitwuzla_term_dump(d_and_bv_const1, "asdf", stdout),
  //              d_error_format);
  // ASSERT_NO_FATAL_FAILURE(bitwuzla_term_dump(d_and_bv_const1, "btor",
  // stdout)); ASSERT_NO_FATAL_FAILURE(bitwuzla_term_dump(d_and_bv_const1,
  // "smt2", stdout));
  // ASSERT_NO_FATAL_FAILURE(bitwuzla_term_dump(d_other_exists, "btor",
  // stdout)); ASSERT_NO_FATAL_FAILURE(bitwuzla_term_dump(d_other_exists,
  // "smt2", stdout)); std::cout << std::endl;
}

TEST_F(TestApi, term_print_regr0)
{
  GTEST_SKIP();  // TODO enable when implemented
  // testing::internal::CaptureStdout();

  // bitwuzla_term_dump(d_rm_rne, "smt2", stdout);
  // printf("\n");
  // bitwuzla_term_dump(d_rm_rna, "smt2", stdout);
  // printf("\n");
  // bitwuzla_term_dump(d_rm_rtn, "smt2", stdout);
  // printf("\n");
  // bitwuzla_term_dump(d_rm_rtp, "smt2", stdout);
  // printf("\n");
  // bitwuzla_term_dump(d_rm_rtz, "smt2", stdout);

  // std::string output = testing::internal::GetCapturedStdout();
  // ASSERT_EQ(output, "RNE\nRNA\nRTN\nRTP\nRTZ");
}

TEST_F(TestApi, term_print_regr1)
{
  GTEST_SKIP();  // TODO enable when implemented
  bitwuzla::Sort bv_sort5  = bitwuzla::mk_bv_sort(5);
  bitwuzla::Sort bv_sort10 = bitwuzla::mk_bv_sort(10);
  bitwuzla::Term fp_const;
  std::string output;

  fp_const = bitwuzla::mk_fp_value(bitwuzla::mk_bv_zero(d_bv_sort1),
                                   bitwuzla::mk_bv_zero(bv_sort5),
                                   bitwuzla::mk_bv_zero(bv_sort10));

  testing::internal::CaptureStdout();
  // bitwuzla_term_dump(fp_const, "smt2", stdout);
  // output = testing::internal::GetCapturedStdout();
  // ASSERT_EQ(output, "(fp #b0 #b00000 #b0000000000)");

  fp_const = bitwuzla::mk_fp_value(bitwuzla::mk_bv_one(d_bv_sort1),
                                   bitwuzla::mk_bv_zero(bv_sort5),
                                   bitwuzla::mk_bv_zero(bv_sort10));

  // testing::internal::CaptureStdout();
  // bitwuzla_term_dump(fp_const, "smt2", stdout);
  // output = testing::internal::GetCapturedStdout();
  // ASSERT_EQ(output, "(fp #b1 #b00000 #b0000000000)");

  fp_const = bitwuzla::mk_fp_value(bitwuzla::mk_bv_zero(d_bv_sort1),
                                   bitwuzla::mk_bv_zero(bv_sort5),
                                   bitwuzla::mk_bv_one(bv_sort10));

  // testing::internal::CaptureStdout();
  // bitwuzla_term_dump(fp_const, "smt2", stdout);
  // output = testing::internal::GetCapturedStdout();
  // ASSERT_EQ(output, "(fp #b0 #b00000 #b0000000001)");

  fp_const = bitwuzla::mk_fp_value(bitwuzla::mk_bv_one(d_bv_sort1),
                                   bitwuzla::mk_bv_zero(bv_sort5),
                                   bitwuzla::mk_bv_one(bv_sort10));

  // testing::internal::CaptureStdout();
  // bitwuzla_term_dump(fp_const, "smt2", stdout);
  // output = testing::internal::GetCapturedStdout();
  // ASSERT_EQ(output, "(fp #b1 #b00000 #b0000000001)");
}

TEST_F(TestApi, terms_indexed)
{
  bitwuzla::Sort fp_sort = bitwuzla::mk_fp_sort(5, 11);
  bitwuzla::Sort bv_sort = bitwuzla::mk_bv_sort(8);
  bitwuzla::Term fp_term = bitwuzla::mk_const(fp_sort);
  bitwuzla::Term bv_term = bitwuzla::mk_const(bv_sort);
  bitwuzla::Term rm      = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RNE);

  bitwuzla::Term idx;

  idx = bitwuzla::mk_term(bitwuzla::Kind::FP_TO_SBV, {rm, fp_term}, {8});
  ASSERT_EQ(idx.num_indices(), 1);
  auto indices = idx.indices();
  ASSERT_EQ(indices.size(), 1);
  ASSERT_EQ(indices[0], 8);

  idx = bitwuzla::mk_term(bitwuzla::Kind::FP_TO_UBV, {rm, fp_term}, {9});
  ASSERT_EQ(idx.num_indices(), 1);
  indices = idx.indices();
  ASSERT_EQ(indices.size(), 1);
  ASSERT_EQ(indices[0], 9);

  idx = bitwuzla::mk_term(bitwuzla::Kind::FP_TO_FP_FROM_BV, {bv_term}, {3, 5});
  ASSERT_EQ(idx.num_indices(), 2);
  indices = idx.indices();
  ASSERT_EQ(indices.size(), 2);
  ASSERT_EQ(indices[0], 3);
  ASSERT_EQ(indices[1], 5);

  idx = bitwuzla::mk_term(
      bitwuzla::Kind::FP_TO_FP_FROM_FP, {rm, fp_term}, {7, 18});
  ASSERT_EQ(idx.num_indices(), 2);
  indices = idx.indices();
  ASSERT_EQ(indices.size(), 2);
  ASSERT_EQ(indices[0], 7);
  ASSERT_EQ(indices[1], 18);

  idx = bitwuzla::mk_term(
      bitwuzla::Kind::FP_TO_FP_FROM_SBV, {rm, bv_term}, {8, 24});
  ASSERT_EQ(idx.num_indices(), 2);
  indices = idx.indices();
  ASSERT_EQ(indices.size(), 2);
  ASSERT_EQ(indices[0], 8);
  ASSERT_EQ(indices[1], 24);

  idx = bitwuzla::mk_term(
      bitwuzla::Kind::FP_TO_FP_FROM_UBV, {rm, bv_term}, {5, 11});
  ASSERT_EQ(idx.num_indices(), 2);
  indices = idx.indices();
  ASSERT_EQ(indices.size(), 2);
  ASSERT_EQ(indices[0], 5);
  ASSERT_EQ(indices[1], 11);

  idx = bitwuzla::mk_term(bitwuzla::Kind::BV_EXTRACT, {bv_term}, {6, 0});
  ASSERT_EQ(idx.num_indices(), 2);
  indices = idx.indices();
  ASSERT_EQ(indices.size(), 2);
  ASSERT_EQ(indices[0], 6);
  ASSERT_EQ(indices[1], 0);
}

TEST_F(TestApi, terms)
{
#ifndef NDEBUG
  bitwuzla::Sort fp_sort    = bitwuzla::mk_fp_sort(5, 11);
  bitwuzla::Sort bv_sort    = bitwuzla::mk_bv_sort(16);
  bitwuzla::Sort bool_sort  = bitwuzla::mk_bool_sort();
  bitwuzla::Sort array_sort = bitwuzla::mk_array_sort(bv_sort, bv_sort);
  std::vector<bitwuzla::Sort> domain = {bv_sort, bv_sort, bv_sort};
  bitwuzla::Sort fun_sort            = bitwuzla::mk_fun_sort(domain, bv_sort);

  std::vector<bitwuzla::Term> fp_args = {d_rm_rna,
                                         bitwuzla::mk_const(fp_sort),
                                         bitwuzla::mk_const(fp_sort),
                                         bitwuzla::mk_const(fp_sort)};

  std::vector<bitwuzla::Term> bv_args = {bitwuzla::mk_const(bv_sort),
                                         bitwuzla::mk_const(bv_sort),
                                         bitwuzla::mk_const(bv_sort),
                                         bitwuzla::mk_const(bv_sort)};

  std::vector<bitwuzla::Term> bool_args = {bitwuzla::mk_const(bool_sort),
                                           bitwuzla::mk_const(bool_sort)};

  for (size_t i = 0; i < static_cast<size_t>(bitwuzla::Kind::NUM_KINDS); ++i)
  {
    bitwuzla::Kind kind = static_cast<bitwuzla::Kind>(i);
    bitwuzla::Term term;
    switch (kind)
    {
      case bitwuzla::Kind::CONSTANT:
      case bitwuzla::Kind::CONST_ARRAY:
      case bitwuzla::Kind::VALUE:
      case bitwuzla::Kind::VARIABLE: continue;

      // Boolean
      case bitwuzla::Kind::NOT:
        term = bitwuzla::mk_term(kind, {bool_args[0]});
        break;

      case bitwuzla::Kind::AND:
      case bitwuzla::Kind::IFF:
      case bitwuzla::Kind::IMPLIES:
      case bitwuzla::Kind::OR:
      case bitwuzla::Kind::XOR:
        term = bitwuzla::mk_term(kind, bool_args);
        break;

      // BV Unary
      case bitwuzla::Kind::BV_DEC:
      case bitwuzla::Kind::BV_INC:
      case bitwuzla::Kind::BV_NEG:
      case bitwuzla::Kind::BV_NOT:
      case bitwuzla::Kind::BV_REDAND:
      case bitwuzla::Kind::BV_REDOR:
      case bitwuzla::Kind::BV_REDXOR:
        term = bitwuzla::mk_term(kind, {bv_args[0]});
        break;

      // BV Binary
      case bitwuzla::Kind::BV_ASHR:
      case bitwuzla::Kind::BV_COMP:
      case bitwuzla::Kind::BV_NAND:
      case bitwuzla::Kind::BV_NOR:
      case bitwuzla::Kind::BV_ROL:
      case bitwuzla::Kind::BV_ROR:
      case bitwuzla::Kind::BV_SADD_OVERFLOW:
      case bitwuzla::Kind::BV_SDIV_OVERFLOW:
      case bitwuzla::Kind::BV_SDIV:
      case bitwuzla::Kind::BV_SGE:
      case bitwuzla::Kind::BV_SGT:
      case bitwuzla::Kind::BV_SHL:
      case bitwuzla::Kind::BV_SHR:
      case bitwuzla::Kind::BV_SLE:
      case bitwuzla::Kind::BV_SLT:
      case bitwuzla::Kind::BV_SMOD:
      case bitwuzla::Kind::BV_SMUL_OVERFLOW:
      case bitwuzla::Kind::BV_SREM:
      case bitwuzla::Kind::BV_SSUB_OVERFLOW:
      case bitwuzla::Kind::BV_SUB:
      case bitwuzla::Kind::BV_UADD_OVERFLOW:
      case bitwuzla::Kind::BV_UDIV:
      case bitwuzla::Kind::BV_UGE:
      case bitwuzla::Kind::BV_UGT:
      case bitwuzla::Kind::BV_ULE:
      case bitwuzla::Kind::BV_ULT:
      case bitwuzla::Kind::BV_UMUL_OVERFLOW:
      case bitwuzla::Kind::BV_UREM:
      case bitwuzla::Kind::BV_USUB_OVERFLOW:
      case bitwuzla::Kind::BV_XNOR:
        term = bitwuzla::mk_term(kind, {bv_args[0], bv_args[1]});
        break;

      // BV Binary+
      case bitwuzla::Kind::BV_ADD:
      case bitwuzla::Kind::BV_AND:
      case bitwuzla::Kind::BV_CONCAT:
      case bitwuzla::Kind::BV_MUL:
      case bitwuzla::Kind::BV_OR:
      case bitwuzla::Kind::BV_XOR:
        term = bitwuzla::mk_term(kind, bv_args);
        break;

      case bitwuzla::Kind::DISTINCT:
      case bitwuzla::Kind::EQUAL:
        term = bitwuzla::mk_term(kind, bv_args);
        break;

      // BV indexed
      case bitwuzla::Kind::BV_EXTRACT:
        term = bitwuzla::mk_term(kind, {bv_args[0]}, {3, 2});
        break;

      case bitwuzla::Kind::BV_REPEAT:
      case bitwuzla::Kind::BV_ROLI:
      case bitwuzla::Kind::BV_RORI:
      case bitwuzla::Kind::BV_SIGN_EXTEND:
      case bitwuzla::Kind::BV_ZERO_EXTEND:
        term = bitwuzla::mk_term(kind, {bv_args[0]}, {5});
        break;

      // Arrays
      case bitwuzla::Kind::ARRAY_SELECT:
        term = bitwuzla::mk_term(kind,
                                 {bitwuzla::mk_const(array_sort), bv_args[0]});
        break;

      case bitwuzla::Kind::ARRAY_STORE:
        term = bitwuzla::mk_term(
            kind, {bitwuzla::mk_const(array_sort), bv_args[0], bv_args[1]});
        break;

      case bitwuzla::Kind::APPLY:
        term = bitwuzla::mk_term(
            kind,
            {bitwuzla::mk_const(fun_sort), bv_args[0], bv_args[1], bv_args[2]});
        break;

      // Binder
      case bitwuzla::Kind::EXISTS:
      case bitwuzla::Kind::FORALL:
      case bitwuzla::Kind::LAMBDA: {
        std::vector<bitwuzla::Term> vars = {
            bitwuzla::mk_var(bv_sort),
            bitwuzla::mk_var(bv_sort),
        };
        // body
        term = bitwuzla::mk_term(
            kind,
            {vars[0],
             vars[1],
             bitwuzla::mk_term(bitwuzla::Kind::BV_SLT, vars)});
        break;
      }

      // FP Unary
      case bitwuzla::Kind::FP_ABS:
      case bitwuzla::Kind::FP_IS_INF:
      case bitwuzla::Kind::FP_IS_NAN:
      case bitwuzla::Kind::FP_IS_NEG:
      case bitwuzla::Kind::FP_IS_NORMAL:
      case bitwuzla::Kind::FP_IS_POS:
      case bitwuzla::Kind::FP_IS_SUBNORMAL:
      case bitwuzla::Kind::FP_IS_ZERO:
      case bitwuzla::Kind::FP_NEG:
        term = bitwuzla::mk_term(kind, {fp_args[1]});
        break;

      // FP Binary
      case bitwuzla::Kind::FP_EQUAL:
      case bitwuzla::Kind::FP_GEQ:
      case bitwuzla::Kind::FP_GT:
      case bitwuzla::Kind::FP_LEQ:
      case bitwuzla::Kind::FP_LT:
      case bitwuzla::Kind::FP_MAX:
      case bitwuzla::Kind::FP_MIN:
      case bitwuzla::Kind::FP_REM:
        term = bitwuzla::mk_term(kind, {fp_args[1], fp_args[2]});
        break;

      case bitwuzla::Kind::FP_SQRT:
      case bitwuzla::Kind::FP_RTI:
        term = bitwuzla::mk_term(kind, {fp_args[0], fp_args[1]});
        break;

      // FP Ternary
      case bitwuzla::Kind::FP_ADD:
      case bitwuzla::Kind::FP_DIV:
      case bitwuzla::Kind::FP_MUL:
      case bitwuzla::Kind::FP_SUB:
        term = bitwuzla::mk_term(kind, {fp_args.begin(), fp_args.end() - 1});
        break;

      case bitwuzla::Kind::FP_FP:
        term = bitwuzla::mk_term(kind,
                                 {bitwuzla::mk_const(bitwuzla::mk_bv_sort(1)),
                                  bv_args[0],
                                  bv_args[1]});
        break;

      // FP Quaternery
      case bitwuzla::Kind::FP_FMA:
        term = bitwuzla::mk_term(kind, fp_args);
        break;

      // FP indexed
      case bitwuzla::Kind::FP_TO_FP_FROM_BV:
        term = bitwuzla::mk_term(kind, {bv_args[0]}, {5, 11});
        break;

      case bitwuzla::Kind::FP_TO_FP_FROM_SBV:
      case bitwuzla::Kind::FP_TO_FP_FROM_UBV:
        term = bitwuzla::mk_term(kind, {fp_args[0], bv_args[0]}, {5, 11});
        break;

      case bitwuzla::Kind::FP_TO_FP_FROM_FP:
        term = bitwuzla::mk_term(kind, {fp_args[0], fp_args[1]}, {5, 11});
        break;

      case bitwuzla::Kind::FP_TO_SBV:
      case bitwuzla::Kind::FP_TO_UBV:
        term = bitwuzla::mk_term(kind, {fp_args[0], fp_args[1]}, {16});
        break;

      // Others
      case bitwuzla::Kind::ITE: {
        term = bitwuzla::mk_term(kind,
                                 {
                                     bool_args[0],
                                     bv_args[0],
                                     bv_args[1],
                                 });
        break;
      }

      default: break;
    }
    // Unhandled BitwuzlaKind
    ASSERT_FALSE(term.is_null());

    auto children = term.children();
    size_t size   = children.size();

    if (term.is_const() || term.is_variable() || term.is_value())
    {
      ASSERT_TRUE(children.empty());
      continue;
    }

    ASSERT_GT(size, 0);
    for (size_t i = 0; i < size; ++i)
    {
      ASSERT_FALSE(children[i].is_null());
    }

    bitwuzla::Term tterm;
    if (term.is_const_array())
    {
      ASSERT_EQ(size, 1);
      tterm = bitwuzla::mk_const_array(term.sort(), children[0]);
    }
    else
    {
      kind = term.kind();
      if (term.num_indices() > 0)
      {
        tterm = bitwuzla::mk_term(kind, children, term.indices());
      }
      else if (kind == bitwuzla::Kind::LAMBDA || kind == bitwuzla::Kind::FORALL
               || kind == bitwuzla::Kind::EXISTS)
      {
        // TODO: variables are already bound and can't be passed to mk_term
        // create new vars and substitute
        tterm = term;
      }
      else
      {
        assert(kind != bitwuzla::Kind::BV_NOT || size == 1);
        tterm = bitwuzla::mk_term(kind, children);
      }
    }
    ASSERT_EQ(tterm, term);
  }

  bitwuzla::Term t;

  t = bitwuzla::mk_const(bv_sort);
  ASSERT_EQ(t.kind(), bitwuzla::Kind::CONSTANT);
  ASSERT_TRUE(t.children().empty());

  t = bitwuzla::mk_var(bv_sort);
  ASSERT_EQ(t.kind(), bitwuzla::Kind::VARIABLE);
  ASSERT_TRUE(t.children().empty());

  t = bitwuzla::mk_rm_value(bitwuzla::RoundingMode::RNA);
  ASSERT_EQ(t.kind(), bitwuzla::Kind::VALUE);
  ASSERT_TRUE(t.children().empty());

  t = bitwuzla::mk_fp_value_from_real(d_fp_sort16, t, "1.1");
  ASSERT_EQ(t.kind(), bitwuzla::Kind::VALUE);
  ASSERT_TRUE(t.children().empty());

  t = bitwuzla::mk_fp_nan(d_fp_sort16);
  ASSERT_EQ(t.kind(), bitwuzla::Kind::VALUE);
  ASSERT_TRUE(t.children().empty());

  t = bitwuzla::mk_bv_one(bv_sort);
  ASSERT_EQ(t.kind(), bitwuzla::Kind::VALUE);
  ASSERT_TRUE(t.children().empty());

  t = bitwuzla::mk_bv_value(bv_sort, "43", 10);
  ASSERT_EQ(t.kind(), bitwuzla::Kind::VALUE);
  ASSERT_TRUE(t.children().empty());

  // TODO enable when implemented
  // t = bitwuzla::mk_const_array(array_sort, t);
  // ASSERT_EQ(t.kind(), bitwuzla::Kind::VALUE);
  // ASSERT_TRUE(t.children().empty());
#endif
}

TEST_F(TestApi, substitute)
{
  GTEST_SKIP();  // TODO enable when implemented
  bitwuzla::Sort bv_sort             = bitwuzla::mk_bv_sort(16);
  bitwuzla::Sort bool_sort           = bitwuzla::mk_bool_sort();
  std::vector<bitwuzla::Sort> domain = {bv_sort, bv_sort, bv_sort};
  bitwuzla::Sort fun_sort            = bitwuzla::mk_fun_sort(domain, bool_sort);
  bitwuzla::Sort array_sort = bitwuzla::mk_array_sort(bv_sort, bv_sort);

  bitwuzla::Term bv_const = bitwuzla::mk_const(bv_sort);
  bitwuzla::Term bv_value = bitwuzla::mk_bv_value(bv_sort, "143", 10);

  // simple substitution const -> value
  {
    std::unordered_map<bitwuzla::Term, bitwuzla::Term> map{
        {bv_const, bv_value}};
    bitwuzla::Term result = bitwuzla::substitute_term(bv_const, map);
    ASSERT_EQ(result, bv_value);
  }

  // (sdiv x y) -> (sdiv value y)
  {
    bitwuzla::Term x = bitwuzla::mk_const(bv_sort);
    bitwuzla::Term y = bitwuzla::mk_const(bv_sort);

    std::unordered_map<bitwuzla::Term, bitwuzla::Term> map{{x, bv_value}};

    bitwuzla::Term result = bitwuzla::substitute_term(
        bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV, {x, y}), map);
    ASSERT_EQ(result,
              bitwuzla::mk_term(bitwuzla::Kind::BV_SDIV, {bv_value, y}));
  }

  // partial substitution of variables in quantified formula
  {
    std::vector<bitwuzla::Term> args = {bitwuzla::mk_const(fun_sort),
                                        bitwuzla::mk_var(bv_sort, "x"),
                                        bitwuzla::mk_var(bv_sort, "y"),
                                        bitwuzla::mk_var(bv_sort, "z")};
    args.push_back(bitwuzla::mk_term(bitwuzla::Kind::APPLY,
                                     {args.begin() + 1, args.end()}));
    bitwuzla::Term q = bitwuzla::mk_term(bitwuzla::Kind::FORALL, args);

    std::unordered_map<bitwuzla::Term, bitwuzla::Term> map{
        {args[1], bitwuzla::mk_const(bv_sort)},
        {args[2], bitwuzla::mk_const(bv_sort)}};

    // Build expected
    std::vector<bitwuzla::Term> args_expected = {
        args[0], map.at(args[1]), map.at(args[2]), bitwuzla::mk_var(bv_sort)};
    args_expected.push_back(
        bitwuzla::mk_term(bitwuzla::Kind::APPLY, args_expected));
    bitwuzla::Term expected =
        bitwuzla::mk_term(bitwuzla::Kind::FORALL, {args_expected[3]});

    bitwuzla::Term result = bitwuzla::substitute_term(q, map);
    ASSERT_EQ(result, expected);
  }

  // substitute term in constant array
  {
    bitwuzla::Term term        = bitwuzla::mk_const(bv_sort);
    bitwuzla::Term const_array = bitwuzla::mk_const_array(array_sort, term);

    std::unordered_map<bitwuzla::Term, bitwuzla::Term> map{{term, bv_value}};

    bitwuzla::Term result = bitwuzla::substitute_term(const_array, map);

    bitwuzla::Term expected = bitwuzla::mk_const_array(array_sort, bv_value);
    ASSERT_EQ(result, expected);
    ASSERT_TRUE(term.is_const_array());
  }
}

TEST_F(TestApi, term_print1)
{
  GTEST_SKIP();  // TODO enable when implemented
  std::string filename     = "term_dump1.out";
  FILE *tmpfile            = fopen(filename.c_str(), "w");
  bitwuzla::Sort bv1       = bitwuzla::mk_bool_sort();
  bitwuzla::Term a         = bitwuzla::mk_const(bv1, "a");
  bitwuzla::Term nota      = bitwuzla::mk_term(bitwuzla::Kind::NOT, {a});
  // bitwuzla_term_dump(nota, "smt2", tmpfile);
  fclose(tmpfile);

  std::ifstream ifs(filename);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  unlink(filename.c_str());

  ASSERT_EQ("(bvnot a)", content);
}

TEST_F(TestApi, term_print2)
{
  GTEST_SKIP();  // TODO enable when implemented
  std::string filename = "term_dump2.out";
  FILE *tmpfile        = fopen(filename.c_str(), "w");

  bitwuzla::Sort bv1   = bitwuzla::mk_bv_sort(1);
  bitwuzla::Sort fn1_1 = bitwuzla::mk_fun_sort({bv1}, bv1);
  bitwuzla::Term f     = bitwuzla::mk_const(fn1_1, "f");
  // bitwuzla_term_dump(f, "smt2", tmpfile);
  fclose(tmpfile);

  std::ifstream ifs(filename);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  unlink(filename.c_str());

  ASSERT_EQ("(declare-fun f ((_ BitVec 1)) (_ BitVec 1))\n", content);
}

TEST_F(TestApi, term_print3)
{
  GTEST_SKIP();  // TODO enable when implemented
  std::string filename = "term_dump3.out";
  FILE *tmpfile        = fopen(filename.c_str(), "w");

  bitwuzla::Sort bv1   = bitwuzla::mk_bv_sort(1);
  bitwuzla::Sort ar1_1 = bitwuzla::mk_array_sort(bv1, bv1);
  bitwuzla::Term a     = bitwuzla::mk_const(ar1_1, "a");
  // bitwuzla_term_dump(a, "smt2", tmpfile);
  fclose(tmpfile);

  std::ifstream ifs(filename);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  unlink(filename.c_str());

  ASSERT_EQ("(declare-const a (Array (_ BitVec 1) (_ BitVec 1)))\n", content);
}

TEST_F(TestApi, dump_formula2)
{
  GTEST_SKIP();  // TODO enable when implemented
  std::string filename = "formula_dump2.out";
  FILE *tmpfile        = fopen(filename.c_str(), "w");

  bitwuzla::Options options;
  // options.set(bitwuzla::Option::PRETTY_PRINT, false);
  bitwuzla::Sort bv1   = bitwuzla::mk_bv_sort(1);
  bitwuzla::Sort ar1_1 = bitwuzla::mk_array_sort(bv1, bv1);
  bitwuzla::Term a     = bitwuzla::mk_const(ar1_1, "a");
  bitwuzla::Term b     = bitwuzla::mk_const(ar1_1, "b");
  bitwuzla::Term z     = bitwuzla::mk_false();
  bitwuzla::Term e = bitwuzla::mk_term(bitwuzla::Kind::ARRAY_SELECT, {a, z});
  bitwuzla::Term c = bitwuzla::mk_term(bitwuzla::Kind::EQUAL, {a, b});
  bitwuzla::Bitwuzla bitwuzla(options);
  bitwuzla.assert_formula(e);
  bitwuzla.assert_formula(c);
  // bitwuzla_dump_formula(d_bzla, "smt2", tmpfile);
  fclose(tmpfile);

  std::ifstream ifs(filename);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  unlink(filename.c_str());

  ASSERT_EQ(
      "(set-logic QF_ABV)\n"
      "(declare-const a (Array (_ BitVec 1) (_ BitVec 1)))\n"
      "(declare-const b (Array (_ BitVec 1) (_ BitVec 1)))\n"
      "(assert (= (select a #b0) #b1))\n"
      "(assert (= a b))\n"
      "(check-sat)\n"
      "(exit)\n",
      content);
}

TEST_F(TestApi, arrayfun)
{
  bitwuzla::Sort bvsort = bitwuzla::mk_bv_sort(4);
  std::vector<bitwuzla::Sort> domain{bvsort};
  bitwuzla::Sort funsort = bitwuzla::mk_fun_sort(domain, bvsort);
  bitwuzla::Sort arrsort = bitwuzla::mk_array_sort(bvsort, bvsort);
  bitwuzla::Term f       = bitwuzla::mk_const(funsort, "f");
  bitwuzla::Term a       = bitwuzla::mk_const(arrsort, "a");
  ASSERT_TRUE(f.sort() != a.sort());
  ASSERT_TRUE(f.sort().is_fun());
  ASSERT_TRUE(!a.sort().is_fun());
  ASSERT_TRUE(!f.sort().is_array());
  ASSERT_TRUE(a.sort().is_array());
}

/* -------------------------------------------------------------------------- */
/* Parsing                                                                    */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, parse)
{
  std::string infile_name = "fp_regr1.smt2";
  std::stringstream ss;
  ss << BZLA_REGRESS_DIR << infile_name;
  bitwuzla::Options options;

  ASSERT_THROW(bitwuzla::parse(options, ""), bitwuzla::BitwuzlaException);
  ASSERT_THROW(bitwuzla::parse(options, infile_name),
               bitwuzla::BitwuzlaException);

  // TODO should this throw? (parsing after having created expressions)
  // ASSERT_THROW(
  //    bitwuzla::parse(
  //        ifile, infile_name, std::cout, error_msg, status, is_smt2),
  //    bitwuzla::BitwuzlaException);

  std::string err = bitwuzla::parse(options, ss.str().c_str());
  ASSERT_TRUE(err.empty());
}

/* -------------------------------------------------------------------------- */
/* Termination function                                                       */
/* -------------------------------------------------------------------------- */

TEST_F(TestApi, terminate)
{
  class TestTerminator : public bitwuzla::Terminator
  {
   public:
    bool terminate() override { return true; }
  };

  bitwuzla::Sort bv_sort4 = bitwuzla::mk_bv_sort(4);
  bitwuzla::Term x        = bitwuzla::mk_const(bv_sort4);
  bitwuzla::Term s        = bitwuzla::mk_const(bv_sort4);
  bitwuzla::Term t        = bitwuzla::mk_const(bv_sort4);
  bitwuzla::Term a        = bitwuzla::mk_term(
      bitwuzla::Kind::AND,
      {bitwuzla::mk_term(bitwuzla::Kind::EQUAL,
                         {bitwuzla::mk_term(bitwuzla::Kind::BV_ADD, {x, x}),
                                 bitwuzla::mk_bv_value_uint64(bv_sort4, 3)}),
              bitwuzla::mk_term(
           bitwuzla::Kind::NOT,
           {bitwuzla::mk_term(bitwuzla::Kind::BV_UADD_OVERFLOW, {x, x})})});
  bitwuzla::Term b = bitwuzla::mk_term(
      bitwuzla::Kind::DISTINCT,
      {bitwuzla::mk_term(
           bitwuzla::Kind::BV_MUL,
           {s, bitwuzla::mk_term(bitwuzla::Kind::BV_MUL, {x, t})}),
       bitwuzla::mk_term(
           bitwuzla::Kind::BV_MUL,
           {bitwuzla::mk_term(bitwuzla::Kind::BV_MUL, {s, x}), t})});
  // solved by rewriting
  {
    bitwuzla::Options opts;
    opts.set(bitwuzla::Option::BV_SOLVER, "bitblast");
    bitwuzla::Bitwuzla bitwuzla(opts);
    bitwuzla.assert_formula(a);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::UNSAT);
  }
  {
    bitwuzla::Options opts;
    opts.set(bitwuzla::Option::BV_SOLVER, "prop");
    bitwuzla::Bitwuzla bitwuzla(opts);
    bitwuzla.assert_formula(a);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::UNSAT);
  }
  // not solved by rewriting, should be terminated in the prop case
  TestTerminator tt;
  {
    bitwuzla::Options opts;
    opts.set(bitwuzla::Option::BV_SOLVER, "bitblast");
    bitwuzla::Bitwuzla bitwuzla(opts);
    bitwuzla.configure_terminator(&tt);
    bitwuzla.assert_formula(b);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::UNSAT);
  }
  {
    bitwuzla::Options opts;
    opts.set(bitwuzla::Option::BV_SOLVER, "prop");
    bitwuzla::Bitwuzla bitwuzla(opts);
    bitwuzla.configure_terminator(&tt);
    bitwuzla.assert_formula(b);
    ASSERT_EQ(bitwuzla.check_sat(), bitwuzla::Result::UNKNOWN);
  }
}

/* -------------------------------------------------------------------------- */

}  // namespace bzla::test

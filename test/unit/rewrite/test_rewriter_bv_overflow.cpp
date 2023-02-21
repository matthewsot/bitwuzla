#include <cassert>
#include <cmath>
#include <cstdint>

#include "node/node_manager.h"
#include "option/option.h"
#include "solving_context.h"
#include "test/unit/rewrite/test_rewriter.h"

namespace bzla::test {

using namespace bzla::node;

class TestRewriterBvOverflow : public TestRewriter
{
 protected:
  static constexpr uint32_t TEST_OVERFLOW_LOW  = 1;
  static constexpr uint32_t TEST_OVERFLOW_HIGH = 4;

  int32_t add(int32_t x, int32_t y) { return x + y; }

  int32_t sub(int32_t x, int32_t y) { return x - y; }

  int32_t mul(int32_t x, int32_t y) { return x * y; }

  int32_t div(int32_t x, int32_t y)
  {
    assert(y != 0);
    return x / y;
  }

  void u_overflow_test(Kind kind, int32_t low, int32_t high, uint32_t rwl)
  {
    assert(low > 0);
    assert(low <= high);

    for (int32_t num_bits = low; num_bits <= high; num_bits++)
    {
      for (int32_t i = 0, max = std::pow(2, num_bits); i < max; ++i)
      {
        for (int32_t j = 0; j < max; ++j)
        {
          option::Options options;
          options.set<uint64_t>(option::Option::REWRITE_LEVEL, rwl);
          SolvingContext ctx = SolvingContext(options);

          int32_t result = 0;

          switch (kind)
          {
            case Kind::BV_UADDO:
            case Kind::BV_SADDO: result = add(i, j); break;

            case Kind::BV_USUBO:
            case Kind::BV_SSUBO: result = sub(i, j); break;

            case Kind::BV_UMULO:
            case Kind::BV_SMULO: result = mul(i, j); break;
            default: assert(kind == Kind::BV_SDIVO); result = div(i, j);
          }

          NodeManager& nm = NodeManager::get();
          Node val1       = nm.mk_value(BitVector::from_ui(num_bits, i));
          Node val2       = nm.mk_value(BitVector::from_ui(num_bits, j));
          ctx.assert_formula(nm.mk_node(kind, {val1, val2}));

          Result sat_res = ctx.solve();
          ASSERT_TRUE(sat_res != Result::UNKNOWN);
          if (sat_res == Result::SAT)
          {
            ASSERT_TRUE(result < 0 || result >= max);
          }
        }
      }
    }
  }

  void s_overflow_test(Kind kind,
                       bool exclude_second_zero,
                       int32_t low,
                       int32_t high,
                       uint32_t rwl)
  {
    assert(low > 0);
    assert(low <= high);

    int32_t result;

    for (int32_t num_bits = low; num_bits <= high; num_bits++)
    {
      int32_t max = std::pow(2, num_bits - 1);
      for (int32_t i = -max; i < max; i++)
      {
        for (int32_t j = -max; j < max; j++)
        {
          if (!exclude_second_zero || j != 0)
          {
            option::Options options;
            options.set<uint64_t>(option::Option::REWRITE_LEVEL, rwl);
            SolvingContext ctx = SolvingContext(options);

            switch (kind)
            {
              case Kind::BV_UADDO:
              case Kind::BV_SADDO: result = add(i, j); break;

              case Kind::BV_USUBO:
              case Kind::BV_SSUBO: result = sub(i, j); break;

              case Kind::BV_UMULO:
              case Kind::BV_SMULO: result = mul(i, j); break;

              default: assert(kind == Kind::BV_SDIVO); result = div(i, j);
            }

            NodeManager& nm = NodeManager::get();
            Node val1       = nm.mk_value(BitVector::from_si(num_bits, i));
            Node val2       = nm.mk_value(BitVector::from_si(num_bits, j));
            ctx.assert_formula(nm.mk_node(kind, {val1, val2}));

            Result sat_res = ctx.solve();
            ASSERT_TRUE(sat_res != Result::UNKNOWN);
            if (sat_res == Result::SAT)
            {
              ASSERT_TRUE(!(result >= -max && result < max));
            }
          }
        }
      }
    }
  }
};

TEST_F(TestRewriterBvOverflow, uaddo)
{
  u_overflow_test(Kind::BV_UADDO, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  u_overflow_test(Kind::BV_UADDO, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

TEST_F(TestRewriterBvOverflow, usubo)
{
  u_overflow_test(Kind::BV_USUBO, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  u_overflow_test(Kind::BV_USUBO, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

TEST_F(TestRewriterBvOverflow, umulo)
{
  u_overflow_test(Kind::BV_UMULO, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  u_overflow_test(Kind::BV_UMULO, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

TEST_F(TestRewriterBvOverflow, saddo)
{
  s_overflow_test(
      Kind::BV_SADDO, false, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  s_overflow_test(
      Kind::BV_SADDO, false, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

TEST_F(TestRewriterBvOverflow, ssubo)
{
  s_overflow_test(
      Kind::BV_SSUBO, false, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  s_overflow_test(
      Kind::BV_SSUBO, false, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

TEST_F(TestRewriterBvOverflow, smulo)
{
  s_overflow_test(
      Kind::BV_SMULO, false, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  s_overflow_test(
      Kind::BV_SMULO, false, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

TEST_F(TestRewriterBvOverflow, sdivo)
{
  s_overflow_test(
      Kind::BV_SDIVO, true, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 1);
  s_overflow_test(
      Kind::BV_SDIVO, true, TEST_OVERFLOW_LOW, TEST_OVERFLOW_HIGH, 0);
}

}  // namespace bzla::test

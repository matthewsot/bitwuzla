#ifndef BZLA_SOLVER_FP_SYMFPU_WRAPPER_H_INCLUDED
#define BZLA_SOLVER_FP_SYMFPU_WRAPPER_H_INCLUDED

#include "solver/fp/floating_point.h"
#include "solver/fp/rounding_mode.h"

extern "C" {
#include "bzlabvstruct.h"
#include "bzlacore.h"
#include "bzlaexp.h"
#include "bzlanode.h"
}

#include <cstdint>

#include "bv/bitvector.h"
#include "node/node.h"
#include "node/node_manager.h"
#include "symfpu/core/ite.h"
#include "symfpu/utils/numberOfRoundingModes.h"

namespace bzla {
namespace fp {

template <bool T>
class SymFpuSymBV;
template <bool T>
class SymFpuSymBVOld;  // TODO: remove with old infrastructure
class WordBlaster;
class WordBlasterOld;  // TODO: remove with old infrastructure

/* ========================================================================== */
/* Glue for SymFPU: concrete.                                                 */
/* ========================================================================== */

/* Mapping between types. */
template <bool T>
struct signedToLiteralType;
template <>
struct signedToLiteralType<true>
{
  using literalType = int32_t;
};
template <>
struct signedToLiteralType<false>
{
  using literalType = uint32_t;
};

/* -------------------------------------------------------------------------- */
/* Wrapper for BitVector.                                                     */
/* -------------------------------------------------------------------------- */

/**
 * The template parameter distinguishes signed and unsigned bit-vectors, a
 * distinction symfpu uses.
 */
template <bool is_signed>
class SymFpuBV
{
  friend SymFpuSymBV<true>;
  friend SymFpuSymBV<false>;
  friend SymFpuSymBVOld<true>;   // TODO: remove with old infrastructure
  friend SymFpuSymBVOld<false>;  // TODO: remove with old infrastructure
  friend WordBlaster;
  friend WordBlasterOld;

 protected:
  using literalType = typename signedToLiteralType<is_signed>::literalType;

  friend SymFpuBV<!is_signed>; /* Allow conversion between the types. */
  friend ::symfpu::ite<bool, SymFpuBV<is_signed> >; /* For ite. */

 public:
  SymFpuBV(const uint32_t bw, const uint32_t val);
  SymFpuBV(const bool &val);
  SymFpuBV(const SymFpuBV<is_signed> &other);
  SymFpuBV(const SymFpuBV<!is_signed> &other);
  SymFpuBV(const BitVector &bv);
  ~SymFpuBV();

  uint32_t getWidth(void) const;
  BitVector *getBv(void) const { return d_bv.get(); }

  static SymFpuBV<is_signed> one(const uint32_t &bw);
  static SymFpuBV<is_signed> zero(const uint32_t &bw);
  static SymFpuBV<is_signed> allOnes(const uint32_t &bw);

  bool isAllOnes() const;
  bool isAllZeros() const;

  static SymFpuBV<is_signed> maxValue(const uint32_t &bw);
  static SymFpuBV<is_signed> minValue(const uint32_t &bw);

  SymFpuBV<is_signed> operator=(const SymFpuBV<is_signed> &other);

  /*** Operators ***/
  SymFpuBV<is_signed> operator<<(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator>>(const SymFpuBV<is_signed> &op) const;

  SymFpuBV<is_signed> operator|(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator&(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator+(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator-(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator*(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator/(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator%(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> operator-(void) const;
  SymFpuBV<is_signed> operator~(void) const;

  SymFpuBV<is_signed> increment() const;
  SymFpuBV<is_signed> decrement() const;
  SymFpuBV<is_signed> signExtendRightShift(const SymFpuBV<is_signed> &op) const;

  /*** Modular operations ***/
  // No overflow checking so these are the same as other operations
  SymFpuBV<is_signed> modularLeftShift(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> modularRightShift(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> modularIncrement() const;
  SymFpuBV<is_signed> modularDecrement() const;
  SymFpuBV<is_signed> modularAdd(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> modularNegate() const;

  /*** Comparisons ***/
  bool operator==(const SymFpuBV<is_signed> &op) const;
  bool operator<=(const SymFpuBV<is_signed> &op) const;
  bool operator>=(const SymFpuBV<is_signed> &op) const;
  bool operator<(const SymFpuBV<is_signed> &op) const;
  bool operator>(const SymFpuBV<is_signed> &op) const;

  /*** Type conversion ***/
  SymFpuBV<true> toSigned(void) const;
  SymFpuBV<false> toUnsigned(void) const;

  /*** Bit hacks ***/
  SymFpuBV<is_signed> extend(uint32_t extension) const;
  SymFpuBV<is_signed> contract(uint32_t reduction) const;
  SymFpuBV<is_signed> resize(uint32_t newSize) const;
  SymFpuBV<is_signed> matchWidth(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> append(const SymFpuBV<is_signed> &op) const;
  SymFpuBV<is_signed> extract(uint32_t upper, uint32_t lower) const;

 private:
  // TODO: This doesn't have to be a pointer
  std::unique_ptr<BitVector> d_bv;
};

/* -------------------------------------------------------------------------- */
/* Template parameter for SymFPU templates.                                   */
/* -------------------------------------------------------------------------- */

class SymFpuTraits
{
 public:
  /* The six key types that SymFPU uses. */
  using bwt  = uint32_t;
  using rm   = RoundingMode;
  using fpt  = FloatingPointTypeInfo;
  using prop = bool;
  using sbv  = SymFpuBV<true>;
  using ubv  = SymFpuBV<false>;

  /* Give concrete instances of each rounding mode, mainly for comparisons. */
  static RoundingMode RNE(void);
  static RoundingMode RNA(void);
  static RoundingMode RTP(void);
  static RoundingMode RTN(void);
  static RoundingMode RTZ(void);

  /* Properties used by Symfpu. */
  static void precondition(const bool &p);
  static void postcondition(const bool &p);
  static void invariant(const bool &p);
};

/* ========================================================================== */
/* Glue for SymFPU: symbolic.                                                 */
/* ========================================================================== */

class SymFpuSymRM;
class SymFpuSymRMOld;  // TODO: remove with old infrastructure
class SymFpuSymProp;
class SymFpuSymPropOld;  // TODO: remove with old infrastructure
template <bool T>
class SymFpuSymBV;
template <bool T>
class SymFpuSymBVOld;  // TODO: remove with old infrastructure

/* Mapping between sorts. */
template <bool T>
struct BzlaSignedToLitSort;
template <>
struct BzlaSignedToLitSort<true>
{
  using BzlaLitSort = int32_t;
};
template <>
struct BzlaSignedToLitSort<false>
{
  using BzlaLitSort = uint32_t;
};

/* -------------------------------------------------------------------------- */
/* Wrapper for propositions.                                                  */
/* -------------------------------------------------------------------------- */

// TODO: remove with old infrastructure
class SymFpuSymPropOld
{
  friend WordBlasterOld;
  friend SymFpuSymBVOld<true>;
  friend SymFpuSymBVOld<false>;
  friend ::symfpu::ite<SymFpuSymPropOld, SymFpuSymPropOld>;

 public:
  SymFpuSymPropOld(BzlaNode *node);
  SymFpuSymPropOld(bool v);
  SymFpuSymPropOld(const SymFpuSymPropOld &other);
  ~SymFpuSymPropOld();

  BzlaNode *getNode() const { return d_node; }

  SymFpuSymPropOld &operator=(const SymFpuSymPropOld &other);

  SymFpuSymPropOld operator!(void) const;
  SymFpuSymPropOld operator&&(const SymFpuSymPropOld &op) const;
  SymFpuSymPropOld operator||(const SymFpuSymPropOld &op) const;
  SymFpuSymPropOld operator==(const SymFpuSymPropOld &op) const;
  SymFpuSymPropOld operator^(const SymFpuSymPropOld &op) const;

 protected:
  bool check_node(const BzlaNode *node) const;

 private:
  static inline Bzla *s_bzla = nullptr;
  BzlaNode *d_node;
};

class SymFpuSymProp
{
  friend WordBlaster;
  friend SymFpuSymBV<true>;
  friend SymFpuSymBV<false>;
  friend ::symfpu::ite<SymFpuSymProp, SymFpuSymProp>;

 public:
  SymFpuSymProp(const Node &node);
  SymFpuSymProp(bool v);
  SymFpuSymProp(const SymFpuSymProp &other);
  ~SymFpuSymProp();

  const Node &getNode() const { return d_node; }

  SymFpuSymProp &operator=(const SymFpuSymProp &other);

  SymFpuSymProp operator!(void) const;
  SymFpuSymProp operator&&(const SymFpuSymProp &op) const;
  SymFpuSymProp operator||(const SymFpuSymProp &op) const;
  SymFpuSymProp operator==(const SymFpuSymProp &op) const;
  SymFpuSymProp operator^(const SymFpuSymProp &op) const;

 protected:
  bool check_node(const Node &node) const;

 private:
  Node d_node;
};

/* -------------------------------------------------------------------------- */
/* Wrapper for bit-vector terms.                                              */
/* -------------------------------------------------------------------------- */

// TODO: remove with old infrastructure
template <bool is_signed>
class SymFpuSymBVOld;

// TODO: remove with old infrastructure
template <bool is_signed>
class SymFpuSymBVOld
{
  friend WordBlasterOld;
  friend SymFpuSymBVOld<!is_signed>; /* Allow conversion between the sorts. */
  friend ::symfpu::ite<SymFpuSymPropOld, SymFpuSymBVOld<is_signed> >;

 public:
  /** Constructors for bit-vector nodes. */
  SymFpuSymBVOld(BzlaNode *node);
  SymFpuSymBVOld(const uint32_t w, const uint32_t val);
  SymFpuSymBVOld(const SymFpuSymPropOld &p);
  SymFpuSymBVOld(const SymFpuSymBVOld<is_signed> &other);
  SymFpuSymBVOld(const SymFpuSymBVOld<!is_signed> &other);
  SymFpuSymBVOld(const BitVector &bv);
  SymFpuSymBVOld(const SymFpuBV<is_signed> &bv);
  /** Constructors for Boolean nodes. */
  SymFpuSymBVOld(bool v);
  /** Destructor. */
  ~SymFpuSymBVOld();

  SymFpuSymBVOld<is_signed> &operator=(const SymFpuSymBVOld<is_signed> &other);

  uint32_t getWidth(void) const;
  BzlaNode *getNode(void) const { return d_node; }

  /** Constant creation and test */
  static SymFpuSymBVOld<is_signed> one(const uint32_t &w);
  static SymFpuSymBVOld<is_signed> zero(const uint32_t &w);
  static SymFpuSymBVOld<is_signed> allOnes(const uint32_t &w);

  SymFpuSymPropOld isAllOnes() const;
  SymFpuSymPropOld isAllZeros() const;

  static SymFpuSymBVOld<is_signed> maxValue(const uint32_t &w);
  static SymFpuSymBVOld<is_signed> minValue(const uint32_t &w);

  /** Operators */
  SymFpuSymBVOld<is_signed> operator<<(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator>>(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator|(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator&(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator+(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator-(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator*(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator/(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator%(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> operator-(void) const;
  SymFpuSymBVOld<is_signed> operator~(void) const;
  SymFpuSymBVOld<is_signed> increment() const;
  SymFpuSymBVOld<is_signed> decrement() const;
  SymFpuSymBVOld<is_signed> signExtendRightShift(
      const SymFpuSymBVOld<is_signed> &op) const;

  /** Modular operations */
  // This back-end doesn't do any overflow checking so these are the same as
  // other operations
  SymFpuSymBVOld<is_signed> modularLeftShift(
      const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> modularRightShift(
      const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> modularIncrement() const;
  SymFpuSymBVOld<is_signed> modularDecrement() const;
  SymFpuSymBVOld<is_signed> modularAdd(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> modularNegate() const;

  /** Operators for Boolean nodes */
  SymFpuSymPropOld operator!(void) const;
  SymFpuSymPropOld operator&&(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymPropOld operator||(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymPropOld operator^(const SymFpuSymBVOld<is_signed> &op) const;

  /** Comparisons */
  SymFpuSymPropOld operator==(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymPropOld operator<=(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymPropOld operator>=(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymPropOld operator<(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymPropOld operator>(const SymFpuSymBVOld<is_signed> &op) const;

  /** Type conversion */
  // Bitwuzla nodes make no distinction between signed and unsigned, thus these
  // are trivial
  SymFpuSymBVOld<true> toSigned(void) const;
  SymFpuSymBVOld<false> toUnsigned(void) const;

  /** Bit hacks */
  SymFpuSymBVOld<is_signed> extend(uint32_t extension) const;
  SymFpuSymBVOld<is_signed> contract(uint32_t reduction) const;
  SymFpuSymBVOld<is_signed> resize(uint32_t newSize) const;
  SymFpuSymBVOld<is_signed> matchWidth(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> append(const SymFpuSymBVOld<is_signed> &op) const;
  SymFpuSymBVOld<is_signed> extract(uint32_t upper, uint32_t lower) const;

 protected:
  using literalType = typename BzlaSignedToLitSort<is_signed>::BzlaLitSort;

  // BzlaNode* boolNodeToBV(BzlaNode* node) const;
  // BzlaNode* BVToBoolNode(BzlaNode* node) const;

  bool check_node(const BzlaNode *node) const;
  bool checkBooleanNode(const BzlaNode *node) const;
  // BzlaNode *fromProposition (BzlaNode *node) const;
  // BzlaNode *toProposition (BzlaNode *node) const;

 private:
  static inline Bzla *s_bzla = nullptr;
  BzlaNode *d_node;
};

template <bool is_signed>
class SymFpuSymBV;

template <bool is_signed>
class SymFpuSymBV
{
  friend WordBlaster;
  friend SymFpuSymBV<!is_signed>; /* Allow conversion between the sorts. */
  friend ::symfpu::ite<SymFpuSymProp, SymFpuSymBV<is_signed> >;

 public:
  /** Constructors for bit-vector nodes. */
  SymFpuSymBV(const Node &node);
  SymFpuSymBV(const uint32_t w, const uint32_t val);
  SymFpuSymBV(const SymFpuSymProp &p);
  SymFpuSymBV(const SymFpuSymBV<is_signed> &other);
  SymFpuSymBV(const SymFpuSymBV<!is_signed> &other);
  SymFpuSymBV(const BitVector &bv);
  SymFpuSymBV(const SymFpuBV<is_signed> &bv);
  /** Construrs for Boolean nodes. */
  SymFpuSymBV(bool v);
  /** Destructor. */
  ~SymFpuSymBV();

  SymFpuSymBV<is_signed> &operator=(const SymFpuSymBV<is_signed> &other);

  uint32_t getWidth(void) const;
  const Node &getNode(void) const { return d_node; }

  /** Constant creation and test */
  static SymFpuSymBV<is_signed> one(const uint32_t &w);
  static SymFpuSymBV<is_signed> zero(const uint32_t &w);
  static SymFpuSymBV<is_signed> allOnes(const uint32_t &w);

  SymFpuSymProp isAllOnes() const;
  SymFpuSymProp isAllZeros() const;

  static SymFpuSymBV<is_signed> maxValue(const uint32_t &w);
  static SymFpuSymBV<is_signed> minValue(const uint32_t &w);

  /** Operators */
  SymFpuSymBV<is_signed> operator<<(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator>>(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator|(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator&(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator+(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator-(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator*(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator/(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator%(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> operator-(void) const;
  SymFpuSymBV<is_signed> operator~(void) const;
  SymFpuSymBV<is_signed> increment() const;
  SymFpuSymBV<is_signed> decrement() const;
  SymFpuSymBV<is_signed> signExtendRightShift(
      const SymFpuSymBV<is_signed> &op) const;

  /** Modular operations */
  // This back-end doesn't do any overflow checking so these are the same as
  // other operations
  SymFpuSymBV<is_signed> modularLeftShift(
      const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> modularRightShift(
      const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> modularIncrement() const;
  SymFpuSymBV<is_signed> modularDecrement() const;
  SymFpuSymBV<is_signed> modularAdd(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> modularNegate() const;

  /** Operators for Boolean nodes */
  SymFpuSymProp operator!(void) const;
  SymFpuSymProp operator&&(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymProp operator||(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymProp operator^(const SymFpuSymBV<is_signed> &op) const;

  /** Comparisons */
  SymFpuSymProp operator==(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymProp operator<=(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymProp operator>=(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymProp operator<(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymProp operator>(const SymFpuSymBV<is_signed> &op) const;

  /** Type conversion */
  // Bitwuzla nodes make no distinction between signed and unsigned, thus these
  // are trivial
  SymFpuSymBV<true> toSigned(void) const;
  SymFpuSymBV<false> toUnsigned(void) const;

  /** Bit hacks */
  SymFpuSymBV<is_signed> extend(uint32_t extension) const;
  SymFpuSymBV<is_signed> contract(uint32_t reduction) const;
  SymFpuSymBV<is_signed> resize(uint32_t newSize) const;
  SymFpuSymBV<is_signed> matchWidth(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> append(const SymFpuSymBV<is_signed> &op) const;
  SymFpuSymBV<is_signed> extract(uint32_t upper, uint32_t lower) const;

 protected:
  using literalType = typename BzlaSignedToLitSort<is_signed>::BzlaLitSort;

  // BzlaNode* boolNodeToBV(BzlaNode* node) const;
  // BzlaNode* BVToBoolNode(BzlaNode* node) const;

  bool check_node(const Node &node) const;
  bool check_bool_node(const Node &node) const;
  // BzlaNode *fromProposition (BzlaNode *node) const;
  // BzlaNode *toProposition (BzlaNode *node) const;

 private:
  Node d_node;
};
/* -------------------------------------------------------------------------- */
/* Wrapper for rounding modes.                                                */
/* -------------------------------------------------------------------------- */

// TODO: remove with old infrastructure
class SymFpuSymRMOld
{
  friend WordBlasterOld;
  friend symfpu::ite<SymFpuSymPropOld, SymFpuSymRMOld>;

 public:
  /* Constructors. */
  SymFpuSymRMOld(BzlaNode *node);
  SymFpuSymRMOld(const RoundingMode rm);
  SymFpuSymRMOld(const SymFpuSymRMOld &other);
  /* Destructor. */
  ~SymFpuSymRMOld();

  BzlaNode *getNode() const { return d_node; }

  SymFpuSymPropOld valid(void) const;
  SymFpuSymPropOld operator==(const SymFpuSymRMOld &other) const;

 protected:
  bool check_node(const BzlaNode *node) const;

 private:
  static inline Bzla *s_bzla = nullptr;
  BzlaNode *init_const(const RoundingMode rm);
  BzlaNode *d_node;
};

class SymFpuSymRM
{
  friend WordBlaster;
  friend symfpu::ite<SymFpuSymProp, SymFpuSymRM>;

 public:
  /* Constructors. */
  SymFpuSymRM(const Node &node);
  SymFpuSymRM(const RoundingMode rm);
  SymFpuSymRM(const SymFpuSymRM &other);
  /* Destructor. */
  ~SymFpuSymRM();

  const Node &getNode() const { return d_node; }

  SymFpuSymProp valid(void) const;
  SymFpuSymProp operator==(const SymFpuSymRM &other) const;

 protected:
  bool check_node(const Node &node) const;

 private:
  Node mk_value(const RoundingMode rm);
  Node d_node;
};

/* -------------------------------------------------------------------------- */
/* Template parameter for SymFPU templates.                                   */
/* -------------------------------------------------------------------------- */

// TODO: remove with old infrastructure
class SymFpuSymTraitsOld
{
 public:
  /* The six key types that SymFPU uses. */
  using bwt  = uint32_t;
  using rm   = SymFpuSymRMOld;
  using fpt  = FloatingPointTypeInfo;
  using prop = SymFpuSymPropOld;
  using sbv  = SymFpuSymBVOld<true>;
  using ubv  = SymFpuSymBVOld<false>;

  /* Give concrete instances (wrapped nodes) for each rounding mode. */
  static SymFpuSymRMOld RNE(void);
  static SymFpuSymRMOld RNA(void);
  static SymFpuSymRMOld RTP(void);
  static SymFpuSymRMOld RTN(void);
  static SymFpuSymRMOld RTZ(void);

  /* Properties used by Symfpu. */
  static void precondition(const bool b);
  static void postcondition(const bool b);
  static void invariant(const bool b);
  static void precondition(const prop &p);
  static void postcondition(const prop &p);
  static void invariant(const prop &p);
};

class SymFpuSymTraits
{
 public:
  /* The six key types that SymFPU uses. */
  using bwt  = uint32_t;
  using rm   = SymFpuSymRM;
  using fpt  = FloatingPointTypeInfo;
  using prop = SymFpuSymProp;
  using sbv  = SymFpuSymBV<true>;
  using ubv  = SymFpuSymBV<false>;

  /* Give concrete instances (wrapped nodes) for each rounding mode. */
  static SymFpuSymRM RNE(void);
  static SymFpuSymRM RNA(void);
  static SymFpuSymRM RTP(void);
  static SymFpuSymRM RTN(void);
  static SymFpuSymRM RTZ(void);

  /* Properties used by Symfpu. */
  static void precondition(const bool b);
  static void postcondition(const bool b);
  static void invariant(const bool b);
  static void precondition(const prop &p);
  static void postcondition(const prop &p);
  static void invariant(const prop &p);
};
/* -------------------------------------------------------------------------- */

}  // namespace fp
}  // namespace bzla

/* ========================================================================== */
/* ITE specializations.                                                       */
/* ========================================================================== */

namespace symfpu {

#define BZLA_FP_ITE(T)                                              \
  template <>                                                       \
  struct ite<bool, T>                                               \
  {                                                                 \
    static const T &iteOp(const bool &_c, const T &_t, const T &_e) \
    {                                                               \
      return _c ? _t : _e;                                          \
    }                                                               \
  };
BZLA_FP_ITE(bzla::fp::SymFpuTraits::rm);
BZLA_FP_ITE(bzla::fp::SymFpuTraits::prop);
BZLA_FP_ITE(bzla::fp::SymFpuTraits::sbv);
BZLA_FP_ITE(bzla::fp::SymFpuTraits::ubv);
#undef BZLA_FP_ITE

#define BZLA_FP_SYM_ITE(S, T)                                                 \
  template <>                                                                 \
  struct ite<bzla::fp::SymFpuSymPropOld, S>                                   \
  {                                                                           \
    static const S iteOp(const bzla::fp::SymFpuSymPropOld &_c,                \
                         const S &_t,                                         \
                         const S &_e)                                         \
    {                                                                         \
      BzlaNode *c = _c.getNode();                                             \
      BzlaNode *t = _t.getNode();                                             \
      BzlaNode *e = _e.getNode();                                             \
      assert(c);                                                              \
      assert(t);                                                              \
      assert(e);                                                              \
      Bzla *bzla = S::s_bzla;                                                 \
      assert(bzla);                                                           \
      assert(bzla == bzla_node_real_addr(c)->bzla);                           \
      assert(bzla == bzla_node_real_addr(t)->bzla);                           \
      assert(bzla == bzla_node_real_addr(e)->bzla);                           \
      BzlaNode *ite = bzla_exp_cond(bzla, c, t, e);                           \
      S res(ite);                                                             \
      bzla_node_release(bzla, ite);                                           \
      return res;                                                             \
    }                                                                         \
  };                                                                          \
  template <>                                                                 \
  struct ite<bzla::fp::SymFpuSymProp, T>                                      \
  {                                                                           \
    static const T iteOp(const bzla::fp::SymFpuSymProp &_c,                   \
                         const T &_t,                                         \
                         const T &_e)                                         \
    {                                                                         \
      assert(!_c.getNode().is_null());                                        \
      assert(!_t.getNode().is_null());                                        \
      assert(!_e.getNode().is_null());                                        \
      return bzla::NodeManager::get().mk_node(                                \
          bzla::node::Kind::ITE, {_c.getNode(), _t.getNode(), _e.getNode()}); \
    }                                                                         \
  };
BZLA_FP_SYM_ITE(bzla::fp::SymFpuSymTraitsOld::rm,
                bzla::fp::SymFpuSymTraits::rm);
BZLA_FP_SYM_ITE(bzla::fp::SymFpuSymTraitsOld::prop,
                bzla::fp::SymFpuSymTraits::prop);
BZLA_FP_SYM_ITE(bzla::fp::SymFpuSymTraitsOld::sbv,
                bzla::fp::SymFpuSymTraits::sbv);
BZLA_FP_SYM_ITE(bzla::fp::SymFpuSymTraitsOld::ubv,
                bzla::fp::SymFpuSymTraits::ubv);
#undef BZLA_FP_SYM_ITE
}  // namespace symfpu

/* -------------------------------------------------------------------------- */
#endif

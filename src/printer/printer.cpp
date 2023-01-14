#include "printer/printer.h"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "bv/bitvector.h"
#include "node/kind_info.h"
#include "node/node_ref_vector.h"
#include "node/unordered_node_ref_map.h"
#include "solver/fp/floating_point.h"
#include "solver/fp/rounding_mode.h"

namespace bzla {

using namespace node;

/* --- Printer public ------------------------------------------------------- */

int32_t Printer::s_stream_index_maximum_depth = std::ios_base::xalloc();

void
Printer::print(std::ostream& os, const Node& node)
{
  size_t depth = os.iword(Printer::s_stream_index_maximum_depth);
  unordered_node_ref_map<std::string> let_map;
  bool annotate = depth && node.num_children() > 0;
  if (annotate)
  {
    os << "(!@t" << node.id() << " ";
  }
  letify(os, node, let_map, depth);
  if (annotate)
  {
    os << ")";
  }
}

void
Printer::print(std::ostream& os, const Type& type)
{
  if (type.is_bool())
  {
    os << "Bool";
  }
  else if (type.is_bv())
  {
    os << "(_ BitVec " << type.bv_size() << ")";
  }
  else if (type.is_fp())
  {
    os << "(_ FloatingPoint " << type.fp_exp_size() << " " << type.fp_sig_size()
       << ")";
  }
  else if (type.is_rm())
  {
    os << "RoundingMode";
  }
  else if (type.is_array())
  {
    os << "(Array ";
    print(os, type.array_index());
    os << " ";
    print(os, type.array_element());
    os << ")";
  }
  else if (type.is_uninterpreted())
  {
    const std::optional<std::string>& symbol = type.uninterpreted_symbol();
    os << (symbol ? *symbol : "@bzla.sort" + std::to_string(type.id()));
  }
  else if (type.is_fun())
  {
    const auto& types = type.fun_types();
    size_t n          = types.size();
    for (size_t i = 0; i < n - 1; ++i)
    {
      os << types[i] << " ";
    }
    os << "-> " << types[n - 1];
  }
  else
  {
    assert(false);
  }
}

/* --- Printer private ------------------------------------------------------ */

void
Printer::print(std::ostream& os,
               const Node& node,
               node::unordered_node_ref_map<std::string>& let_map,
               size_t max_depth)
{
  std::vector<std::pair<ConstNodeRef, size_t>> visit;
  visit.emplace_back(node, 0);
  node::unordered_node_ref_map<bool> cache;
  bool expect_space = false;
  do
  {
    const auto& p    = visit.back();
    const Node& cur  = p.first;
    size_t cur_depth = p.second;

    auto [it, inserted] = cache.emplace(cur, false);
    if (inserted)
    {
      Kind kind = cur.kind();
      if (kind == Kind::VALUE || kind == Kind::CONSTANT
          || kind == Kind::VARIABLE)
      {
        it->second = true;
        continue;
      }

      // Stop at maximum depth
      if (max_depth && cur_depth >= max_depth)
      {
        it->second = true;
        continue;
      }

      auto lit = let_map.find(cur);
      if (lit != let_map.end())
      {
        it->second = true;
        continue;
      }

      for (size_t i = 0, size = cur.num_children(); i < size; ++i)
      {
        visit.emplace_back(cur[size - 1 - i], cur_depth + 1);
      }

      if (expect_space)
      {
        os << " ";
      }
      expect_space = true;

      const char* symbol = KindInfo::smt2_name(cur.kind());
      switch (cur.kind())
      {
        case Kind::CONST_ARRAY:
          os << "((as const ";
          Printer::print(os, cur.type());
          os << ")";
          break;

        case Kind::NOT:
        case Kind::AND:
        case Kind::OR:
        case Kind::IMPLIES:
        case Kind::XOR:
        case Kind::DISTINCT:
        case Kind::EQUAL:
        case Kind::ITE:
        case Kind::BV_ADD:
        case Kind::BV_AND:
        case Kind::BV_ASHR:
        case Kind::BV_COMP:
        case Kind::BV_CONCAT:
        case Kind::BV_DEC:
        case Kind::BV_INC:
        case Kind::BV_MUL:
        case Kind::BV_NAND:
        case Kind::BV_NEG:
        case Kind::BV_NOR:
        case Kind::BV_NOT:
        case Kind::BV_OR:
        case Kind::BV_REDAND:
        case Kind::BV_REDOR:
        case Kind::BV_REDXOR:
        case Kind::BV_ROL:
        case Kind::BV_ROR:
        case Kind::BV_SADDO:
        case Kind::BV_SDIV:
        case Kind::BV_SDIVO:
        case Kind::BV_SGE:
        case Kind::BV_SGT:
        case Kind::BV_SHL:
        case Kind::BV_SHR:
        case Kind::BV_SLE:
        case Kind::BV_SLT:
        case Kind::BV_SMOD:
        case Kind::BV_SMULO:
        case Kind::BV_SREM:
        case Kind::BV_SSUBO:
        case Kind::BV_SUB:
        case Kind::BV_UADDO:
        case Kind::BV_UDIV:
        case Kind::BV_UGE:
        case Kind::BV_UGT:
        case Kind::BV_ULE:
        case Kind::BV_ULT:
        case Kind::BV_UMULO:
        case Kind::BV_UREM:
        case Kind::BV_USUBO:
        case Kind::BV_XNOR:
        case Kind::BV_XOR:
        case Kind::FP_ABS:
        case Kind::FP_ADD:
        case Kind::FP_DIV:
        case Kind::FP_EQUAL:
        case Kind::FP_FMA:
        case Kind::FP_FP:
        case Kind::FP_GEQ:
        case Kind::FP_GT:
        case Kind::FP_IS_INF:
        case Kind::FP_IS_NAN:
        case Kind::FP_IS_NEG:
        case Kind::FP_IS_NORMAL:
        case Kind::FP_IS_POS:
        case Kind::FP_IS_SUBNORMAL:
        case Kind::FP_IS_ZERO:
        case Kind::FP_LEQ:
        case Kind::FP_LT:
        case Kind::FP_MAX:
        case Kind::FP_MIN:
        case Kind::FP_MUL:
        case Kind::FP_NEG:
        case Kind::FP_REM:
        case Kind::FP_RTI:
        case Kind::FP_SQRT:
        case Kind::FP_SUB:
        case Kind::SELECT:
        case Kind::STORE: os << "(" << symbol; break;

        case Kind::BV_EXTRACT:
        case Kind::BV_REPEAT:
        case Kind::BV_ROLI:
        case Kind::BV_RORI:
        case Kind::BV_SIGN_EXTEND:
        case Kind::BV_ZERO_EXTEND:
        case Kind::FP_TO_FP_FROM_BV:
        case Kind::FP_TO_FP_FROM_FP:
        case Kind::FP_TO_FP_FROM_SBV:
        case Kind::FP_TO_FP_FROM_UBV:
        case Kind::FP_TO_SBV:
        case Kind::FP_TO_UBV:
          os << "((_ " << symbol;
          for (size_t i = 0; i < cur.num_indices(); ++i)
          {
            os << " " << cur.index(i);
          }
          os << ")";
          break;

        case Kind::APPLY:
          os << "(";
          expect_space = false;
          break;

        case Kind::EXISTS:
        case Kind::FORALL:
        case Kind::LAMBDA:
          os << "(" << symbol << " ((";
          print_symbol(os, cur[0]);
          os << " ";
          Printer::print(os, cur[0].type());
          os << ")) ";
          visit.pop_back();  // Pop variable
          visit.pop_back();  // Pop body
          letify(os, cur[1], let_map, max_depth);
          break;

        case Kind::VALUE:
        case Kind::CONSTANT:
        case Kind::VARIABLE:
        case Kind::NULL_NODE:
        case Kind::NUM_KINDS: assert(false); break;
      }
      continue;
    }
    else if (!it->second)
    {
      os << ")";
      it->second = true;
    }
    else
    {
      if (expect_space)
      {
        os << " ";
      }
      expect_space = true;

      const Type& type = cur.type();
      auto kind        = cur.kind();
      if (kind == Kind::VALUE)
      {
        if (type.is_bool())
        {
          os << (cur.value<bool>() ? "true" : "false");
        }
        else if (type.is_bv())
        {
          os << "#b" << cur.value<BitVector>();
        }
        else if (type.is_fp())
        {
          os << cur.value<FloatingPoint>();
        }
        else if (type.is_rm())
        {
          os << cur.value<RoundingMode>();
        }
        else
        {
          assert(false);
        }
      }
      else if (kind == Kind::CONSTANT || kind == Kind::VARIABLE)
      {
        print_symbol(os, cur);
      }
      else if (max_depth && cur_depth >= max_depth)
      {
        os << "@t" << cur.id();
      }
      else
      {
        auto lit = let_map.find(cur);
        assert(lit != let_map.end());
        os << lit->second;
      }
    }
    visit.pop_back();
  } while (!visit.empty());
}

void
Printer::print_symbol(std::ostream& os, const Node& node)
{
  const auto symbol = node.symbol();
  if (symbol)
  {
    os << symbol->get();
  }
  // Default symbol
  else
  {
    os << (node.kind() == Kind::CONSTANT ? "@bzla.const" : "@bzla.var");
    os << "_" << node.id();
  }
}

void
Printer::letify(std::ostream& os,
                const Node& node,
                node::unordered_node_ref_map<std::string>& let_map,
                size_t max_depth)
{
  node::node_ref_vector visit{node}, lets;
  std::vector<size_t> depth{0};
  node::unordered_node_ref_map<bool> cache;
  node::unordered_node_ref_map<uint64_t> refs;

  // Find nodes that need to be letified (i.e., referenced more than once)
  node::node_ref_vector binders;
  do
  {
    const Node& cur = visit.back();
    visit.pop_back();
    size_t cur_depth = depth.back();
    depth.pop_back();

    // Do not go below binders
    auto kind = cur.kind();
    if (kind == Kind::FORALL || kind == Kind::EXISTS || kind == Kind::LAMBDA)
    {
      binders.push_back(cur);
      continue;
    }

    // Do not go further than the maximum specified depth.
    if (max_depth > 0 && cur_depth >= max_depth)
    {
      continue;
    }

    auto [it, inserted] = cache.emplace(cur, false);
    if (inserted)
    {
      for (size_t i = 0, size = cur.num_children(); i < size; ++i)
      {
        visit.push_back(cur[i]);
        depth.push_back(cur_depth + 1);
        ++refs[cur[i]];
        if (refs[cur[i]] == 2 && cur[i].num_children() > 0)
        {
          lets.push_back(cur[i]);
        }
      }
    }
  } while (!visit.empty());

  while (!binders.empty())
  {
    const Node& cur = binders.back();
    binders.pop_back();

    auto [it, inserted] = cache.emplace(cur, false);
    if (inserted)
    {
      for (size_t i = 0, size = cur.num_children(); i < size; ++i)
      {
        binders.push_back(cur[i]);
        auto itr = refs.find(cur[i]);
        if (itr == refs.end())
        {
          continue;
        }
        ++itr->second;
        if (itr->second == 2 && cur[i].num_children() > 0)
        {
          lets.push_back(cur[i]);
        }
      }
    }
  }

  // Sort letified nodes by id in ascending order
  std::sort(lets.begin(), lets.end(), [](const Node& a, const Node& b) {
    return a.id() < b.id();
  });
  if (lets.size() > 0)
  {
    os << "(let (";
    for (size_t i = 0; i < lets.size(); ++i)
    {
      if (i > 0)
      {
        os << " ";
      }

      // Construct symbol of let
      std::stringstream ss;
      ss << "_let" << i;

      os << "(" << ss.str() << " ";
      print(os, lets[i], let_map, max_depth);
      os << ")";

      let_map[lets[i]] = ss.str();
    }
    os << ") ";
  }

  print(os, node, let_map, max_depth);

  if (lets.size() > 0)
  {
    os << ")";
  }
}

namespace printer {

std::ostream&
operator<<(std::ostream& ostream, const set_depth& d)
{
  ostream.iword(Printer::s_stream_index_maximum_depth) = d.depth();
  return ostream;
}

}  // namespace printer

}  // namespace bzla

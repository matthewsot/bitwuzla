#ifndef BZLA_OPTION_OPTION_H_INCLUDED
#define BZLA_OPTION_OPTION_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace bzla::option {

/* -------------------------------------------------------------------------- */

enum class Option
{
  INCREMENTAL,          // bool
  LOG_LEVEL,            // numeric
  PRODUCE_MODELS,       // bool
  PRODUCE_UNSAT_CORES,  // bool
  SAT_SOLVER,           // enum
  SEED,                 // numeric
  VERBOSITY,            // numeric

  BV_SOLVER,      // enum
  SMT_COMP_MODE,  // bool

  PROP_NPROPS,                  // numeric
  PROP_NUPDATES,                // numeric
  PROP_PATH_SEL,                // enum
  PROP_PROB_PICK_INV_VALUE,     // numeric
  PROP_PROB_PICK_RANDOM_INPUT,  // numeric
  PROP_CONST_BITS,              // bool
  PROP_INEQ_BOUNDS,             // bool
  PROP_OPT_LT_CONCAT_SEXT,      // bool
  PROP_SEXT,                    // bool

  NUM_OPTIONS,
};

/* -------------------------------------------------------------------------- */

enum class BvSolver
{
  BITBLAST,
  PROP,
  PREPROP,
};

enum class SatSolver
{
  CADICAL,
  CRYPTOMINISAT,
  KISSAT,
  LINGELING,
};

enum class PropPathSelection
{
  ESSENTIAL,
  RANDOM,
};

/* -------------------------------------------------------------------------- */

class Options;

/** The base class for option info data. */
class OptionBase
{
  friend Options;

 public:
  /**
   * Constructor.
   * @param options   The associated options object.
   * @param opt       The corresponding option.
   * @param desc      The option description (used for CLI help message).
   * @param lng       The long name of the option (`--<lng>` for the CLI).
   * @param shrt      The short name of the option (`-<shrt>` for the CLI).
   * @param is_expert True if this is an expert option.
   */
  OptionBase(Options* options,
             Option opt,
             const char* desc,
             const char* lng,
             const char* shrt = nullptr,
             bool is_expert   = false);
  OptionBase() = delete;
  virtual ~OptionBase();

  /** @return True if this option is a Boolean option. */
  virtual bool is_bool() const { return false; }
  /** @return True if this option is a numeric option. */
  virtual bool is_numeric() const { return false; }
  /** @return True if this option is an option that takes an enum value. */
  virtual bool is_enum() const { return false; }

  /** @return The description of this option. */
  const char* description() const { return d_description; }
  /** @return The long name of this option. */
  const char* lng() const { return d_long; }
  /** @return The short name of this option. */
  const char* shrt() const { return d_short; }

 protected:
  /** The option description. */
  const char* d_description;
  /** The long name of the option (`--<lng>` in the CLI). */
  const char* d_long;
  /** The short name of the option (`-<shrt>` in the CLI). */
  const char* d_short;
  /** True if this is an expert option. */
  bool d_is_expert;
};

/** Option info data for Boolean options. */
class OptionBool : public OptionBase
{
 public:
  /**
   * Constructor.
   *
   * @note On construction, given Boolean value determines the initial and the
   *       default value of the option.
   *
   * @param options   The associated options object.
   * @param opt       The corresponding option.
   * @param value     The initial and default value of the option.
   * @param desc      The option description (used for CLI help message).
   * @param lng       The long name of the option (`--<lng>` for the CLI).
   * @param shrt      The short name of the option (`-<shrt>` for the CLI).
   * @param is_expert True if this is an expert option.
   */
  OptionBool(Options* options,
             Option opt,
             bool value,
             const char* desc,
             const char* lng,
             const char* shrt = nullptr,
             bool is_expert   = false)
      : OptionBase(options, opt, desc, lng, shrt, is_expert),
        d_value(value),
        d_default(value)
  {
  }
  OptionBool() = delete;

  bool is_bool() const override { return true; }

  /**
   * Set the current value of a Boolean option.
   * @param value The current value.
   */
  void set(bool value) { d_value = value; }

  /**
   * Get the current value of a Boolean option.
   * @return The current value of a Boolean option.
   */
  const bool& operator()() const { return d_value; }

  /** @return The default value of this option. */
  const bool& dflt() const { return d_default; }

 private:
  /** The current value. */
  bool d_value;
  /** The default value. */
  bool d_default;
};

/** Option info data for numeric options. */
class OptionNumeric : public OptionBase
{
 public:
  /**
   * Constructor.
   *
   * @note On construction, given uint64_t value determines the initial and the
   *       default integer value of a numeric option.
   *
   * @param options   The associated options object.
   * @param opt       The corresponding option.
   * @param value     The initial and default value of the option.
   * @param min       The minimum value of the option.
   * @param max       The maximum value of the option.
   * @param desc      The option description (used for the CLI help message).
   * @param lng       The long name of the option (`--<lng>` in the CLI).
   * @param shrt      The short name of the option (`-<shrt>` in the CLI).
   * @param is_expert True if this is an expert option.
   */
  OptionNumeric(Options* options,
                Option opt,
                uint64_t value,
                uint64_t min,
                uint64_t max,
                const char* desc,
                const char* lng,
                const char* shrt = nullptr,
                bool is_expert   = false)
      : OptionBase(options, opt, desc, lng, shrt, is_expert),
        d_value(value),
        d_default(value),
        d_min(min),
        d_max(max)
  {
  }
  OptionNumeric() = delete;

  bool is_numeric() const override { return true; }

  /**
   * Set the current value of a numeric option.
   * @param value The current value.
   */
  void set(uint64_t value) { d_value = value; }

  /**
   * Get the current value of a numeric option.
   * @return The current value of a numeric option.
   */
  const uint64_t& operator()() const { return d_value; }

  /** @return The default value of this option. */
  const uint64_t& dflt() const { return d_default; }
  /** @return The minimum value of this option. */
  const uint64_t& min() const { return d_min; }
  /** @return The maximum value of this option. */
  const uint64_t& max() const { return d_max; }

 private:
  /** The current value. */
  uint64_t d_value;
  /** The default value. */
  uint64_t d_default;
  /** The minimum value. */
  uint64_t d_min;
  /** The maximum value. */
  uint64_t d_max;
};

/** Base class for option info data for options that take enum values. */
class OptionEnum : public OptionBase
{
 public:
  OptionEnum(Options* options,
             Option opt,
             const char* desc,
             const char* lng,
             const char* shrt = nullptr,
             bool is_expert   = false)
      : OptionBase(options, opt, desc, lng, shrt, is_expert)
  {
  }

  OptionEnum() = delete;

  bool is_enum() const override { return true; }

  /**
   * Set current value of enum option.
   * @param value The string representation of the enum value.
   */
  virtual void set_str(const std::string& value) = 0;
  /**
   * Get the string representation of the current value of an enum option.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @return The current value of an enum option.
   */
  virtual const std::string& get_str() const = 0;
  /**
   * Get the string representation of the default value of an enum option.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @return The default value of an enum option.
   */
  virtual const std::string& dflt_str() const = 0;
  /**
   * Determine if the given string is a valid mode for an enum option.
   * @param value The mode.
   * @return True if it is valid.
   */
  virtual bool is_valid(const std::string& value) const = 0;
  /** @return A vector of string representations of available modes. */
  virtual std::vector<std::string> modes() const = 0;
};

/** Option info data for options that take enum values. */
template <typename T>
class OptionEnumT : public OptionEnum
{
  using String2EnumMap = std::unordered_map<std::string, T>;
  using Enum2StringMap = std::unordered_map<T, std::string>;

 public:
  /**
   * Constructor.
   *
   * @note On construction, given uint64_t value determines the initial and the
   *       default enum value of the option.
   *
   * @param options     The associated options object.
   * @param opt         The corresponding option.
   * @param value       The initial and default value of the option.
   * @param enum2string A map from option enum value to its string
   *                    representation for the CLI.
   * @param desc        The option description (used for the CLI help message).
   * @param lng         The long name of the option (`--<lng>` in the CLI).
   * @param shrt        The short name of the option (`-<shrt>` in the CLI).
   * @param is_expert   True if this is an expert option.
   */
  OptionEnumT(Options* options,
              Option opt,
              T value,
              const Enum2StringMap& enum2string,
              const char* desc,
              const char* lng,
              const char* shrt = nullptr,
              bool is_expert   = false)
      : OptionEnum(options, opt, desc, lng, shrt, is_expert),
        d_value(value),
        d_enum2string(enum2string)
  {
    for (const auto& p : enum2string)
    {
      d_string2enum.emplace(p.second, p.first);
    }
  }
  OptionEnumT() = delete;

  const T& operator()() const { return d_value; }

  /** @return The default value of this option. */
  const T& dflt() const { return d_default; }

 private:
  std::vector<std::string> modes() const override;
  const std::string& get_str() const override;
  void set_str(const std::string& value) override;
  const std::string& dflt_str() const override;
  bool is_valid(const std::string& value) const override;

  /** The current enum value. */
  T d_value;
  /** The default enum value. */
  T d_default;
  /** A map from enum value to its string representation for the CLI. */
  Enum2StringMap d_enum2string;
  /** A map from string representation for the CLI to enum value. */
  String2EnumMap d_string2enum;
};

/* -------------------------------------------------------------------------- */

class Options
{
  /* Note: d_options and d_lng2option must be initialized first since
   *       initialization of public option members depends on it */

  friend OptionBase;

 private:
  /** The registered options. */
  std::unordered_map<Option, OptionBase*> d_options;
  /** Map long option name to option. */
  std::unordered_map<std::string, Option> d_lng2option;

 public:
  static constexpr uint64_t VERBOSITY_MAX = 4;
  static constexpr uint64_t PROB_100      = 1000;
  static constexpr uint64_t PROB_50       = 500;

  /** Constructor. */
  Options();

  // general options
  OptionBool incremental;
  OptionNumeric log_level;
  OptionBool produce_models;
  OptionBool produce_unsat_cores;
  OptionEnumT<SatSolver> sat_solver;
  OptionNumeric seed;
  OptionNumeric verbosity;

  OptionEnumT<BvSolver> bv_solver;
  OptionBool smt_comp_mode;

  // BV: propagation-based local search engine
  OptionNumeric prop_nprops;
  OptionNumeric prop_nupdates;
  OptionEnumT<PropPathSelection> prop_path_sel;
  OptionNumeric prop_prob_pick_inv_value;
  OptionNumeric prop_prob_pick_random_input;
  OptionBool prop_const_bits;
  OptionBool prop_ineq_bounds;
  OptionBool prop_opt_lt_concat_sext;
  OptionBool prop_sext;

  /** @return True if the given option is a Boolean option. */
  bool is_bool(Option opt) const;
  /** @return True if the given option is a numeric option. */
  bool is_numeric(Option opt) const;
  /** @return True if the given option is an enum option. */
  bool is_enum(Option opt) const;

  /** @return True if the given value is a valid mode for an enum option. */
  bool is_valid_enum(Option opt, const std::string& value) const;

  /** @return The description of the given option. */
  const char* description(Option opt) const;
  /** @return The long name of the given option. */
  const char* lng(Option opt) const;
  /** @return The short name of the given option. */
  const char* shrt(Option opt) const;

  /**
   * @return The string representations of all valid modes for an enum option.
   */
  std::vector<std::string> modes(Option opt) const;

  /** @return Option associated with the given long option name. */
  Option option(const char* lng) const;

  /**
   * Set current value of option.
   * @param opt The option to set.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @param value The value to set.
   */
  template <typename T>
  void set(Option opt, const T& value);

  /**
   * Get the current value of option.
   * @param opt The option to set.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @return The current value.
   */
  template <typename T>
  const T& get(Option opt) const;

  /**
   * Get the minimum value of option.
   * @param opt The option to set.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @return The minimum value.
   */
  template <typename T>
  const T& min(Option opt) const;

  /**
   * Get the maximum value of option.
   * @param opt The option to set.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @return The maximum value.
   */
  template <typename T>
  const T& max(Option opt) const;

  /**
   * Get the maximum value of option.
   * @param opt The option to set.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @return The maximum value.
   */
  template <typename T>
  const T& dflt(Option opt) const;

 private:
  /**
   * Register option.
   * @note This is mainly necessary to have access to options via their enum
   *       identifier from external (the API).
   * @param opt  The option.
   * @param option The associated option data.
   */
  void register_option(Option opt, OptionBase* option);
};

// explicit instantiations
template <>
void Options::set(Option opt, const bool& value);
template <>
void Options::set(Option opt, const uint64_t& value);
template <>
void Options::set(Option opt, const std::string& value);

template <>
const bool& Options::get(Option opt) const;
template <>
const uint64_t& Options::get(Option opt) const;
template <>
const std::string& Options::get(Option opt) const;

template <>
const bool& Options::dflt(Option opt) const;
template <>
const uint64_t& Options::dflt(Option opt) const;
template <>
const std::string& Options::dflt(Option opt) const;

template <>
const uint64_t& Options::min(Option opt) const;
template <>
const uint64_t& Options::max(Option opt) const;

/* -------------------------------------------------------------------------- */
}  // namespace bzla::option

#endif
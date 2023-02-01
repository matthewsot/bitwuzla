#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>

#include "api/cpp/bitwuzla.h"

namespace {

/** Helper to format option names and add meta variables. */
std::string
format_opt(const char* name, bool is_shrt, bool is_numeric, bool is_mode)
{
  if (name)
  {
    std::string fmt = is_shrt ? "-" : "--";
    fmt += name;
    if (is_numeric)
    {
      fmt += " <n>";
    }
    else if (is_mode)
    {
      fmt += " <M>";
    }
    return fmt;
  }
  return std::string();
}

/** Format short name of mode option. */
std::string
format_shortm(const char* shrt)
{
  return format_opt(shrt, true, false, true);
}

/** Format long name of mode option. */
std::string
format_longm(const char* lng)
{
  return format_opt(lng, false, false, true);
}

/** Format short name of numeric option. */
std::string
format_shortn(const char* shrt)
{
  return format_opt(shrt, true, true, false);
}

/** Format long name of numeric option. */
std::string
format_longn(const char* lng)
{
  return format_opt(lng, false, true, false);
}

/** Format short name of boolean option. */
std::string
format_shortb(const char* shrt)
{
  return format_opt(shrt, true, false, false);
}

/** Format long name of boolean option. */
std::string
format_longb(const char* lng)
{
  return format_opt(lng, false, false, false);
}

/** Format string of default value. */
std::string
format_dflt(const std::string& dflt, bool is_bool = false)
{
  if (is_bool)
  {
    return "[" + std::string((dflt == "1" ? "true" : "false")) + "]";
  }
  return "[" + dflt + "]";
}

/** Wrap text to remaining width of 80 character wide line. */
std::string
wrap(const std::string& text, size_t already_used_width)
{
  std::istringstream iss(text);
  size_t line_len = 0;
  size_t width    = 80 - already_used_width;
  std::stringstream wrapped;
  std::string w;
  while (std::getline(iss, w, ' '))
  {
    assert(w.size() < width);
    if (line_len + w.size() >= width)
    {
      wrapped << "\n  " << std::setw(already_used_width) << " ";
      line_len = 0;
    }
    wrapped << w << " ";
    line_len += w.size();
  }
  return wrapped.str();
}

/** Print help message. */
void
print_help(const bitwuzla::Options& options)
{
  std::vector<std::tuple<std::string, std::string, std::string, std::string>>
      opts;

  // Main options
  opts.emplace_back(
      "", "<input>", "", "input file, reads from stdin if omitted");
  opts.emplace_back(format_shortb("h"),
                    format_longb("help"),
                    "",
                    "print this message and exit");
  opts.emplace_back(format_shortb("V"),
                    format_longb("version"),
                    "",
                    "print version and exit");
  opts.emplace_back(format_shortb("c"),
                    format_longb("copyright"),
                    "",
                    "print copyright and exit");

  // Format library options
  for (size_t i = 0, size = static_cast<size_t>(bitwuzla::Option::NUM_OPTS);
       i < size;
       ++i)
  {
    auto o    = static_cast<bitwuzla::Option>(i);
    auto shrt = options.shrt(o);
    auto lng  = options.lng(o);

    if (options.is_mode(o))
    {
      std::stringstream desc;
      desc << options.description(o);
      auto modes = options.modes(o);
      desc << " {";
      for (size_t j = 0, size = modes.size(); j < size; ++j)
      {
        if (j > 0)
        {
          desc << ", ";
        }
        desc << modes[j];
      }
      desc << "}";
      opts.emplace_back(format_shortm(shrt),
                        format_longm(lng),
                        format_dflt(options.get_mode(o)),
                        desc.str());
    }
    else if (options.is_numeric(o))
    {
      opts.emplace_back(format_shortn(shrt),
                        format_longn(lng),
                        format_dflt(std::to_string(options.get(o))),
                        options.description(o));
    }
    else
    {
      assert(options.is_bool(o));
      opts.emplace_back(format_shortb(shrt),
                        format_longb(lng),
                        format_dflt(std::to_string(options.get(o)), true),
                        options.description(o));
    }
  }

  // Compute maximum size of option column
  size_t max_size_first  = 0;
  size_t max_size_second = 0;
  for (const auto& [shrt, lng, dflt, desc] : opts)
  {
    size_t col_size = shrt.size() + lng.size() + 2;
    if (col_size > max_size_first)
    {
      max_size_first = col_size;
    }
    col_size = dflt.size() + 2;
    if (col_size > max_size_second)
    {
      max_size_second = col_size;
    }
  }

  // Print help message
  std::stringstream ss;
  ss << "Usage:\n";
  ss << "  bitwuzla [<options>] [<input>]\n";
  ss << "\n";
  ss << "Options:\n";
  for (const auto& [shrt, lng, dflt, desc] : opts)
  {
    std::string col;
    if (!shrt.empty())
    {
      col = shrt + ", ";
    }
    col += lng;

    ss << "  " << std::left << std::setw(max_size_first) << col
       << std::setw(max_size_second) << dflt
       << wrap(desc, max_size_first + max_size_second) << "\n";
  }
  std::cout << ss.str() << std::endl;
}

/** Print copyright. */
void
print_copyright()
{
  std::cout << bitwuzla::copyright() << std::endl;
}

/** Print version. */
void
print_version()
{
  std::cout << bitwuzla::version() << std::endl;
}

[[noreturn]] void
bzla_exit(int32_t status)
{
#ifndef NDEBUG
  std::exit(status);
#else
  std::quick_exit(status);
#endif
}

}  // namespace

int32_t
main(int32_t argc, char* argv[])
{
  bitwuzla::Options options;

  std::vector<std::string> args;
  std::string filename;
  for (int32_t i = 1; i < argc; ++i)
  {
    std::string arg(argv[i]);
    if (arg == "-h" || arg == "--help")
    {
      print_help(options);
      bzla_exit(EXIT_SUCCESS);
    }
    else if (arg == "-c" || arg == "--copyright")
    {
      print_copyright();
      bzla_exit(EXIT_SUCCESS);
    }
    else if (arg == "-V" || arg == "--version")
    {
      print_version();
      bzla_exit(EXIT_SUCCESS);
    }
    // Check if argument is the intput file.
    // Note: For now only supports .smt2 and .btor suffices
    else if (arg.find(".smt2") == arg.size() - 5
             || arg.find(".btor") == arg.size() - 5)
    {
      filename = arg;
    }
    else
    {
      args.push_back(arg);
    }
  }

  try
  {
    options.set(args);
  }
  catch (const bitwuzla::Exception& e)
  {
    // Remove the "invalid call to '...', prefix
    const std::string& msg = e.msg();
    size_t pos = msg.find("', ");
    std::cerr << "[error] " << msg.substr(pos + 3) << std::endl;
    bzla_exit(EXIT_FAILURE);
  }

  std::string err_msg = bitwuzla::parse(options, filename);
  if (err_msg.empty())
  {
    bzla_exit(EXIT_SUCCESS);
  }
  std::cerr << "[error] " << err_msg << std::endl;
  bzla_exit(EXIT_FAILURE);
}

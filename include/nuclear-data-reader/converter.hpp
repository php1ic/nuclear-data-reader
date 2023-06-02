/**
 *
 * \class Converter
 *
 * \brief Utility class for the conversion of various units and types
 *
 * In the data files, values can be given to >5 dp, half lives are given
 * in scientific notation, energies are in keV, etc. This class exists
 * to convert all of these things into nice(r) formats to display in the file
 * that is output.
 *
 */
#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <string_view>
#include <type_traits>

#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <optional>
#include <ratio>
#include <string>
#include <utility>
#include <vector>


class Converter
{
public:
  Converter() = default;

  Converter(const Converter&)     = default;
  Converter(Converter&&) noexcept = default;

  Converter& operator=(const Converter&)     = default;
  Converter& operator=(Converter&&) noexcept = default;

  ~Converter() = default;

  /// A year in seconds
  constexpr static auto year = 31556952LL;

  /// Durations that allow the use of doubles rather than ints
  using attoseconds  = std::chrono::duration<double, std::atto>;
  using femtoseconds = std::chrono::duration<double, std::femto>;
  using picoseconds  = std::chrono::duration<double, std::pico>;
  using nanoseconds  = std::chrono::duration<double, std::nano>;
  using microseconds = std::chrono::duration<double, std::micro>;
  using milliseconds = std::chrono::duration<double, std::milli>;
  using seconds      = std::chrono::duration<double, std::ratio<1, 1>>;
  using minutes      = std::chrono::duration<double, std::ratio<60, 1>>;
  using hours        = std::chrono::duration<double, std::ratio<3600, 1>>;
  using days         = std::chrono::duration<double, std::ratio<24L * 3600, 1>>;
  using years        = std::chrono::duration<double, std::ratio<year, 1>>;
  using kiloyears    = std::chrono::duration<double, std::ratio<1000 * year, 1>>;
  using millionyears = std::chrono::duration<double, std::ratio<1000000 * year, 1>>;
  using billionyears = std::chrono::duration<double, std::ratio<1000000000 * year, 1>>;


  /// FIXME: Masstable.hpp contains MAX_Z but if we include here we'll get cyclic includes
  /// If we don't cast, MSVC complains about converting types which causes compilation to fail.
  /// Neither gcc or clang have an issue, maybe they should?
  static constexpr std::array<std::pair<std::string_view, uint16_t>, 119> symbolZmap = {
    { { "n", static_cast<uint16_t>(0) },    { "H", static_cast<uint16_t>(1) },    { "He", static_cast<uint16_t>(2) },
      { "Li", static_cast<uint16_t>(3) },   { "Be", static_cast<uint16_t>(4) },   { "B", static_cast<uint16_t>(5) },
      { "C", static_cast<uint16_t>(6) },    { "N", static_cast<uint16_t>(7) },    { "O", static_cast<uint16_t>(8) },
      { "F", static_cast<uint16_t>(9) },    { "Ne", static_cast<uint16_t>(10) },  { "Na", static_cast<uint16_t>(11) },
      { "Mg", static_cast<uint16_t>(12) },  { "Al", static_cast<uint16_t>(13) },  { "Si", static_cast<uint16_t>(14) },
      { "P", static_cast<uint16_t>(15) },   { "S", static_cast<uint16_t>(16) },   { "Cl", static_cast<uint16_t>(17) },
      { "Ar", static_cast<uint16_t>(18) },  { "K", static_cast<uint16_t>(19) },   { "Ca", static_cast<uint16_t>(20) },
      { "Sc", static_cast<uint16_t>(21) },  { "Ti", static_cast<uint16_t>(22) },  { "V", static_cast<uint16_t>(23) },
      { "Cr", static_cast<uint16_t>(24) },  { "Mn", static_cast<uint16_t>(25) },  { "Fe", static_cast<uint16_t>(26) },
      { "Co", static_cast<uint16_t>(27) },  { "Ni", static_cast<uint16_t>(28) },  { "Cu", static_cast<uint16_t>(29) },
      { "Zn", static_cast<uint16_t>(30) },  { "Ga", static_cast<uint16_t>(31) },  { "Ge", static_cast<uint16_t>(32) },
      { "As", static_cast<uint16_t>(33) },  { "Se", static_cast<uint16_t>(34) },  { "Br", static_cast<uint16_t>(35) },
      { "Kr", static_cast<uint16_t>(36) },  { "Rb", static_cast<uint16_t>(37) },  { "Sr", static_cast<uint16_t>(38) },
      { "Y", static_cast<uint16_t>(39) },   { "Zr", static_cast<uint16_t>(40) },  { "Nb", static_cast<uint16_t>(41) },
      { "Mo", static_cast<uint16_t>(42) },  { "Tc", static_cast<uint16_t>(43) },  { "Ru", static_cast<uint16_t>(44) },
      { "Rh", static_cast<uint16_t>(45) },  { "Pd", static_cast<uint16_t>(46) },  { "Ag", static_cast<uint16_t>(47) },
      { "Cd", static_cast<uint16_t>(48) },  { "In", static_cast<uint16_t>(49) },  { "Sn", static_cast<uint16_t>(50) },
      { "Sb", static_cast<uint16_t>(51) },  { "Te", static_cast<uint16_t>(52) },  { "I", static_cast<uint16_t>(53) },
      { "Xe", static_cast<uint16_t>(54) },  { "Cs", static_cast<uint16_t>(55) },  { "Ba", static_cast<uint16_t>(56) },
      { "La", static_cast<uint16_t>(57) },  { "Ce", static_cast<uint16_t>(58) },  { "Pr", static_cast<uint16_t>(59) },
      { "Nd", static_cast<uint16_t>(60) },  { "Pm", static_cast<uint16_t>(61) },  { "Sm", static_cast<uint16_t>(62) },
      { "Eu", static_cast<uint16_t>(63) },  { "Gd", static_cast<uint16_t>(64) },  { "Tb", static_cast<uint16_t>(65) },
      { "Dy", static_cast<uint16_t>(66) },  { "Ho", static_cast<uint16_t>(67) },  { "Er", static_cast<uint16_t>(68) },
      { "Tm", static_cast<uint16_t>(69) },  { "Yb", static_cast<uint16_t>(70) },  { "Lu", static_cast<uint16_t>(71) },
      { "Hf", static_cast<uint16_t>(72) },  { "Ta", static_cast<uint16_t>(73) },  { "W", static_cast<uint16_t>(74) },
      { "Re", static_cast<uint16_t>(75) },  { "Os", static_cast<uint16_t>(76) },  { "Ir", static_cast<uint16_t>(77) },
      { "Pt", static_cast<uint16_t>(78) },  { "Au", static_cast<uint16_t>(79) },  { "Hg", static_cast<uint16_t>(80) },
      { "Tl", static_cast<uint16_t>(81) },  { "Pb", static_cast<uint16_t>(82) },  { "Bi", static_cast<uint16_t>(83) },
      { "Po", static_cast<uint16_t>(84) },  { "At", static_cast<uint16_t>(85) },  { "Rn", static_cast<uint16_t>(86) },
      { "Fr", static_cast<uint16_t>(87) },  { "Ra", static_cast<uint16_t>(88) },  { "Ac", static_cast<uint16_t>(89) },
      { "Th", static_cast<uint16_t>(90) },  { "Pa", static_cast<uint16_t>(91) },  { "U", static_cast<uint16_t>(92) },
      { "Np", static_cast<uint16_t>(93) },  { "Pu", static_cast<uint16_t>(94) },  { "Am", static_cast<uint16_t>(95) },
      { "Cm", static_cast<uint16_t>(96) },  { "Bk", static_cast<uint16_t>(97) },  { "Cf", static_cast<uint16_t>(98) },
      { "Es", static_cast<uint16_t>(99) },  { "Fm", static_cast<uint16_t>(100) }, { "Md", static_cast<uint16_t>(101) },
      { "No", static_cast<uint16_t>(102) }, { "Lr", static_cast<uint16_t>(103) }, { "Rf", static_cast<uint16_t>(104) },
      { "Db", static_cast<uint16_t>(105) }, { "Sg", static_cast<uint16_t>(106) }, { "Bh", static_cast<uint16_t>(107) },
      { "Hs", static_cast<uint16_t>(108) }, { "Mt", static_cast<uint16_t>(109) }, { "Ds", static_cast<uint16_t>(110) },
      { "Rg", static_cast<uint16_t>(111) }, { "Cn", static_cast<uint16_t>(112) }, { "Nh", static_cast<uint16_t>(113) },
      { "Fl", static_cast<uint16_t>(114) }, { "Mc", static_cast<uint16_t>(115) }, { "Lv", static_cast<uint16_t>(116) },
      { "Ts", static_cast<uint16_t>(117) }, { "Og", static_cast<uint16_t>(118) } }
  };

  /**
   * Get the hash of a const char* so we can use a switch statement on std::string_view
   * https://learnmoderncpp.com/2020/06/01/strings-as-switch-case-labels/
   *
   * \param The 'string' to take the hash of
   *
   * \return The hash of the input parameter
   */
  [[nodiscard]] static constexpr inline auto string_hash(const char* the_string)
  {
    uint64_t hash{};
    uint64_t dummy{};
    for (auto ptr = the_string; *ptr != 0; ++ptr, ++dummy)
      {
        hash += static_cast<uint64_t>(*ptr) << dummy;
      }
    return hash;
  }

  /**
   * Using == on floats/doubles is bad, this function compares 2 floats/doubles to see if they are the same.
   * Lifted (almost) directly from here:
   * https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
   *
   * \param One of the numbers to compare
   * \param The other number to compare
   * \param How much accuracy do we require
   *        https://en.wikipedia.org/wiki/Unit_in_the_last_place
   *
   * \return[true] The numbers equal
   * \return[false] The numbers are not equal
   */
  template<typename T>
    requires std::floating_point<T>
  [[nodiscard]] static constexpr bool almost_equal(const T lhs, const T rhs, const uint8_t ulp = 1) noexcept
  {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(lhs - rhs) <= std::numeric_limits<T>::epsilon() * std::fabs(lhs + rhs) * ulp
           // unless the result is subnormal
           || std::fabs(lhs - rhs) < std::numeric_limits<T>::min();
  }

  /**
   * Convert proton number to elemental symbol
   *
   * \param Z The proton value to convert
   *
   * \return The symbol as a std:string
   */
  [[nodiscard]] static std::optional<std::string_view> ZToSymbol(const uint16_t proton_number);

  /**
   * Convert elemental symbol to proton number
   *
   * \param _symbol The symbol to convert
   *
   * \return The proton number as an int
   */
  [[nodiscard]] static std::optional<uint16_t> SymbolToZ(std::string_view symbol);

  /**
   * Convert any type from it's string(_view) representation to the given type.
   * If the string does not convert properly, return the max value of the type
   *
   * We know that the max line length from the files being read is 201 so a string will
   * not be longer than that, hence the use of uint8.
   *
   * \param The full string containing the number
   * \param The first/start character of the number to convert
   * \param The last/end character of the number to convert
   *
   * \return[success] The portion of the string as a variable of the correc type
   * \return[failure] The max value of the type requested
   */
  template<typename T>
  [[nodiscard]] static constexpr T
  StringToNum(const std::string_view str, const uint8_t start, const uint8_t end) noexcept
  {
    const auto number = NumberAsString(str, start, end);
    T value{};
    const auto [ptr, ec]{ std::from_chars(number.data(), number.data() + number.size(), value) };
    return ec == std::errc() ? value : std::numeric_limits<T>::max();
  }

  /**
   * Extract the part of the string <fullString> from <start> to <end>, but
   * if the substring is all spaces, or contains the '*' character, return an empty string.
   * Similarly, if the difference between <start> and <end> is negative, return an empty string.
   * Otherwise, return the substring requested.
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return The substring as described above
   */
  template<typename T, typename U>
    requires std::unsigned_integral<T> && std::unsigned_integral<U>
  [[nodiscard]] static std::string_view NumberAsString(const std::string_view fullString, const T start, const U end)
  {
    if (end <= start)
      {
        return std::string_view{};
      }

    const auto number = TrimString(fullString.substr(start, end - start));

    return (std::all_of(number.cbegin(), number.cend(), isspace) || number.find('*') != std::string_view::npos)
               ? std::string_view{}
               : number;
  }

  /**
   * Trim leading repeated characters form the given string
   *
   * \param The string_view to trim
   * \param The repeated character to remove (default is a space character)
   *
   * \return The original string with the character(s) removed
   */
  [[nodiscard]] static std::string_view TrimStart(std::string_view str, const std::string_view trim_character = " ")
  {
    str.remove_prefix(std::min(str.find_first_not_of(trim_character), str.size()));
    return str;
  }

  /**
   * Trim tailing repeated characters form the given string
   *
   * \param The string_view to trim
   * \param The repeated character to remove (default is a space character)
   *
   * \return The original string with the character(s) removed
   */
  [[nodiscard]] static std::string_view TrimEnd(std::string_view str, const std::string_view trim_character = " ")
  {
    if (const auto pos = str.find_last_not_of(trim_character); pos != std::string_view::npos)
      {
        str.remove_suffix(str.size() - (pos + 1));
      }
    else
      {
        str = std::string_view{};
      }

    return str;
  }

  /**
   * Trim leading and trailing repeated characters from the given std::string_view
   * Note that any occurance of the character 'inside' the string are not removed
   *
   * \param The string_view to trim
   * \param The repeated character to remove (default is a space character)
   *
   * \return The original string with the character(s) removed
   */
  [[nodiscard]] static std::string_view TrimString(std::string_view str, const std::string_view trim_character = " ")
  {
    return Converter::TrimEnd(Converter::TrimStart(str, trim_character), trim_character);
  }

  /**
   * Convert a floating point number to a std::string with the specified number of decimal places.
   * We store 'null' values as the max value a double can take. If that's the case, return 'null' as a string
   *
   * \param number The number to convert
   * \param numDP The number of decimal points to include in the output string
   *
   * \return A std::string of the input number, truncated to the required precision
   * \return A std::string with contents "null" if number is std::numeric_limits<double>::max()
   */
  [[nodiscard]] static std::string FloatToNdp(const double number, const uint8_t numDP = 1) noexcept;

  /**
   * Convert a numeric value and it's unit to a chrono::duration.
   *
   * \param value The numeric value of the time
   * \param unit The SI unit of the time
   *
   * \return a std::chrono::duration represetning the value and unit given
   */
  [[nodiscard]] static std::chrono::duration<double> ToDuration(const double value, std::string_view unit) noexcept;
};


/// string literal to force the hashing of the string
[[nodiscard]] constexpr inline auto operator"" _sh(const char* the_string, size_t /*unused*/)
{
  return Converter::string_hash(the_string);
}

#endif // CONVERTER_HPP

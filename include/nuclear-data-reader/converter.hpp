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

  // Lambda function to cast as required and avoid copy paste errors if we ever need larger values
  static constexpr auto DRY_cast = [](const int value) mutable { return static_cast<uint16_t>(value); };

  /// FIXME: Masstable.hpp contains MAX_Z but if we include here we'll get cyclic includes.
  /// If we don't cast, MSVC complains about converting types which causes compilation to fail.
  /// Neither gcc or clang have an issue, maybe they should?
  static constexpr std::array<std::pair<std::string_view, uint16_t>, 119> symbolZmap = {
    { { "n", DRY_cast(0) },    { "H", DRY_cast(1) },    { "He", DRY_cast(2) },   { "Li", DRY_cast(3) },
      { "Be", DRY_cast(4) },   { "B", DRY_cast(5) },    { "C", DRY_cast(6) },    { "N", DRY_cast(7) },
      { "O", DRY_cast(8) },    { "F", DRY_cast(9) },    { "Ne", DRY_cast(10) },  { "Na", DRY_cast(11) },
      { "Mg", DRY_cast(12) },  { "Al", DRY_cast(13) },  { "Si", DRY_cast(14) },  { "P", DRY_cast(15) },
      { "S", DRY_cast(16) },   { "Cl", DRY_cast(17) },  { "Ar", DRY_cast(18) },  { "K", DRY_cast(19) },
      { "Ca", DRY_cast(20) },  { "Sc", DRY_cast(21) },  { "Ti", DRY_cast(22) },  { "V", DRY_cast(23) },
      { "Cr", DRY_cast(24) },  { "Mn", DRY_cast(25) },  { "Fe", DRY_cast(26) },  { "Co", DRY_cast(27) },
      { "Ni", DRY_cast(28) },  { "Cu", DRY_cast(29) },  { "Zn", DRY_cast(30) },  { "Ga", DRY_cast(31) },
      { "Ge", DRY_cast(32) },  { "As", DRY_cast(33) },  { "Se", DRY_cast(34) },  { "Br", DRY_cast(35) },
      { "Kr", DRY_cast(36) },  { "Rb", DRY_cast(37) },  { "Sr", DRY_cast(38) },  { "Y", DRY_cast(39) },
      { "Zr", DRY_cast(40) },  { "Nb", DRY_cast(41) },  { "Mo", DRY_cast(42) },  { "Tc", DRY_cast(43) },
      { "Ru", DRY_cast(44) },  { "Rh", DRY_cast(45) },  { "Pd", DRY_cast(46) },  { "Ag", DRY_cast(47) },
      { "Cd", DRY_cast(48) },  { "In", DRY_cast(49) },  { "Sn", DRY_cast(50) },  { "Sb", DRY_cast(51) },
      { "Te", DRY_cast(52) },  { "I", DRY_cast(53) },   { "Xe", DRY_cast(54) },  { "Cs", DRY_cast(55) },
      { "Ba", DRY_cast(56) },  { "La", DRY_cast(57) },  { "Ce", DRY_cast(58) },  { "Pr", DRY_cast(59) },
      { "Nd", DRY_cast(60) },  { "Pm", DRY_cast(61) },  { "Sm", DRY_cast(62) },  { "Eu", DRY_cast(63) },
      { "Gd", DRY_cast(64) },  { "Tb", DRY_cast(65) },  { "Dy", DRY_cast(66) },  { "Ho", DRY_cast(67) },
      { "Er", DRY_cast(68) },  { "Tm", DRY_cast(69) },  { "Yb", DRY_cast(70) },  { "Lu", DRY_cast(71) },
      { "Hf", DRY_cast(72) },  { "Ta", DRY_cast(73) },  { "W", DRY_cast(74) },   { "Re", DRY_cast(75) },
      { "Os", DRY_cast(76) },  { "Ir", DRY_cast(77) },  { "Pt", DRY_cast(78) },  { "Au", DRY_cast(79) },
      { "Hg", DRY_cast(80) },  { "Tl", DRY_cast(81) },  { "Pb", DRY_cast(82) },  { "Bi", DRY_cast(83) },
      { "Po", DRY_cast(84) },  { "At", DRY_cast(85) },  { "Rn", DRY_cast(86) },  { "Fr", DRY_cast(87) },
      { "Ra", DRY_cast(88) },  { "Ac", DRY_cast(89) },  { "Th", DRY_cast(90) },  { "Pa", DRY_cast(91) },
      { "U", DRY_cast(92) },   { "Np", DRY_cast(93) },  { "Pu", DRY_cast(94) },  { "Am", DRY_cast(95) },
      { "Cm", DRY_cast(96) },  { "Bk", DRY_cast(97) },  { "Cf", DRY_cast(98) },  { "Es", DRY_cast(99) },
      { "Fm", DRY_cast(100) }, { "Md", DRY_cast(101) }, { "No", DRY_cast(102) }, { "Lr", DRY_cast(103) },
      { "Rf", DRY_cast(104) }, { "Db", DRY_cast(105) }, { "Sg", DRY_cast(106) }, { "Bh", DRY_cast(107) },
      { "Hs", DRY_cast(108) }, { "Mt", DRY_cast(109) }, { "Ds", DRY_cast(110) }, { "Rg", DRY_cast(111) },
      { "Cn", DRY_cast(112) }, { "Nh", DRY_cast(113) }, { "Fl", DRY_cast(114) }, { "Mc", DRY_cast(115) },
      { "Lv", DRY_cast(116) }, { "Ts", DRY_cast(117) }, { "Og", DRY_cast(118) } }
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

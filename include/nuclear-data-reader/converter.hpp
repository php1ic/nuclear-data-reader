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

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <limits>
#include <ratio>
#include <string>
#include <utility>
#include <vector>

class Converter
{
public:
  Converter() = default;

  Converter(const Converter&) = default;
  Converter(Converter&&)      = default;

  Converter& operator=(Converter&&) = default;
  Converter& operator=(const Converter&) = default;

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
  using days         = std::chrono::duration<double, std::ratio<24 * 3600, 1>>;
  using years        = std::chrono::duration<double, std::ratio<year, 1>>;
  using kiloyears    = std::chrono::duration<double, std::ratio<1000 * year, 1>>;
  using millionyears = std::chrono::duration<double, std::ratio<1000000 * year, 1>>;
  using billionyears = std::chrono::duration<double, std::ratio<1000000000 * year, 1>>;


  /// The std::vector<std::pair<>> used to convert Z<->Symbol
  static inline const std::vector<std::pair<std::string, int>> symbolZmap{
    { "n", 0 },    { "H", 1 },    { "He", 2 },   { "Li", 3 },   { "Be", 4 },   { "B", 5 },    { "C", 6 },
    { "N", 7 },    { "O", 8 },    { "F", 9 },    { "Ne", 10 },  { "Na", 11 },  { "Mg", 12 },  { "Al", 13 },
    { "Si", 14 },  { "P", 15 },   { "S", 16 },   { "Cl", 17 },  { "Ar", 18 },  { "K", 19 },   { "Ca", 20 },
    { "Sc", 21 },  { "Ti", 22 },  { "V", 23 },   { "Cr", 24 },  { "Mn", 25 },  { "Fe", 26 },  { "Co", 27 },
    { "Ni", 28 },  { "Cu", 29 },  { "Zn", 30 },  { "Ga", 31 },  { "Ge", 32 },  { "As", 33 },  { "Se", 34 },
    { "Br", 35 },  { "Kr", 36 },  { "Rb", 37 },  { "Sr", 38 },  { "Y", 39 },   { "Zr", 40 },  { "Nb", 41 },
    { "Mo", 42 },  { "Tc", 43 },  { "Ru", 44 },  { "Rh", 45 },  { "Pd", 46 },  { "Ag", 47 },  { "Cd", 48 },
    { "In", 49 },  { "Sn", 50 },  { "Sb", 51 },  { "Te", 52 },  { "I", 53 },   { "Xe", 54 },  { "Cs", 55 },
    { "Ba", 56 },  { "La", 57 },  { "Ce", 58 },  { "Pr", 59 },  { "Nd", 60 },  { "Pm", 61 },  { "Sm", 62 },
    { "Eu", 63 },  { "Gd", 64 },  { "Tb", 65 },  { "Dy", 66 },  { "Ho", 67 },  { "Er", 68 },  { "Tm", 69 },
    { "Yb", 70 },  { "Lu", 71 },  { "Hf", 72 },  { "Ta", 73 },  { "W", 74 },   { "Re", 75 },  { "Os", 76 },
    { "Ir", 77 },  { "Pt", 78 },  { "Au", 79 },  { "Hg", 80 },  { "Tl", 81 },  { "Pb", 82 },  { "Bi", 83 },
    { "Po", 84 },  { "At", 85 },  { "Rn", 86 },  { "Fr", 87 },  { "Ra", 88 },  { "Ac", 89 },  { "Th", 90 },
    { "Pa", 91 },  { "U", 92 },   { "Np", 93 },  { "Pu", 94 },  { "Am", 95 },  { "Cm", 96 },  { "Bk", 97 },
    { "Cf", 98 },  { "Es", 99 },  { "Fm", 100 }, { "Md", 101 }, { "No", 102 }, { "Lr", 103 }, { "Rf", 104 },
    { "Db", 105 }, { "Sg", 106 }, { "Bh", 107 }, { "Hs", 108 }, { "Mt", 109 }, { "Ds", 110 }, { "Rg", 111 },
    { "Cn", 112 }, { "Ed", 113 }, { "Fl", 114 }, { "Ef", 115 }, { "Lv", 116 }, { "Eh", 117 }, { "Ei", 118 }
  };


  /**
   * Using == on floats/doubles is bad, this function compares 2 floats/doubles to see if they are the same.
   * Lifted directly from here:
   * https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
   *
   * \param One of the numbers to compare
   * \param The other number to compare
   * \param
   *
   * \return[true] The numbers equal
   * \return[false] The numbers are not equal
   */
  template<class T>
  static typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y, int ulp)
  {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
           // unless the result is subnormal
           || std::fabs(x - y) < std::numeric_limits<T>::min();
  }

  /**
   * Convert proton number to elemental symbol
   *
   * \param Z The proton value to convert
   *
   * \return The symbol as a std:string
   */
  [[nodiscard]] static std::string ZToSymbol(const int Z);

  /**
   * Convert elemental symbol to proton number
   *
   * \param _symbol The symbol to convert
   *
   * \return The proton number as an int
   */
  [[nodiscard]] static int SymbolToZ(std::string_view symbol);

  /**
   * Convert the entire string <var> into it's integer value.
   * If the string is not an int, pass it through Converter::SymbolToZ
   *
   * \param The string to be converted
   *
   * \return[success] The string as a number
   * \return[failure] 200 signifies failure
   */
  [[nodiscard]] static int StringToInt(const std::string& var);

  /**
   * Extract the part of the string <fullString> from <start> to <end>
   * if it's all spaces or contains the '*' character return an empty string
   * Otherwise, return the substring
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return The substring as desccribed above
   */
  [[nodiscard]] static std::string NumberAsString(const std::string& fullString, const int start, const int end)
  {
    const auto number = fullString.substr(start, end - start);
    return (std::all_of(number.cbegin(), number.cend(), isspace) || number.find('*') != std::string::npos) ? ""
                                                                                                           : number;
  }

  /**
   * Convert the part of the string <fullString> from <start> to <end>
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return[PASS] The given substring as an integer
   * \return[FAIL] The max int type value if an empty string (i.e. all space characters) is provided
   */
  [[nodiscard]] static inline int StringToInt(const std::string& fullString, const int start, const int end)
  {
    const auto number = NumberAsString(fullString, start, end);
    return number.empty() ? std::numeric_limits<int>::max() : std::stoi(number);
  }

  /**
   * Convert the the part of the string <fullString> from <start> to <end>
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return[PASS] The given substring as an double
   * \return[FAIL] The max double type value if an empty string (i.e. all space characters) is provided
   */
  [[nodiscard]] static inline double StringToDouble(const std::string& fullString, const int start, const int end)
  {
    const auto number = NumberAsString(fullString, start, end);
    return number.empty() ? std::numeric_limits<double>::max() : std::stod(number);
  }

  /**
   * Convert a floating point number to a std::string with the specified number of decimal places.
   * We store 'null' values as the max value a double can take. Iif that's the case, return 'null' as a sring
   *
   * \param number The number to convert
   * \param numDP The number of decimal points to include in the output string
   *
   * \return A std:string of the input number, truncated to the required precision
   * \return A std::string with contents "null" if number is std::numeric_limits<double>::max()
   */
  [[nodiscard]] static std::string FloatToNdp(const double number, const int numDP = 1);
};

#endif // CONVERTER_HPP

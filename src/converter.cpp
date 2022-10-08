#include "nuclear-data-reader/converter.hpp"

#include <string_view>

#include <fmt/format.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>


std::optional<std::string_view> Converter::ZToSymbol(const uint16_t proton_number)
{
  const auto symbol = std::find_if(symbolZmap.cbegin(), symbolZmap.cend(), [proton_number](const auto& element) {
    return element.second == proton_number;
  });

  if (symbol == symbolZmap.cend()) [[unlikely]]
    {
      return std::nullopt;
    }

  return symbol->first;
}


std::optional<uint16_t> Converter::SymbolToZ(const std::string_view symbol)
{
  // FIXME: Check that the string format matches that in the map,
  // i.e. first character capital letter, 2nd (if it exists) lower case ^[A-Z]{1}[a-z]?$
  const auto proton_number = std::find_if(
      symbolZmap.cbegin(), symbolZmap.cend(), [symbol](const auto& element) { return element.first == symbol; });

  if (proton_number == symbolZmap.cend()) [[unlikely]]
    {
      return std::nullopt;
    }

  return proton_number->second;
}


std::string Converter::FloatToNdp(const double number, const uint8_t numDP) noexcept
{
  return Converter::almost_equal(number, std::numeric_limits<double>::max()) ? "null"
                                                                             : fmt::format("{:.{}f}", number, numDP);
}


std::chrono::duration<double> Converter::ToDuration(const double value, std::string_view unit) noexcept
{
  return [=]() -> std::chrono::duration<double> {
    switch (Converter::string_hash(unit.data()))
      {
        case "ys"_sh:
          return Converter::attoseconds{ 1.0e-6 * value };
        case "zs"_sh:
          return Converter::attoseconds{ 1.0e-3 * value };
        case "as"_sh:
          return Converter::attoseconds{ value };
        case "ps"_sh:
          return Converter::picoseconds{ value };
        case "ns"_sh:
          return Converter::nanoseconds{ value };
        case "us"_sh:
          return Converter::microseconds{ value };
        case "ms"_sh:
          return Converter::milliseconds{ value };
        case "s"_sh:
          return Converter::seconds{ value };
        case "m"_sh:
          return Converter::minutes{ value };
        case "h"_sh:
          return Converter::hours{ value };
        case "d"_sh:
          return Converter::days{ value };
        case "y"_sh:
          return Converter::years{ value };
        case "ky"_sh:
          return Converter::kiloyears{ value };
        case "My"_sh:
          return Converter::millionyears{ value };
        case "Gy"_sh:
          return Converter::billionyears{ value };
        case "Ty"_sh:
          return Converter::billionyears{ 1.0e3 * value };
        case "Py"_sh:
          return Converter::billionyears{ 1.0e6 * value };
        case "Ey"_sh:
          return Converter::billionyears{ 1.0e9 * value };
        case "Zy"_sh:
          return Converter::billionyears{ 1.0e12 * value };
        case "Yy"_sh:
          return Converter::billionyears{ 1.0e15 * value };
        default:
          return Converter::seconds{ 1.0e24 };
      }
  }();
}

#include "nuclear-data-reader/converter.hpp"

#include <string_view>

#include <fmt/format.h>

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>


std::string_view Converter::ZToSymbol(const uint8_t Z, const uint8_t verbosity)
{
  const auto it = std::find_if(
      symbolZmap().cbegin(), symbolZmap().cend(), [Z](const auto& element) { return element.second == Z; });

  return [&]() {
    if (it == symbolZmap().end())
      {
        if (verbosity > 0)
          {
            fmt::print("\n**WARNING**: {} is not a valid proton number\n", Z);
          }
        return std::string_view{ "Xy" };
      }

    return it->first;
  }();
}


uint8_t Converter::SymbolToZ(const std::string_view symbol, const uint8_t verbosity)
{
  const auto it = std::find_if(
      symbolZmap().cbegin(), symbolZmap().cend(), [symbol](const auto& element) { return element.first == symbol; });

  return [&]() {
    if (it == symbolZmap().end())
      {
        if (verbosity > 0)
          {
            fmt::print("\n**WARNING**: {} is not a valid symbol\n", symbol);
          }
        return uint8_t{ 200 };
      }

    return it->second;
  }();
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

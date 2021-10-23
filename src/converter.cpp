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


std::tuple<std::chrono::duration<double>, std::chrono::duration<double>>
Converter::ToDuration(std::string_view unit, const double value, const double error) noexcept
{
  if (unit == "ys" || unit.find_first_not_of(' ') == std::string::npos)
    {
      return { Converter::attoseconds{ 1.0e-6 * value }, Converter::attoseconds{ 1.0e-6 * error } };
    }
  else if (unit == "zs")
    {
      return { Converter::attoseconds{ 1.0e-3 * value }, Converter::attoseconds{ 1.0e-3 * error } };
    }
  else if (unit == "as")
    {
      return { Converter::attoseconds{ value }, Converter::attoseconds{ error } };
    }
  else if (unit == "ps")
    {
      return { Converter::picoseconds{ value }, Converter::picoseconds{ error } };
    }
  else if (unit == "ns")
    {
      return { Converter::nanoseconds{ value }, Converter::nanoseconds{ error } };
    }
  else if (unit == "us")
    {
      return { Converter::microseconds{ value }, Converter::microseconds{ error } };
    }
  else if (unit == "ms")
    {
      return { Converter::milliseconds{ value }, Converter::milliseconds{ error } };
    }
  else if (unit == "s")
    {
      return { Converter::seconds{ value }, Converter::seconds{ error } };
    }
  else if (unit == "m")
    {
      return { Converter::minutes{ value }, Converter::minutes{ error } };
    }
  else if (unit == "h")
    {
      return { Converter::hours{ value }, Converter::hours{ error } };
    }
  else if (unit == "d")
    {
      return { Converter::days{ value }, Converter::days{ error } };
    }
  else if (unit == "y")
    {
      return { Converter::years{ value }, Converter::years{ error } };
    }
  else if (unit == "ky")
    {
      return { Converter::kiloyears{ value }, Converter::kiloyears{ error } };
    }
  else if (unit == "My")
    {
      return { Converter::millionyears{ value }, Converter::millionyears{ error } };
    }
  else if (unit == "Gy")
    {
      return { Converter::billionyears{ value }, Converter::billionyears{ error } };
    }
  else if (unit == "Ty")
    {
      return { Converter::billionyears{ 1.0e3 * value }, Converter::billionyears{ 1.0e3 * error } };
    }
  else if (unit == "Py")
    {
      return { Converter::billionyears{ 1.0e6 * value }, Converter::billionyears{ 1.0e6 * error } };
    }
  else if (unit == "Ey")
    {
      return { Converter::billionyears{ 1.0e9 * value }, Converter::billionyears{ 1.0e9 * error } };
    }
  else if (unit == "Zy")
    {
      return { Converter::billionyears{ 1.0e12 * value }, Converter::billionyears{ 1.0e12 * error } };
    }
  else if (unit == "Yy")
    {
      return { Converter::billionyears{ 1.0e15 * value }, Converter::billionyears{ 1.0e15 * error } };
    }
  else
    {
      return { Converter::seconds{ 1.0e24 }, Converter::seconds{ 1.0 } };
    }
}

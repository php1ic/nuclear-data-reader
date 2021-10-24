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
  std::chrono::duration<double> duration{};

  if (unit == "ys" || unit.find_first_not_of(' ') == std::string::npos)
    {
      duration = Converter::attoseconds{ 1.0e-6 * value };
    }
  else if (unit == "zs")
    {
      duration = Converter::attoseconds{ 1.0e-3 * value };
    }
  else if (unit == "as")
    {
      duration = Converter::attoseconds{ value };
    }
  else if (unit == "ps")
    {
      duration = Converter::picoseconds{ value };
    }
  else if (unit == "ns")
    {
      duration = Converter::nanoseconds{ value };
    }
  else if (unit == "us")
    {
      duration = Converter::microseconds{ value };
    }
  else if (unit == "ms")
    {
      duration = Converter::milliseconds{ value };
    }
  else if (unit == "s")
    {
      duration = Converter::seconds{ value };
    }
  else if (unit == "m")
    {
      duration = Converter::minutes{ value };
    }
  else if (unit == "h")
    {
      duration = Converter::hours{ value };
    }
  else if (unit == "d")
    {
      duration = Converter::days{ value };
    }
  else if (unit == "y")
    {
      duration = Converter::years{ value };
    }
  else if (unit == "ky")
    {
      duration = Converter::kiloyears{ value };
    }
  else if (unit == "My")
    {
      duration = Converter::millionyears{ value };
    }
  else if (unit == "Gy")
    {
      duration = Converter::billionyears{ value };
    }
  else if (unit == "Ty")
    {
      duration = Converter::billionyears{ 1.0e3 * value };
    }
  else if (unit == "Py")
    {
      duration = Converter::billionyears{ 1.0e6 * value };
    }
  else if (unit == "Ey")
    {
      duration = Converter::billionyears{ 1.0e9 * value };
    }
  else if (unit == "Zy")
    {
      duration = Converter::billionyears{ 1.0e12 * value };
    }
  else if (unit == "Yy")
    {
      duration = Converter::billionyears{ 1.0e15 * value };
    }
  else
    {
      duration = Converter::seconds{ 1.0e24 };
    }

  return duration;
}

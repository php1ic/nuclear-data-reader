#include "nuclear-data-reader/converter.hpp"

#include <string_view>

#include <fmt/format.h>

#include <algorithm>
#include <limits>
#include <stdexcept>
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
  return Converter::almost_equal(number, std::numeric_limits<double>::max(), 1) ? "null"
                                                                                : fmt::format("{:.{}f}", number, numDP);
}

#include "nuclear-data-reader/converter.hpp"

#include <string_view>

#include <fmt/format.h>

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>


int Converter::StringToInt(const std::string& var)
{
  int number{ 0 };

  try
    {
      number = std::stoi(var);
    }
  catch (const std::invalid_argument& ia)
    {
      number = Converter::SymbolToZ(var);
    }

  return number;
}

std::string Converter::ZToSymbol(const int Z)
{
  const auto it =
      std::find_if(symbolZmap.cbegin(), symbolZmap.cend(), [Z](const auto& element) { return element.second == Z; });

  return [&]() {
    if (it == symbolZmap.end())
      {
        // fmt::print("\n**WARNING**: {} is not a valid proton number\n", Z);
        return std::string{ "Xy" };
      }

    return it->first;
  }();
}


int Converter::SymbolToZ(std::string_view symbol)
{
  const auto it = std::find_if(
      symbolZmap.cbegin(), symbolZmap.cend(), [symbol](const auto& element) { return element.first == symbol; });

  return [&]() {
    if (it == symbolZmap.end())
      {
        // fmt::print("\n**WARNING**: {} is not a valid symbol\n", symbol);
        return 200;
      }

    return it->second;
  }();
}


std::string Converter::FloatToNdp(const double number, const int numDP)
{
  return Converter::almost_equal(number, std::numeric_limits<double>::max(), 1) ? "null"
                                                                                : fmt::format("{:.{}f}", number, numDP);
}

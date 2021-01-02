#include "nuclear-data-reader/converter.hpp"

#include <catch2/catch.hpp>

#include <limits>


TEST_CASE("String -> int", "[Converter]")
{
  REQUIRE(Converter::StringToInt("abc123def", 3, 6) == 123);
  REQUIRE(Converter::StringToInt("456", 0, 3) == 456);
  REQUIRE(Converter::StringToInt("   ", 0, 3) == std::numeric_limits<int>::max());
}


TEST_CASE("String -> double", "[Converter]")
{
  REQUIRE(Converter::StringToDouble("abc1.23def", 3, 7) == Approx(1.23));
  REQUIRE(Converter::StringToDouble("45.6", 0, 4) == Approx(45.6));
  REQUIRE(Converter::StringToDouble("   ", 0, 3) == Approx(std::numeric_limits<double>::max()));
}


TEST_CASE("String/Symbol to Int", "[Converter]")
{
  REQUIRE(Converter::StringToInt("C") == 6);
  REQUIRE(Converter::StringToInt("He") == 2);
  REQUIRE(Converter::StringToInt("Ag") == 47);
}


TEST_CASE("Symbol -> Z", "[Converter]")
{
  SECTION("A valid isotope symbol is given")
  {
    REQUIRE(Converter::SymbolToZ("He") == 2);
    REQUIRE(Converter::SymbolToZ("C") == 6);
    REQUIRE(Converter::SymbolToZ("Ag") == 47);
  }

  SECTION("An invalid isotope symbol is given")
  {
    REQUIRE(Converter::SymbolToZ("Xy") == 200);
    REQUIRE(Converter::SymbolToZ("ab") == 200);
    REQUIRE(Converter::SymbolToZ("IS") == 200);
  }
}


TEST_CASE("Z -> Symbol", "[Converter]")
{
  SECTION("A valid proton number is given")
  {
    REQUIRE_THAT(Converter::ZToSymbol(1), Catch::Matches("H"));
    REQUIRE_THAT(Converter::ZToSymbol(23), Catch::Matches("V"));
    REQUIRE_THAT(Converter::ZToSymbol(45), Catch::Matches("Rh"));
  }

  SECTION("An invalid proton number is given")
  {
    REQUIRE_THAT(Converter::ZToSymbol(-2), Catch::Matches("Xy"));
    REQUIRE_THAT(Converter::ZToSymbol(120), Catch::Matches("Xy"));
  }
}


TEST_CASE("Floating point comparison", "[Converter]")
{
  REQUIRE(Converter::almost_equal(0.1, 0.1, 1));
  REQUIRE_FALSE(Converter::almost_equal(0.12, 0.11, 1));
}


TEST_CASE("Float -> string", "[Converter]")
{
  REQUIRE_THAT(Converter::FloatToNdp(1.234, 3), Catch::Matches("1.234"));
  REQUIRE_THAT(Converter::FloatToNdp(std::numeric_limits<double>::max(), 2), Catch::Matches("null"));
}

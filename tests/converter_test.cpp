#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "nuclear-data-reader/converter.hpp"

#include <catch2/catch.hpp>


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
    REQUIRE(Converter::ZToSymbol(1) == "H");
    REQUIRE(Converter::ZToSymbol(23) == "V");
    REQUIRE(Converter::ZToSymbol(45) == "Rh");
  }

  SECTION("An invalid proton number is given")
  {
    REQUIRE(Converter::ZToSymbol(-2) == "Xy");
    REQUIRE(Converter::ZToSymbol(120) == "Xy");
  }
}


TEST_CASE("Floating point comparison", "[Converter]")
{
  REQUIRE(Converter::almost_equal(0.1, 0.1));
  REQUIRE_FALSE(Converter::almost_equal(0.12, 0.11));
}


TEST_CASE("Float -> string", "[Converter]")
{
  REQUIRE_THAT(Converter::FloatToNdp(1.234, 3), Catch::Equals("1.234"));
  REQUIRE_THAT(Converter::FloatToNdp(std::numeric_limits<double>::max(), 2), Catch::Equals("null"));
}


TEST_CASE("Trim a string", "[Converter]")
{
  std::string_view space{ "     567   " };
  REQUIRE(Converter::TrimString(space) == "567");

  std::string_view leading_space{ "     963.2" };
  REQUIRE(Converter::TrimString(leading_space) == "963.2");

  std::string_view trailing_space{ "abc123   " };
  REQUIRE(Converter::TrimString(trailing_space) == "abc123");

  std::string_view different_char{ "___asdf___" };
  REQUIRE(Converter::TrimString(different_char, "_") == "asdf");
}


TEST_CASE("Generic string -> value", "[Converter]")
{
  std::string_view d_str{ "987.654" };
  SECTION("Double") { REQUIRE(Converter::StringToNum<double>(d_str, 0, 7) == Approx(987.654)); }

  std::string_view e_str{ "   *   " };
  SECTION("Double")
  {
    REQUIRE(Converter::StringToNum<double>(e_str, 0, 7) == Approx(std::numeric_limits<double>::max()));
  }

  // Be specific, and verbose, so initialise with empty string
  std::string_view b_str{ "" };
  SECTION("Double")
  {
    REQUIRE(Converter::StringToNum<double>(b_str, 0, 7) == Approx(std::numeric_limits<double>::max()));
  }

  std::string_view f_str{ "abc987.654abc" };
  SECTION("Float") { REQUIRE(Converter::StringToNum<float>(f_str, 3, 10) == Approx(987.654)); }

  std::string_view i8_str{ "123" };
  SECTION("uint8_t") { REQUIRE(Converter::StringToNum<uint8_t>(i8_str, 0, 3) == 123); }

  std::string_view i16_str{ "4563" };
  SECTION("uint16_t") { REQUIRE(Converter::StringToNum<uint16_t>(i16_str, 0, 4) == 4563); }
}


TEST_CASE("", "[.Benchmark]")
{
  SECTION("Double")
  {
    BENCHMARK_ADVANCED("Double charconv")(Catch::Benchmark::Chronometer meter)
    {
      std::string_view sv_str{ "987.654" };
      meter.measure([&sv_str]() { return Converter::StringToNum<double>(sv_str, 0, 7); });
    };
  }

  SECTION("Integer")
  {
    BENCHMARK_ADVANCED("Int charconv")(Catch::Benchmark::Chronometer meter)
    {
      std::string_view sv_str{ "123" };
      meter.measure([&sv_str]() { return Converter::StringToNum<uint8_t>(sv_str, 0, 3); });
    };
  }
}

#include "nuclear-data-reader/converter.hpp"

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>


TEST_CASE("Symbol -> Z", "[Converter]")
{
  SECTION("A valid isotope symbol is given")
  {
    REQUIRE(Converter::SymbolToZ("He").value() == 2);
    REQUIRE(Converter::SymbolToZ("C").value() == 6);
    REQUIRE(Converter::SymbolToZ("Ag").value() == 47);
  }

  SECTION("An invalid isotope symbol is given")
  {
    SECTION("Quiet failure on invalid symbol")
    {
      REQUIRE_FALSE(Converter::SymbolToZ("Xy").has_value());
      REQUIRE_FALSE(Converter::SymbolToZ("ab").has_value());
      REQUIRE_FALSE(Converter::SymbolToZ("IS").has_value());
    }
  }
}


TEST_CASE("Z -> Symbol", "[Converter]")
{
  SECTION("A valid proton number is given")
  {
    REQUIRE_FALSE(Converter::ZToSymbol(1).value().compare("H"));
    REQUIRE_FALSE(Converter::ZToSymbol(23).value().compare("V"));
    REQUIRE_FALSE(Converter::ZToSymbol(45).value().compare("Rh"));
  }

  SECTION("An invalid proton number is given")
  {
    SECTION("Quiet failure on invalid proton number")
    {
      // Replicate a -ve number being passed
      // If we do pass a -ve number, we get a compiler warning about conversion
      REQUIRE_FALSE(Converter::ZToSymbol(65534).has_value());
      REQUIRE_FALSE(Converter::ZToSymbol(120).has_value());
    }
  }
}


TEST_CASE("Floating point comparison", "[Converter]")
{
  REQUIRE(Converter::almost_equal(0.1, 0.1));
  REQUIRE_FALSE(Converter::almost_equal(0.12, 0.11));
}


TEST_CASE("Float -> string", "[Converter]")
{
  REQUIRE_THAT(Converter::FloatToNdp(1.234, 3), Catch::Matchers::Equals("1.234"));
  REQUIRE_THAT(Converter::FloatToNdp(std::numeric_limits<double>::max(), 2), Catch::Matchers::Equals("null"));
}


TEST_CASE("Trim a string", "[Converter]")
{
  SECTION("Leading")
  {
    std::string_view space{ "     123   " };
    REQUIRE_FALSE(Converter::TrimStart(space).compare("123   "));

    std::string_view none{ "full" };
    REQUIRE_FALSE(Converter::TrimStart(none).compare("full"));

    std::string_view random{ "~~~<" };
    REQUIRE_FALSE(Converter::TrimStart(random, "~").compare("<"));

    std::string_view no_match{ "9876" };
    REQUIRE_FALSE(Converter::TrimStart(no_match, " ").compare("9876"));

    std::string_view empty{ "   " };
    REQUIRE(Converter::TrimStart(empty, " ").empty());

    std::string_view nothing{ "" }; // NOLINT
    REQUIRE(Converter::TrimStart(nothing, " ").empty());
  }

  SECTION("Trailing")
  {
    std::string_view space{ "     123   " };
    REQUIRE_FALSE(Converter::TrimEnd(space).compare("     123"));

    std::string_view none{ "full" };
    REQUIRE_FALSE(Converter::TrimEnd(none).compare("full"));

    std::string_view random{ ">#####" };
    REQUIRE_FALSE(Converter::TrimEnd(random, "#").compare(">"));

    std::string_view no_match{ "9876" };
    REQUIRE_FALSE(Converter::TrimEnd(no_match, " ").compare("9876"));

    std::string_view nothing{ "" }; // NOLINT
    REQUIRE(Converter::TrimEnd(nothing, " ").empty());

    std::string_view empty{ "   " };
    REQUIRE(Converter::TrimEnd(empty, " ").empty());
  }

  SECTION("Surrounding")
  {
    std::string_view space{ "     567   " };
    REQUIRE_FALSE(Converter::TrimString(space).compare("567"));

    std::string_view leading_space{ "     963.2" };
    REQUIRE_FALSE(Converter::TrimString(leading_space).compare("963.2"));

    std::string_view trailing_space{ "abc123   " };
    REQUIRE_FALSE(Converter::TrimString(trailing_space).compare("abc123"));

    std::string_view different_char{ "___asdf___" };
    REQUIRE_FALSE(Converter::TrimString(different_char, "_").compare("asdf"));
  }
}


TEST_CASE("Generic string -> value", "[Converter]")
{
  SECTION("Double")
  {
    std::string_view d_str{ "987.654" };
    REQUIRE(Converter::StringToNum<double>(d_str, 0, 7) == Catch::Approx(987.654));
  }

  SECTION("Float")
  {
    std::string_view f_str{ "abc987.654abc" };
    REQUIRE(Converter::StringToNum<float>(f_str, 3, 10) == Catch::Approx(987.654));
  }

  SECTION("uint8_t")
  {
    std::string_view i8_str{ "123" };
    REQUIRE(Converter::StringToNum<uint8_t>(i8_str, 0, 3) == 123);
  }

  SECTION("uint16_t")
  {
    std::string_view i16_str{ "4563" };
    REQUIRE(Converter::StringToNum<uint16_t>(i16_str, 0, 4) == 4563);
  }

  SECTION("No number")
  {
    std::string_view e_str{ "   *   " };
    REQUIRE(Converter::StringToNum<double>(e_str, 0, 7) == Catch::Approx(std::numeric_limits<double>::max()));

    // Be specific, and verbose, so initialise with empty string
    std::string_view b_str{ "" }; // NOLINT
    REQUIRE(Converter::StringToNum<int>(b_str, 0, 5) == Catch::Approx(std::numeric_limits<int>::max()));
  }
}


TEST_CASE("Substring extraction", "[Converter]")
{
  SECTION("End is before start")
  {
    std::string_view t_string{ "irrelevant" };
    REQUIRE(Converter::NumberAsString(t_string, 2, 1).empty());
  }

  SECTION("Start is after end")
  {
    std::string_view t_string{ "irrelevant" };
    REQUIRE(Converter::NumberAsString(t_string, 5, 4).empty());
  }
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

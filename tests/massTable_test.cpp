#include "nuclear-data-reader/massTable.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Alter the table year", "[MassTable]")
{
  const MassTable table(2003);
  REQUIRE(table.year == 2003);

  REQUIRE(table.setTableYear(2012));
  REQUIRE(table.year == 2012);
}


TEST_CASE("Absolute paths are constructed", "[MassTable]")
{
  REQUIRE_THAT(MassTable::getAbsolutePath(), Catch::Matchers::Contains("nuclear-data-reader/data"));
}


TEST_CASE("Try to read non-existant file", "[MassTable]")
{
  MassTable table(2003);
  table.setFilePaths(2003);

  SECTION("NUBASE") { REQUIRE_FALSE(table.readNUBASE("doesnot.exist")); }
  SECTION("AME") { REQUIRE_FALSE(table.readAME("doesnot.exist")); }
}


TEST_CASE("Validate the requested table year", "[MassTable]")
{
  SECTION("Construction is correct")
  {
    MassTable table(2003);
    REQUIRE(table.validateYear());
    REQUIRE(table.year == 2003);
  }

  SECTION("Construction is correct but ammendment of the year is not")
  {
    MassTable table(2012);
    REQUIRE_FALSE(table.setTableYear(2000));
    REQUIRE(table.year == 2012);
  }

  SECTION("Construction is invalid so the default is set")
  {
    MassTable table(1999);
    REQUIRE(table.year == table.valid_years.back());
  }
}

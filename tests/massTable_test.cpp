#include "nuclear-data-reader/massTable.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Alter the table year", "[MassTable]")
{
  const MassTable table;
  REQUIRE(table.table_year == 3);
  table.setTableYear(12);
  REQUIRE(table.table_year == 12);
}


TEST_CASE("Absolute paths are constructed", "[MassTable]")
{
  REQUIRE_THAT(MassTable::getAbsolutePath(), Catch::Matchers::Contains("nuclear-data-reader/data"));
}


TEST_CASE("Try to read non-existant file", "[MassTable]")
{
  MassTable table;
  table.setFilePaths(3);

  SECTION("NUBASE") { REQUIRE_FALSE(table.readNUBASE("doesnot.exist")); }
  SECTION("AME") { REQUIRE_FALSE(table.readAME("doesnot.exist")); }
}

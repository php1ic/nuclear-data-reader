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
  table.setFilePaths();

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


TEST_CASE("Read a line from the AME mass table as a whole", "[MassTable]")
{
  const std::string line{ "   9   20   11   31 Na    x   12654.768    211.321     7385.492    6.817 B-  15872.148  "
                          "211.668  31 013585.452    226.862" };

  MassTable table(2003);

  auto data = table.parseAMEMassFormat(line);

  REQUIRE(data.A == 31);
  REQUIRE(data.Z == 11);
  REQUIRE(data.N == 20);
  REQUIRE(data.mass_excess == Approx(12654.768));
  REQUIRE(data.dmass_excess == Approx(211.321));
  REQUIRE(data.binding_energy_per_A == Approx(7385.492));
  REQUIRE(data.dbinding_energy_per_A == Approx(6.817));
  REQUIRE(data.beta_decay_energy == Approx(15872.148));
  REQUIRE(data.dbeta_decay_energy == Approx(211.668));
  REQUIRE(data.atomic_mass == Approx(13585.452));
  REQUIRE(data.datomic_mass == Approx(226.862));
}

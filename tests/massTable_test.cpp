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


TEST_CASE("Match up isotopes", "[MassTable]")
{
  SECTION("No existing mass table")
  {
    MassTable table(2012);
    const auto it = table.getMatchingIsotope("", 1);
    REQUIRE(it == table.ameDataTable.end());
  }

  SECTION("Basic mass table with matching isotope")
  {
    const std::string reaction_line{ " 238 U   92   11280.01    1.18  13723#    196#     4269.75    2.94   1144.20    "
                                     "1.22 -10182#    359#    -5635.64    1.19" };
    MassTable table(2012);
    AME::Data ame("");
    ame.A = 238;
    ame.Z = 92;
    table.ameDataTable.emplace_back(ame);

    const auto it = table.getMatchingIsotope(reaction_line, 1);

    REQUIRE(it != table.ameDataTable.end());
    REQUIRE(it->full_data.size() == 125);
  }

  SECTION("Basic mass table with no matching isotope")
  {
    const std::string reaction_line{ " 138 Gd  64   22868#    446#     3545#    196#     3304#    277#   -20838#    "
                                     "627#     4959#    200#   -22845#    627#" };
    MassTable table(2012);
    AME::Data ame("");
    ame.A = 1;
    ame.Z = 1;
    table.ameDataTable.emplace_back(ame);

    const auto it = table.getMatchingIsotope(reaction_line, 1);

    REQUIRE(it == table.ameDataTable.end());
  }
}


TEST_CASE("Read a line from the first AME reaction file as a whole", "[MassTable]")
{
  MassTable table(2003);
  AME::Data ame("");
  ame.A = 1;
  ame.Z = 6;
  table.ameDataTable.emplace_back(ame);

  const std::string reaction_line{ " 152 Tb  65   15756.30   40.69  10504.87   40.49   3153.38   41.32  -7115.94   "
                                   "42.43  -3354.08   40.02 -10036.92   40.17" };

  REQUIRE_FALSE(table.parseAMEReactionOneFormat(reaction_line));

  table.ameDataTable.back().A = 152;
  table.ameDataTable.back().Z = 65;

  REQUIRE(table.parseAMEReactionOneFormat(reaction_line));

  const auto data = table.ameDataTable.back();

  REQUIRE(data.s_2n == Approx(15756.30));
  REQUIRE(data.ds_2n == Approx(40.69));
  REQUIRE(data.s_2p == Approx(10504.87));
  REQUIRE(data.ds_2p == Approx(40.49));
  REQUIRE(data.q_a == Approx(3153.38));
  REQUIRE(data.dq_a == Approx(41.32));
  REQUIRE(data.q_2bm == Approx(-7115.94));
  REQUIRE(data.dq_2bm == Approx(42.43));
  REQUIRE(data.q_ep == Approx(-3354.08));
  REQUIRE(data.dq_ep == Approx(40.02));
  REQUIRE(data.q_bm_n == Approx(-10036.92));
  REQUIRE(data.dq_bm_n == Approx(40.17));
}


TEST_CASE("Read a line from the second AME reaction file as a whole", "[MassTable]")
{
  MassTable table(2003);
  AME::Data ame("");
  ame.A = 1;
  ame.Z = 6;
  table.ameDataTable.emplace_back(ame);

  const std::string reaction_line{ " 136 I   53    3780.98   49.91   8960.63  102.43   6537.65   72.59  13771.00   "
                                   "50.83   8309.36   55.39   5089.71   55.83" };

  REQUIRE_FALSE(table.parseAMEReactionTwoFormat(reaction_line));

  table.ameDataTable.back().A = 136;
  table.ameDataTable.back().Z = 53;

  REQUIRE(table.parseAMEReactionTwoFormat(reaction_line));

  const auto data = table.ameDataTable.back();

  REQUIRE(data.s_n == Approx(3780.98));
  REQUIRE(data.ds_n == Approx(49.91));
  REQUIRE(data.s_p == Approx(8960.63));
  REQUIRE(data.ds_p == Approx(102.43));
  REQUIRE(data.q_4bm == Approx(6537.65));
  REQUIRE(data.dq_4bm == Approx(72.59));
  REQUIRE(data.q_da == Approx(13771.00));
  REQUIRE(data.dq_da == Approx(50.83));
  REQUIRE(data.q_pa == Approx(8309.36));
  REQUIRE(data.dq_pa == Approx(55.39));
  REQUIRE(data.q_na == Approx(5089.71));
  REQUIRE(data.dq_na == Approx(55.83));
}

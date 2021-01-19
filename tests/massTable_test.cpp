#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER

#include "nuclear-data-reader/massTable.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Construct an instance", "[MassTable]")
{
  SECTION("Construct with a valid year")
  {
    const MassTable table(2003);
    REQUIRE(table.year == 2003);
  }

  SECTION("Construct with an invalid year")
  {
    const MassTable table(1999);
    REQUIRE(table.year == table.valid_years.back());
  }
}


TEST_CASE("Alter the table year", "[MassTable]")
{
  SECTION("A valid year is given")
  {
    const MassTable table(2003);

    REQUIRE(table.setTableYear(2012));
    REQUIRE(table.year == 2012);

    REQUIRE(table.setTableYear(2016));
    REQUIRE(table.year == 2016);
  }

  SECTION("An invalid year is given so the original isn't changed")
  {
    const MassTable table(2012);
    REQUIRE(table.year == 2012);

    REQUIRE_FALSE(table.setTableYear(2000));
    REQUIRE(table.year == 2012);
  }
}


TEST_CASE("Absolute paths are constructed", "[MassTable]")
{
  SECTION("2003 data")
  {
    const MassTable table(2003);
    table.setFilePaths();
    REQUIRE_THAT(table.NUBASE_masstable, Catch::Matches(MassTable::getAbsolutePath() / "2003" / "nubtab03.asc"));
    REQUIRE_THAT(table.AME_masstable, Catch::Matches(MassTable::getAbsolutePath() / "2003" / "mass.mas03"));
    REQUIRE_THAT(table.AME_reaction_1, Catch::Matches(MassTable::getAbsolutePath() / "2003" / "rct1.mas03"));
    REQUIRE_THAT(table.AME_reaction_2, Catch::Matches(MassTable::getAbsolutePath() / "2003" / "rct2.mas03"));
  }

  SECTION("2012 data")
  {
    const MassTable table(2012);
    table.setFilePaths();
    REQUIRE_THAT(table.NUBASE_masstable, Catch::Matches(MassTable::getAbsolutePath() / "2012" / "nubtab12.asc"));
    REQUIRE_THAT(table.AME_masstable, Catch::Matches(MassTable::getAbsolutePath() / "2012" / "mass.mas12"));
    REQUIRE_THAT(table.AME_reaction_1, Catch::Matches(MassTable::getAbsolutePath() / "2012" / "rct1.mas12"));
    REQUIRE_THAT(table.AME_reaction_2, Catch::Matches(MassTable::getAbsolutePath() / "2012" / "rct2.mas12"));
  }

  SECTION("2016 data")
  {
    const MassTable table(2016);
    table.setFilePaths();
    REQUIRE_THAT(table.NUBASE_masstable, Catch::Matches(MassTable::getAbsolutePath() / "2016" / "nubase2016.txt"));
    REQUIRE_THAT(table.AME_masstable, Catch::Matches(MassTable::getAbsolutePath() / "2016" / "mass16.txt"));
    REQUIRE_THAT(table.AME_reaction_1, Catch::Matches(MassTable::getAbsolutePath() / "2016" / "rct1-16.txt"));
    REQUIRE_THAT(table.AME_reaction_2, Catch::Matches(MassTable::getAbsolutePath() / "2016" / "rct2-16.txt"));
  }
}


TEST_CASE("Try to read non-existant file", "[MassTable]")
{
  MassTable table(2003);
  table.setFilePaths();

  SECTION("NUBASE") { REQUIRE_FALSE(table.readNUBASE("doesnot.exist")); }
  SECTION("AME mass table") { REQUIRE_FALSE(table.readAME("doesnot.exist")); }
  SECTION("AME first reaction file") { REQUIRE_FALSE(table.readAMEReactionFile("doesnot.exist", 1)); }
  SECTION("AME second reaction file") { REQUIRE_FALSE(table.readAMEReactionFile("doesnot.exist", 2)); }
}


TEST_CASE("Read the NUBASE file", "[MassTable]")
{
  MassTable table(2003);
  table.setFilePaths();

  REQUIRE(table.readNUBASE(table.NUBASE_masstable));
}


TEST_CASE("Read the AME mass file", "[MassTable]")
{
  const MassTable table(2003);
  table.setFilePaths();

  REQUIRE(table.readAME(table.AME_masstable));
}


TEST_CASE("Read the AME reaction files", "[MassTable]")
{
  SECTION("The AME mass table has not been read")
  {
    MassTable table(2003);
    table.setFilePaths();
    REQUIRE(table.readAMEReactionFile(table.AME_reaction_1, 1));
    REQUIRE(table.readAMEReactionFile(table.AME_reaction_2, 2));
  }
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


TEST_CASE("Populate the internal mass table", "[MassTable]")
{
  MassTable table(2003);
  REQUIRE(table.populateInternalMassTable());
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


TEST_CASE("Read the NUBASE format", "[MassTable]")
{
  SECTION("2003 ground-state isotope")
  {
    MassTable table(2003);

    const std::string line{
      "189 0810   189Tl  -24602       11                              2.3    m 0.2    (1/2+)        99           B+=100"
    };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE(nubase.A == 189);
    REQUIRE(nubase.Z == 81);
    REQUIRE(nubase.N == 108);
    REQUIRE_THAT(nubase.symbol, Catch::Matches("Tl"));
    REQUIRE(nubase.level == 0);
    REQUIRE(nubase.mass_excess == Approx(-24602.0));
    REQUIRE(nubase.dmass_excess == Approx(11.0));
    REQUIRE_THAT(nubase.halflife_unit, Catch::Matches("m"));
    REQUIRE(nubase.hl == Converter::minutes{ 2.3 });
    REQUIRE(nubase.hl_error == Converter::minutes{ 0.2 });
    REQUIRE(nubase.J == Approx(0.5));
    REQUIRE(nubase.J_exp == 0);
    REQUIRE(nubase.J_tent == 1);
    REQUIRE_FALSE(nubase.decay.compare("B+"));
  }

  SECTION("2003 first isomeric state")
  {
    MassTable table(2003);

    const std::string line{ "018 0091   18Fxm    1995.1      0.5    1121.36    0.15       234     ns        5+" };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE(nubase.A == 18);
    REQUIRE(nubase.Z == 9);
    REQUIRE(nubase.N == 9);
    REQUIRE_THAT(nubase.symbol, Catch::Matches("F"));
    REQUIRE(nubase.level == 1);
  }

  SECTION("2003 stable ground state")
  {
    MassTable table(2003);

    const std::string line{ "124 0500   124Sn  -88236.8      1.4                          stbl       >100Py 0+         "
                            "   97 52Ka41t   IS=5.79 5;2B- ?" };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE(nubase.A == 124);
    REQUIRE(nubase.Z == 50);
    REQUIRE(nubase.N == 74);
    REQUIRE_THAT(nubase.symbol, Catch::Matches("Sn"));
    REQUIRE(nubase.level == 0);
    REQUIRE(nubase.mass_excess == Approx(-88236.8));
    REQUIRE(nubase.dmass_excess == Approx(1.4));
    REQUIRE_THAT(nubase.halflife_unit, Catch::Matches(""));
    REQUIRE(nubase.hl == Converter::seconds{ 1.0e24 });
    REQUIRE(nubase.hl_error == Converter::seconds{ 1.0 });
    REQUIRE(nubase.J == Approx(0.0));
    REQUIRE(nubase.J_exp == 0);
    REQUIRE(nubase.J_tent == 0);
    REQUIRE_FALSE(nubase.decay.compare("stable"));
    REQUIRE(table.pnSide.at(50));
  }


  SECTION("2012 ground-state isotope")
  {
    MassTable table(2012);

    const std::string line{ "028 0130   28Al   -16850.53     0.12                           2.2414 m 0.0012 3+         "
                            "   01          1934 B-=100" };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE(nubase.year == 1934);
  }
}

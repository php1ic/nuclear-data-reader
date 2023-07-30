#include "nuclear-data-reader/massTable.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>


TEST_CASE("Construct an instance", "[MassTable]")
{
  SECTION("Construct with the valid years")
  {
    for (auto theYear : MassTable::valid_years)
      {
        const MassTable table(theYear);
        // The constructor will change 1997 to 1995 so we need to manually alter the value so the check will pass
        if (theYear == 1997)
          {
            theYear = 1995;
          }
        REQUIRE(table.year == theYear);
      }
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
  SECTION("1983 data")
  {
    constexpr int year = 1983;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    // REQUIRE(table.NUBASE_masstable.string() == (root / "nubtab03.asc").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass.mas83").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1.mas83").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2.mas83").string());
  }

  SECTION("1993 data")
  {
    constexpr int year = 1993;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    // REQUIRE(table.NUBASE_masstable.string() == (root / "nubtab03.asc").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass_exp.mas93").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1_exp.mas93").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2_exp.mas93").string());
  }

  SECTION("1995/1997 data")
  {
    constexpr int year = 1995;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    REQUIRE(table.NUBASE_masstable.string() == (root / "nubtab97.asc").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass_exp.mas95").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1_exp.mas95").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2_exp.mas95").string());
  }

  SECTION("2003 data")
  {
    constexpr int year = 2003;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    REQUIRE(table.NUBASE_masstable.string() == (root / "nubtab03.asc").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass.mas03").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1.mas03").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2.mas03").string());
  }

  SECTION("2012 data")
  {
    constexpr int year = 2012;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    REQUIRE(table.NUBASE_masstable.string() == (root / "nubtab12.asc").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass.mas12").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1.mas12").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2.mas12").string());
  }

  SECTION("2016 data")
  {
    constexpr int year = 2016;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    REQUIRE(table.NUBASE_masstable.string() == (root / "nubase2016.txt").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass16.txt").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1-16.txt").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2-16.txt").string());
  }

  SECTION("2020 data")
  {
    constexpr int year = 2020;
    const MassTable table(year);

    table.setFilePaths();
    const std::filesystem::path root = std::filesystem::path{ NDR_DATA_PATH } / std::to_string(year);

    REQUIRE(table.NUBASE_masstable.string() == (root / "nubase_1.mas20").string());
    REQUIRE(table.AME_masstable.string() == (root / "mass.mas20").string());
    REQUIRE(table.AME_reaction_1.string() == (root / "rct1.mas20").string());
    REQUIRE(table.AME_reaction_2.string() == (root / "rct2.mas20").string());
  }
}


TEST_CASE("Try to read non-existent file", "[MassTable]")
{
  MassTable table(2003);
  table.setFilePaths();

  SECTION("NUBASE")
  {
    REQUIRE_FALSE(table.readNUBASE("doesnot.exist"));
  }
  SECTION("AME mass table")
  {
    REQUIRE_FALSE(table.readAMEMassFile("doesnot.exist"));
  }
  SECTION("AME first reaction file")
  {
    REQUIRE_FALSE(table.readAMEReactionFileOne("doesnot.exist"));
  }
  SECTION("AME second reaction file")
  {
    REQUIRE_FALSE(table.readAMEReactionFileTwo("doesnot.exist"));
  }
}


TEST_CASE("Read the NUBASE file", "[MassTable]")
{
  SECTION("File instance with no header")
  {
    MassTable table(2003);
    table.setFilePaths();

    REQUIRE(table.readNUBASE(table.NUBASE_masstable));
  }

  SECTION("File instance with a header")
  {
    MassTable table(2020);
    table.setFilePaths();

    REQUIRE(table.readNUBASE(table.NUBASE_masstable));
  }
}


TEST_CASE("Read the AME mass file", "[MassTable]")
{
  const MassTable table(2003);
  table.setFilePaths();

  REQUIRE(table.readAMEMassFile(table.AME_masstable));
}


TEST_CASE("Read the AME reaction files", "[MassTable]")
{
  SECTION("The AME mass table has not been read")
  {
    const MassTable table(2003);
    table.setFilePaths();
    REQUIRE(table.readAMEReactionFileOne(table.AME_reaction_1));
    REQUIRE(table.readAMEReactionFileTwo(table.AME_reaction_2));
  }

  SECTION("The repeated header in reaction file number 2 from 2020 is skipped")
  {
    const MassTable table(2020);
    table.setFilePaths();
    REQUIRE(table.readAMEReactionFileTwo(table.AME_reaction_2));
  }
}


TEST_CASE("Validate the requested table year", "[MassTable]")
{
  SECTION("Construction is correct")
  {
    const MassTable table(2003);
    REQUIRE(table.ValidYear());
    REQUIRE(table.year == 2003);
  }

  SECTION("Construction is correct but amendment of the year is not")
  {
    const MassTable table(2012);
    REQUIRE_FALSE(table.setTableYear(2000));
    REQUIRE(table.year == 2012);
  }

  SECTION("Construction is invalid so the default is set")
  {
    const MassTable table(1999);
    REQUIRE(table.year == table.valid_years.back());
  }
}


TEST_CASE("Validate user input table year", "[MassTable]")
{
  SECTION("A correct year is given")
  {
    const MassTable table(2003);
    REQUIRE(table.ValidYear(2012));
  }

  SECTION("An unknown year is given")
  {
    const MassTable table(2012);
    REQUIRE_FALSE(table.ValidYear(1999));
  }
}

TEST_CASE("Populate the internal mass table", "[MassTable]")
{
  SECTION("Before the first NUBASE table")
  {
    MassTable table(1993);
    REQUIRE(table.populateInternalMassTable());
  }

  SECTION("After the first NUBASE table")
  {
    MassTable table(2003);
    REQUIRE(table.populateInternalMassTable());
  }
}


TEST_CASE("Read a line from the AME mass table as a whole", "[MassTable]")
{
  const std::string line{ "   9   20   11   31 Na    x   12654.768    211.321     7385.492    6.817 B-  15872.148  "
                          "211.668  31 013585.452    226.862" };

  const MassTable table(2003);

  const auto data = table.parseAMEMassFormat(line);

  REQUIRE(data.A == 31);
  REQUIRE(data.Z == 11);
  REQUIRE(data.N == 20);
  REQUIRE(data.mass_excess == Catch::Approx(12654.768));
  REQUIRE(data.dmass_excess == Catch::Approx(211.321));
  REQUIRE(data.binding_energy_per_A == Catch::Approx(7385.492));
  REQUIRE(data.dbinding_energy_per_A == Catch::Approx(6.817));
  REQUIRE(data.beta_decay_energy == Catch::Approx(15872.148));
  REQUIRE(data.dbeta_decay_energy == Catch::Approx(211.668));
  REQUIRE(data.atomic_mass == Catch::Approx(13585.452));
  REQUIRE(data.datomic_mass == Catch::Approx(226.862));
}


TEST_CASE("Match up isotopes", "[MassTable]")
{
  SECTION("No existing mass table")
  {
    const MassTable table(2012);
    const auto it = table.getMatchingIsotope("", 1, 1);
    REQUIRE(it == table.ameDataTable.end());
  }

  SECTION("Basic mass table with matching isotope")
  {
    const std::string reaction_line{ " 238 U   92   11280.01    1.18  13723#    196#     4269.75    2.94   1144.20    "
                                     "1.22 -10182#    359#    -5635.64    1.19" };
    MassTable table(2012);
    AME::Data ame("", 2012);
    ame.A = 238;
    ame.Z = 92;
    table.ameDataTable.emplace_back(ame);

    const auto it = table.getMatchingIsotope(reaction_line, 238, 92);

    REQUIRE(it != table.ameDataTable.end());
    REQUIRE(it->full_data.size() == 125);
  }

  SECTION("Basic mass table with no matching isotope")
  {
    const std::string reaction_line{ " 138 Gd  64   22868#    446#     3545#    196#     3304#    277#   -20838#    "
                                     "627#     4959#    200#   -22845#    627#" };
    MassTable table(2012);
    AME::Data ame("", 2012);
    ame.A = 1;
    ame.Z = 1;
    table.ameDataTable.emplace_back(ame);

    const auto it = table.getMatchingIsotope(reaction_line, 138, 64);

    REQUIRE(it == table.ameDataTable.end());
  }
}


TEST_CASE("Read a line from the first AME reaction file as a whole", "[MassTable]")
{
  MassTable table(2003);
  AME::Data ame("", 2003);
  ame.A = 1;
  ame.Z = 6;
  table.ameDataTable.emplace_back(ame);

  const std::string reaction_line{ " 152 Tb  65   15756.30   40.69  10504.87   40.49   3153.38   41.32  -7115.94   "
                                   "42.43  -3354.08   40.02 -10036.92   40.17" };

  REQUIRE_FALSE(table.parseAMEReactionOneFormat(reaction_line, 152, 65));

  table.ameDataTable.back().A = 152;
  table.ameDataTable.back().Z = 65;

  REQUIRE(table.parseAMEReactionOneFormat(reaction_line, 152, 65));

  const auto data = table.ameDataTable.back();

  REQUIRE(data.s_2n == Catch::Approx(15756.30));
  REQUIRE(data.ds_2n == Catch::Approx(40.69));
  REQUIRE(data.s_2p == Catch::Approx(10504.87));
  REQUIRE(data.ds_2p == Catch::Approx(40.49));
  REQUIRE(data.q_a == Catch::Approx(3153.38));
  REQUIRE(data.dq_a == Catch::Approx(41.32));
  REQUIRE(data.q_2bm == Catch::Approx(-7115.94));
  REQUIRE(data.dq_2bm == Catch::Approx(42.43));
  REQUIRE(data.q_ep == Catch::Approx(-3354.08));
  REQUIRE(data.dq_ep == Catch::Approx(40.02));
  REQUIRE(data.q_bm_n == Catch::Approx(-10036.92));
  REQUIRE(data.dq_bm_n == Catch::Approx(40.17));
}


TEST_CASE("Read a line from the second AME reaction file as a whole", "[MassTable]")
{
  MassTable table(2003);
  AME::Data ame("", 2003);
  ame.A = 1;
  ame.Z = 6;
  table.ameDataTable.emplace_back(ame);

  const std::string reaction_line{ " 136 I   53    3780.98   49.91   8960.63  102.43   6537.65   72.59  13771.00   "
                                   "50.83   8309.36   55.39   5089.71   55.83" };

  REQUIRE_FALSE(table.parseAMEReactionTwoFormat(reaction_line, 136, 53));

  table.ameDataTable.back().A = 136;
  table.ameDataTable.back().Z = 53;

  REQUIRE(table.parseAMEReactionTwoFormat(reaction_line, 136, 53));

  const auto data = table.ameDataTable.back();

  REQUIRE(data.s_n == Catch::Approx(3780.98));
  REQUIRE(data.ds_n == Catch::Approx(49.91));
  REQUIRE(data.s_p == Catch::Approx(8960.63));
  REQUIRE(data.ds_p == Catch::Approx(102.43));
  REQUIRE(data.q_4bm == Catch::Approx(6537.65));
  REQUIRE(data.dq_4bm == Catch::Approx(72.59));
  REQUIRE(data.q_da == Catch::Approx(13771.00));
  REQUIRE(data.dq_da == Catch::Approx(50.83));
  REQUIRE(data.q_pa == Catch::Approx(8309.36));
  REQUIRE(data.dq_pa == Catch::Approx(55.39));
  REQUIRE(data.q_na == Catch::Approx(5089.71));
  REQUIRE(data.dq_na == Catch::Approx(55.83));
}


TEST_CASE("Read the NUBASE format", "[MassTable]")
{
  SECTION("2003 ground-state isotope")
  {
    MassTable table(2003);

    const std::string line{ "189 0810   189Tl  -24602       11                              2.3    m 0.2    (1/2+)   "
                            "     99           B+=100" };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE(nubase.A == 189);
    REQUIRE(nubase.Z == 81);
    REQUIRE(nubase.N == 108);
    REQUIRE_THAT(nubase.symbol, Catch::Matchers::Matches("Tl"));
    REQUIRE(nubase.level == 0);
    REQUIRE(nubase.mass_excess == Catch::Approx(-24602.0));
    REQUIRE(nubase.dmass_excess == Catch::Approx(11.0));
    REQUIRE_THAT(nubase.halflife_unit, Catch::Matchers::Matches("m"));
    REQUIRE(nubase.hl == Converter::minutes{ 2.3 });
    REQUIRE(nubase.hl_error == Converter::minutes{ 0.2 });
    REQUIRE(nubase.J == Catch::Approx(0.5));
    REQUIRE(nubase.J_exp == NUBASE::Measured::THEORETICAL);
    REQUIRE(nubase.pi == NUBASE::Parity::POSITIVE);
    REQUIRE(nubase.pi_exp == NUBASE::Measured::THEORETICAL);
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
    REQUIRE_THAT(nubase.symbol, Catch::Matchers::Matches("F"));
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
    REQUIRE_THAT(nubase.symbol, Catch::Matchers::Matches("Sn"));
    REQUIRE(nubase.level == 0);
    REQUIRE(nubase.mass_excess == Catch::Approx(-88236.8));
    REQUIRE(nubase.dmass_excess == Catch::Approx(1.4));
    REQUIRE_THAT(nubase.halflife_unit, Catch::Matchers::Matches(""));
    REQUIRE(nubase.hl == Converter::seconds{ 1.0e24 });
    REQUIRE(nubase.hl_error == Converter::seconds{ 1.0 });
    REQUIRE(nubase.J == Catch::Approx(0.0));
    REQUIRE(nubase.J_exp == NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(nubase.pi == NUBASE::Parity::POSITIVE);
    REQUIRE(nubase.pi_exp == NUBASE::Measured::EXPERIMENTAL);
    REQUIRE_FALSE(nubase.decay.compare("stable"));
    REQUIRE(table.neutron_rich.at(50));
  }


  SECTION("2012 ground-state isotope")
  {
    MassTable table(2012);

    const std::string line{ "028 0130   28Al   -16850.53     0.12                           2.2414 m 0.0012 3+         "
                            "   01          1934 B-=100" };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE(nubase.year == 1934);
  }

  SECTION("Bad Z value read")
  {
    MassTable table(2003);

    // Manually alter Z value to be out of acceptable range
    const std::string line{ "221 3000   221Fr   13278        5                              4.9    m 0.2    5/2-       "
                            "   90 97Ch53d   A~100;B-=0.0048 15;14C=8.8e-11 11" };

    const auto nubase = table.parseNUBASEFormat(line);

    REQUIRE_THAT(nubase.symbol, Catch::Matchers::Matches("Xx"));
  }
}


TEST_CASE("Merge AME and NUBASE data", "[MassTable]")
{
  // Not sure about the test in this case.

  // We are testing that we fall into the print (to screen) warning condition
  // Can we catch what is printe to screen and validate the text?
  SECTION("NUBASE table is a different size to the AME table")
  {
    MassTable table(2016);
    NUBASE::Data nubase_1("", 2016);
    NUBASE::Data nubase_2("", 2016);
    table.nubaseDataTable.emplace_back(nubase_1);
    table.nubaseDataTable.emplace_back(nubase_2);
    AME::Data ame_1("", 2016);
    table.ameDataTable.emplace_back(ame_1);

    REQUIRE(table.mergeData());
  }

  // We are testing that if no matching isotope is found, we print it's details to screen
  // Again, can we catch what is being printed and validate?
  SECTION("No matching isotope is found")
  {
    MassTable table(2012);
    NUBASE::Data nubase_1("", 2012);
    nubase_1.A = 50;
    nubase_1.Z = 25;
    NUBASE::Data nubase_2("", 2012);
    nubase_2.A = 100;
    nubase_2.Z = 60;
    table.nubaseDataTable.emplace_back(nubase_1);
    table.nubaseDataTable.emplace_back(nubase_2);
    AME::Data ame_1("", 2012);
    ame_1.A = 5;
    ame_1.Z = 2;
    table.ameDataTable.emplace_back(ame_1);

    REQUIRE(table.mergeData());
  }
}


TEST_CASE("Output a json file", "[MassTable]")
{
  // Not sure about this test
  // We are just testing that the code runs, there is no check it does what we want it to do
  MassTable table(2016);
  NUBASE::Data nubase_1("", 2016);
  nubase_1.A           = 50;
  nubase_1.Z           = 25;
  nubase_1.mass_excess = 123.456;
  table.nubaseDataTable.emplace_back(nubase_1);
  AME::Data ame_1("", 2012);
  ame_1.A           = 50;
  ame_1.Z           = 25;
  ame_1.mass_excess = 124.689;
  table.ameDataTable.emplace_back(ame_1);

  [[maybe_unused]] const auto i = table.mergeData();

  REQUIRE(table.outputTableToJSON());
}

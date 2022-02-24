#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER

#include "nuclear-data-reader/nubase_data.hpp"

#include <catch2/catch.hpp>


// TEST_CASE("", "[Isotope]")
//{
//}


TEST_CASE("Set Symbol", "[NUBASEData]")
{
  NUBASE::Data gs03_isotope("", 2003);

  gs03_isotope.setSymbol("Pb");
  REQUIRE_THAT(gs03_isotope.symbol, Catch::Matches("Pb"));
}


TEST_CASE("Set State", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    SECTION("Ground state")
    {
      const std::string gs03{ "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)  "
                              "     99 94Yo01tj  n=100" };
      NUBASE::Data gs03_isotope(gs03, 2003);

      gs03_isotope.setState();
      REQUIRE(gs03_isotope.level == 0);
    }

    SECTION("First Isomeric state")
    {
      const std::string isomer03{ "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+   "
                                  "            97Zi04t   IT=100" };
      NUBASE::Data isomer03_isotope(isomer03, 2003);

      auto anotherIsomer = std::move(isomer03_isotope);

      anotherIsomer.setState();
      REQUIRE(anotherIsomer.level == 1);
    }

    SECTION("An isomeric state is added to the correct ground state")
    {
      std::vector<NUBASE::Data> table;

      const std::string gs03{ "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)  "
                              "     99 94Yo01tj  n=100" };
      NUBASE::Data gs03_isotope(gs03, 2003);

      gs03_isotope.setA();
      gs03_isotope.setZ();
      gs03_isotope.setState();

      table.emplace_back(gs03_isotope);

      const std::string isomer03{ "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+   "
                                  "            97Zi04t   IT=100" };
      NUBASE::Data isomer03_isotope(isomer03, 2003);

      isomer03_isotope.setA();
      isomer03_isotope.setZ();
      isomer03_isotope.setState();

      isomer03_isotope.setIsomerData(table);
      REQUIRE(table.front().energy_levels.size() == 1);
      REQUIRE(table.front().energy_levels.front().energy == Approx(200));
      REQUIRE(table.front().energy_levels.front().error == Approx(40));
      REQUIRE(table.front().energy_levels.front().level == 1);
    }
  }

  SECTION("Post 2020")
  {
    SECTION("Ground state")
    {
      const std::string gs20{ "095 0360   95Kr   -56159         19                                   114     ms 3      "
                              "1/2+*         10          1994 B-=100;B-n=2.87 18;B-2n ?" };
      NUBASE::Data gs20_isotope(gs20, 2020);

      gs20_isotope.setState();
      REQUIRE(gs20_isotope.level == 0);
    }

    SECTION("First Isomeric state")
    {
      const std::string isomer20{ "202 0821   202Pbm -23771          4        2169.85       0.08           3.54   h "
                                  "0.02   9-            08          1954 IT=90.5 5;B+=9.5 5" };
      NUBASE::Data isomer20_isotope(isomer20, 2020);
      isomer20_isotope.setState();
      REQUIRE(isomer20_isotope.level == 1);
    }

    SECTION("An isomeric state is added to the correct ground state")
    {
      std::vector<NUBASE::Data> table;

      const std::string gs20{ "265 1080   265Hs  120900         24                                     1.96  ms 0.16   "
                              "3/2+#         99          1984 A~100;SF ?" };

      NUBASE::Data gs20_isotope(gs20, 2020);

      gs20_isotope.setA();
      gs20_isotope.setZ();
      gs20_isotope.setState();

      table.emplace_back(gs20_isotope);

      const std::string isomer20{ "265 1081   265Hsm 121130         24         229         22       AD   360     us "
                                  "150    11/2-#        99          1995 A~100;IT ?" };
      NUBASE::Data isomer20_isotope(isomer20, 2020);

      isomer20_isotope.setA();
      isomer20_isotope.setZ();
      isomer20_isotope.setState();

      isomer20_isotope.setIsomerData(table);
      REQUIRE(table.front().energy_levels.size() == 1);
      REQUIRE(table.front().energy_levels.front().energy == Approx(229));
      REQUIRE(table.front().energy_levels.front().error == Approx(22));
      REQUIRE(table.front().energy_levels.front().level == 1);
    }
  }
}


TEST_CASE("State object is created correctly", "[NUBASEData]")
{
  auto level = NUBASE::Data::State(0, 1.2345, 0.321);

  REQUIRE(level.level == 0);
  REQUIRE(level.energy == Approx(1.2345));
  REQUIRE(level.error == Approx(0.321));
}


TEST_CASE("Set IsomerEnergy", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data isomer03_isotope("084 0351   84Brm  -77460      100       340     100     BD     6.0    m 0.2    (6-) "
                                  "         97           B-=100",
                                  2003);

    REQUIRE(isomer03_isotope.setIsomerEnergy() == Approx(340.0));
  }

  SECTION("Post 2020")
  {
    NUBASE::Data isomer20_isotope("114 0431   114Tcm -58437         13         160        430       MD*   90     ms 20 "
                                  "    1+#           12          2011 B-~100;IT ?;B-n=1.3 4",
                                  2020);

    REQUIRE(isomer20_isotope.setIsomerEnergy() == Approx(160));
  }
}


TEST_CASE("Set IsomerEnergyError", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data isomer03_isotope("206 0811   206Tlm -19610.0      1.4    2643.11    0.19         3.74   m 0.03   "
                                  "(12-)         99           IT=100",
                                  2003);

    REQUIRE(isomer03_isotope.setIsomerEnergyError() == Approx(0.19));
  }

  SECTION("Post 2020")
  {
    NUBASE::Data isomer20_isotope("136 0621   136Smm -64546         12        2264.7        1.1           15     us 1  "
                                  "    (8-)          19          1994 IT=100",
                                  2020);

    REQUIRE(isomer20_isotope.setIsomerEnergyError() == Approx(1.1));
  }
}


TEST_CASE("Read and set measured or theoretical", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope(
        "115 0470   115Ag  -84990       30                             20.0    m 0.5    1/2-       "
        "   99           B-=100",
        2003);

    NUBASE::Data theoretical03_isotope(
        "014 0090   14F     32660#     400#                                             2-#                        p ?",
        2003);

    gs03_isotope.setExperimental();
    REQUIRE(gs03_isotope.exp == NUBASE::Measured::EXPERIMENTAL);
    gs03_isotope.setExperimental(NUBASE::Measured::THEORETICAL);
    REQUIRE(gs03_isotope.exp == NUBASE::Measured::THEORETICAL);

    theoretical03_isotope.setExperimental();
    REQUIRE(theoretical03_isotope.exp == NUBASE::Measured::THEORETICAL);
    theoretical03_isotope.setExperimental(NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(theoretical03_isotope.exp == NUBASE::Measured::EXPERIMENTAL);
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("226 0870   226Fr   27521          6                                    48.5    s 0.7    "
                              "1-*           96          1969 B-=100",
                              2020);

    NUBASE::Data theoretical20_isotope("065 0240   65Cr   -28310#       200#                                   27.5   "
                                       "ms 2.1    1/2-#         10          1997 B-=100;B-n ?;B-2n ?",
                                       2020);

    gs20_isotope.setExperimental();
    REQUIRE(gs20_isotope.exp == NUBASE::Measured::EXPERIMENTAL);
    gs20_isotope.setExperimental(NUBASE::Measured::THEORETICAL);
    REQUIRE(gs20_isotope.exp == NUBASE::Measured::THEORETICAL);

    theoretical20_isotope.setExperimental();
    REQUIRE(theoretical20_isotope.exp == NUBASE::Measured::THEORETICAL);
    theoretical20_isotope.setExperimental(NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(theoretical20_isotope.exp == NUBASE::Measured::EXPERIMENTAL);
  }
}


TEST_CASE("Clean complicated spin parity format ", "[NUBASEData]")
{
  SECTION("131I 2nd isomer")
  {
    NUBASE::Data iso("131 0492   131Inn -64040       70      4100      70     BD   320     ms 60     (19/2+..23/2+)94  "
                     "         B->99;B-n=0.028 5;IT<1",
                     2003);
    std::string jpi = iso.full_data.substr(iso.position.START_SPIN, (iso.position.END_SPIN - iso.position.START_SPIN));
    // Remove all white space
    jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const char c) { return std::isspace(c); }), jpi.end());

    REQUIRE(iso.cleanSpinParityString(jpi) == "(19/2+)");
  }

  SECTION("118Rh")
  {
    NUBASE::Data iso("118 0450   118Rh  -65140#     500#                           310     ms 30     (4-10)(+#)    97 "
                     "00Jo18tjd B-=100",
                     2003);
    std::string jpi = iso.full_data.substr(iso.position.START_SPIN, (iso.position.END_SPIN - iso.position.START_SPIN));
    // Remove all white space
    jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const char c) { return std::isspace(c); }), jpi.end());

    REQUIRE(iso.cleanSpinParityString(jpi) == "(4)(+#)");
  }

  SECTION("176Ta 1st isomer")
  {
    NUBASE::Data iso("176 0731   176Tam -51270       30       103.0     1.0          1.1   ms 0.1    (+)           98  "
                     "         IT=100",
                     2003);
    std::string jpi = iso.full_data.substr(iso.position.START_SPIN, (iso.position.END_SPIN - iso.position.START_SPIN));
    // Remove all white space
    jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const char c) { return std::isspace(c); }), jpi.end());

    REQUIRE(iso.cleanSpinParityString(jpi) == "(0+)");
  }

  SECTION("71Se 1st isomer")
  {
    NUBASE::Data iso("071 0341   71Sem  -63070       30        48.79    0.05         5.6   us 0.7    1/2- to 9/2-  93  "
                     "         IT=100",
                     2003);
    std::string jpi = iso.full_data.substr(iso.position.START_SPIN, (iso.position.END_SPIN - iso.position.START_SPIN));
    // Remove all white space
    jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const char c) { return std::isspace(c); }), jpi.end());

    REQUIRE(iso.cleanSpinParityString(jpi) == "1/2-");
  }

  SECTION("42Sc 5th isomer")
  {
    NUBASE::Data iso("042 0215   42Scr  -26044.91     0.26   6076.33    0.08  RQ                     (1+ to 4+)    01  "
                     "         IT=100",
                     2003);
    std::string jpi = iso.full_data.substr(iso.position.START_SPIN, (iso.position.END_SPIN - iso.position.START_SPIN));
    // Remove all white space
    jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const char c) { return std::isspace(c); }), jpi.end());

    REQUIRE(iso.cleanSpinParityString(jpi) == "(1+)");
  }

  SECTION("142Ho")
  {
    NUBASE::Data iso("142 0670   142Ho  -37470#     500#                           400     ms 100    (6 to 9)      02 "
                     "B+~100;B+p=?;p~0",
                     2003);
    std::string jpi = iso.full_data.substr(iso.position.START_SPIN, (iso.position.END_SPIN - iso.position.START_SPIN));
    // Remove all white space
    jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const char c) { return std::isspace(c); }), jpi.end());

    REQUIRE(iso.cleanSpinParityString(jpi) == "(6)");
  }
}


TEST_CASE("Set all spin and parity values to unknown", "[NUBASEData]")
{
  NUBASE::Data iso("", 2003);
  iso.J      = 5.0;
  iso.J_exp  = NUBASE::Measured::EXPERIMENTAL;
  iso.pi     = NUBASE::Parity::POSITIVE;
  iso.pi_exp = NUBASE::Measured::EXPERIMENTAL;

  iso.setAllSpinParityValuesAsUnknown();

  REQUIRE(iso.J == Approx(100.0));
  REQUIRE(iso.J_exp == NUBASE::Measured::THEORETICAL);
  REQUIRE(iso.pi == NUBASE::Parity::UNKNOWN);
  REQUIRE(iso.pi_exp == NUBASE::Measured::THEORETICAL);
}


TEST_CASE("Read spin parity of the state", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope(
        "028 0110   28Na     -989       13                             30.5   ms 0.4    1+         "
        "   01           B-=100;B-n=0.58 12",
        2003);

    NUBASE::Data isomer03_isotope(
        "077 0321   77Gem  -71054.3      1.7     159.70    0.10        52.9    s 0.6    1/2-   "
        "       97           B-=81 2;IT=19 2",
        2003);

    gs03_isotope.setSpinParity();

    REQUIRE(gs03_isotope.J == Approx(1.0));
    REQUIRE(gs03_isotope.J_exp == NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(gs03_isotope.pi == NUBASE::Parity::POSITIVE);
    REQUIRE(gs03_isotope.pi_exp == NUBASE::Measured::EXPERIMENTAL);

    isomer03_isotope.setSpinParity();

    REQUIRE(isomer03_isotope.J == Approx(0.5));
    REQUIRE(isomer03_isotope.J_exp == NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(isomer03_isotope.pi == NUBASE::Parity::NEGATIVE);
    REQUIRE(isomer03_isotope.pi_exp == NUBASE::Measured::EXPERIMENTAL);
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("182 0760   182Os  -44609         22                                    21.84   h 0.20   "
                              "0+            15          1950 EC=100",
                              2020);

    NUBASE::Data isomer20_isotope("080 0391   80Y  m -60920          6         228.5        0.1            4.8    s "
                                  "0.3    1-            05          1998 IT=81 2;B+=19 2",
                                  2020);

    gs20_isotope.setSpinParity();

    REQUIRE(gs20_isotope.J == Approx(0));
    REQUIRE(gs20_isotope.J_exp == NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(gs20_isotope.pi == NUBASE::Parity::POSITIVE);
    REQUIRE(gs20_isotope.pi_exp == NUBASE::Measured::EXPERIMENTAL);

    isomer20_isotope.setSpinParity();

    REQUIRE(isomer20_isotope.J == Approx(1));
    REQUIRE(isomer20_isotope.J_exp == NUBASE::Measured::EXPERIMENTAL);
    REQUIRE(isomer20_isotope.pi == NUBASE::Parity::NEGATIVE);
    REQUIRE(isomer20_isotope.pi_exp == NUBASE::Measured::EXPERIMENTAL);
  }
}


TEST_CASE("Read half-life unit", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope("205 0820   205Pb  -23770.1      1.2                           15.3   My 0.7    5/2-     "
                              "     93           EC=100",
                              2003);

    gs03_isotope.setHalfLifeUnit();

    REQUIRE_THAT(gs03_isotope.halflife_unit, Catch::Matches("My"));
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("214 0860   214Rn   -4320          9                                   259     ns 3      "
                              "0+            09          1970 A=100",
                              2020);


    gs20_isotope.setHalfLifeUnit();

    REQUIRE_THAT(gs20_isotope.halflife_unit, Catch::Matches("ns"));
  }
}


TEST_CASE("Read half-life error", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope("140 0560   140Ba  -83271        8                             12.752  d 0.003  0+       "
                              "     98           B-=100",
                              2003);

    gs03_isotope.setHalfLife();
    auto hl_error = Converter::days{ 0.003 };

    REQUIRE(gs03_isotope.hl_error == hl_error);
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("249 0990   249Es   71180#        30#                                  102.2    m 0.6    "
                              "7/2+          11          1956 B+~100;A=0.57 8",
                              2020);

    gs20_isotope.setHalfLife();
    auto hl_error = Converter::minutes{ 0.6 };

    REQUIRE(gs20_isotope.hl_error == hl_error);
  }
}


TEST_CASE("Read and set half-life to double", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    SECTION("yoctoseconds")
    {
      NUBASE::Data data("012 0080   12O     32048       18                            580     ys 30     0+            "
                        "00 95Kr03t   2p=60 30;B+ ?",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::attoseconds{ 580 * 1.0e-6 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("zeptoseconds")
    {
      NUBASE::Data data("010 0020   10He    48810       70                              2.7   zs 1.8    0+            "
                        "99 94Os04t   2n=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::attoseconds{ 2.7 * 1.0e-3 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("attoseconds")
    {
      NUBASE::Data data("008 0040   8Be      4941.67     0.04                          67     as 17     0+            "
                        "99           A=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::attoseconds{ 67 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("picoseconds")
    {
      NUBASE::Data data("016 0050   16B     37080       60                           <190     ps        0-            "
                        "99           n ?",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::picoseconds{ 190 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("nanoseconds")
    {
      NUBASE::Data data("208 0821   208Pbm -16853.5      2.3    4895       2          500     ns 10     10+           "
                        "86 98Pf02t   IT=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::nanoseconds{ 500 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("microseconds")
    {
      NUBASE::Data data("220 0900   220Th   14669       22                              9.7   us 0.6    0+            "
                        "97           A=100;EC=2e-7#",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::microseconds{ 9.7 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("milliseconds")
    {
      NUBASE::Data data("056 0230   56V    -46080      200                            216     ms 4      (1+)          "
                        "99 03Ma02tj  B-=100;B-n ?",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::milliseconds{ 216 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("seconds")
    {
      NUBASE::Data data("180 0780   180Pt  -34436       11                             52      s 3      0+            "
                        "94           B+~100;A~0.3",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::seconds{ 52 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("hours")
    {
      NUBASE::Data data("076 0350   76Br   -70289        9                             16.2    h 0.2    1-            "
                        "95           B+=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::hours{ 16.2 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("years")
    {
      NUBASE::Data data("207 0830   207Bi  -20054.4      2.4                           32.9    y 1.4    9/2-          "
                        "94           B+=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::years{ 32.9 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("kilo years")
    {
      NUBASE::Data data("208 0830   208Bi  -18870.0      2.4                          368     ky 4      (5)+          "
                        "86           B+=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::kiloyears{ 368 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Mega years")
    {
      NUBASE::Data data("129 0530   129I   -88503        3                             15.7   My 0.4    7/2+          "
                        "96           B-=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::millionyears{ 15.7 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Giga years")
    {
      NUBASE::Data data("187 0750   187Re  -41215.7      1.4                           41.2   Gy 0.2    5/2+          "
                        "91 01Ga01t   IS=62.60 2;B-=100;A<0.0001",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 41.2 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Tera years")
    {
      NUBASE::Data data("115 0490   115In  -89537        4                            441     Ty 25     9/2+          "
                        "99           IS=95.71 5;B-=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 441 * 1.0e3 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Peta years")
    {
      NUBASE::Data data("174 0720   174Hf  -55846.6      2.8                            2.0   Py 0.4    0+            "
                        "99           IS=0.16 1;A=100;2B+ ?",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 2.0 * 1.0e6 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Exa years")
    {
      NUBASE::Data data("150 0600   150Nd  -73690        3                              6.7   Ey 0.7    0+            "
                        "96 97De40td  IS=5.6 2;2B-=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 6.7 * 1.0e9 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Zetta years")
    {
      NUBASE::Data data("076 0320   76Ge   -73213.0      1.7                            1.58  Zy 0.17   0+            "
                        "95 01Kl11t   IS=7.61 38;2B-=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 1.58 * 1.0e12 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Yotta years")
    {
      NUBASE::Data data("128 0520   128Te  -88992.1      1.7                            2.2   Yy 0.3    0+            "
                        "01 96Ta04t   IS=31.74 8;2B-=100",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 2.2 * 1.0e15 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Lifetimes that aren't times")
    {
      NUBASE::Data data("003 0030   3Li     28670#    2000#                      RN   p-unst                          "
                        "98           p ?",
                        2003);

      data.setHalfLife();
      auto halflife = Converter::seconds{ 1.0e-24 };
      REQUIRE(data.hl == halflife);
    }
  }

  SECTION("Post 2020")
  {
    SECTION("yoctoseconds")
    {
      NUBASE::Data data("016 0040   16Be    57450        170                                   650     ys 130    0+    "
                        "        15          2012 2n=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::attoseconds{ 650 * 1.0e-6 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("zeptoseconds")
    {
      NUBASE::Data data("028 0090   28F     33400        120                                    46     zs        (4-)  "
                        "        13          2012 n=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::attoseconds{ 46 * 1.0e-3 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("attoseconds")
    {
      NUBASE::Data data("009 0048   9Be  i  25738.8        1.7     14390.3        1.7     RQ     1.25  as 0.10   3/2-  "
                        "  T=3/2 06          1976",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::attoseconds{ 1.25 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("picoseconds")
    {
      NUBASE::Data data("026 0080   26O     34660        160                                     4.2   ps 3.3    0+    "
                        "        16          2012 2n=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::picoseconds{ 4.2 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("nanoseconds")
    {
      NUBASE::Data data("241 0941   241Pum  53116.8        1.1       161.6853     0.0009       880     ns 50     1/2+  "
                        "        15          1975 IT=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::nanoseconds{ 880 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("microseconds")
    {
      NUBASE::Data data("249 1020   249No   81790#       280#                                   57     us 12     5/2+# "
                        "        11          2003 B+ ?;A ?",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::microseconds{ 57 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("milliseconds")
    {
      NUBASE::Data data("106 0510   106Sb  -66473          7                                   600     ms 200    (2+)  "
                        "        08          1981 B+=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::milliseconds{ 600 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("seconds")
    {
      NUBASE::Data data("153 0691   153Tmm -53930         12          43.2        0.2            2.5    s 0.2    "
                        "(1/2+)        20          1988 A=92 3;B+=8 3",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::seconds{ 2.5 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("hours")
    {
      NUBASE::Data data("109 0460   109Pd  -87606.5        1.1                                  13.59   h 0.12   5/2+  "
                        "        16          1937 B-=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::hours{ 13.59 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("years")
    {
      NUBASE::Data data("152 0630   152Eu  -72888.5        1.2                                  13.517  y 0.006  3-*   "
                        "        13          1938 B+=72.08 13;B-=27.92 13",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::years{ 13.517 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("kilo years")
    {
      NUBASE::Data data("246 0960   246Cm   62616.9        1.5                                   4.706 ky 0.040  0+    "
                        "        11          1954 A=99.97385 7;SF=0.02615 7",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::kiloyears{ 4.706 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Mega years")
    {
      NUBASE::Data data("135 0550   135Cs  -87582.0        0.4                                   1.33  My 0.19   7/2+* "
                        "        08          1949 B-=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::millionyears{ 1.33 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Giga years")
    {
      NUBASE::Data data("187 0750   187Re  -41216.5        0.7                                  41.6   Gy 0.02   5/2+* "
                        "        09          1931 IS=62.60 5;B-=100;A=0",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 41.6 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Tera years")
    {
      NUBASE::Data data("152 0640   152Gd  -74707.3        1.0                                 108     Ty 8      0+    "
                        "        13          1938 IS=0.20 3;A=100;2B+ ?",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 108 * 1.0e3 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Peta years")
    {
      NUBASE::Data data("050 0230   50V    -49223.24       0.09                                271     Py 13     6+*   "
                        "        19          1949 IS=0.250 10;B+~100;B- ?",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 271 * 1.0e6 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Exa years")
    {
      NUBASE::Data data("082 0340   82Se   -77593.9        0.5                                  87.6   Ey 1.5    0+    "
                        "        19          1922 IS=8.82 15;2B-=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 87.6 * 1.0e9 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Zetta years")
    {
      NUBASE::Data data("076 0320   76Ge   -73212.898      0.018                                 1.88  Zy 0.08   0+    "
                        "        95          1933 IS=7.75 12;2B-=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 1.88 * 1.0e12 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Yotta years")
    {
      NUBASE::Data data("128 0520   128Te  -88993.8        0.7                                   2.25  Yy 0.09   0+    "
                        "        15          1924 IS=31.74 8;2B-=100",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::billionyears{ 2.25 * 1.0e15 };
      REQUIRE(data.hl == halflife);
    }

    SECTION("Lifetimes that aren't times")
    {
      NUBASE::Data data("005 0040   5Be     37140#      2000#                                  p-unst            1/2+# "
                        "        18               p ?",
                        2020);

      data.setHalfLife();
      auto halflife = Converter::seconds{ 1.0e-24 };
      REQUIRE(data.hl == halflife);
    }
  }
}


TEST_CASE("Read the major decay mode", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    SECTION("Simple decay mode scenarios")
    {
      NUBASE::Data gs03_isotope("229 0880   229Ra   32563       19                              4.0    m 0.2    5/2(+) "
                                "       90           B-=100",
                                2003);

      NUBASE::Data isomer03_isotope("076 0371   76Rbm  -60162.9      1.9     316.93    0.08         3.050 us 0.007  "
                                    "(4+)          95 00Ch07t   IT=100",
                                    2003);


      gs03_isotope.setDecayMode();
      REQUIRE_THAT(gs03_isotope.decay, Catch::Matches("B-"));

      isomer03_isotope.setDecayMode();
      REQUIRE_THAT(isomer03_isotope.decay, Catch::Matches("IT"));
    }

    SECTION("Convert guess into unkown")
    {
      NUBASE::Data data{ "223 0830   223Bi   32140#     400#                             1#     s >300ns 9/2-#         "
                         "11 10Al24i  2009 B- ?;B-n ?",
                         2003 };

      data.setDecayMode();
      REQUIRE_THAT(data.decay, Catch::Matches("unknown"));
    }

    SECTION("Convert e+ -> B+")
    {
      NUBASE::Data data{ "119 0540   119Xe  -78794       10                              5.8    m 0.3    5/2(+)        "
                         "00 90Ne.Aj   e+=79 5;EC=21 5",
                         2003 };

      data.setDecayMode();
      REQUIRE_FALSE(data.decay.compare("B+"));
    }

    SECTION("Remove unwanted characters")
    {
      NUBASE::Data data{ "168 0711   168Lum -56880      100       180     110     BD*    6.7    m 0.4    3+            "
                         "94           B+>95;IT<5",
                         2003 };

      data.setDecayMode();
      REQUIRE_FALSE(data.decay.compare("B+"));
    }
  }

  SECTION("Post 2020")
  {
    SECTION("Simple decay mode scenarios")
    {
      NUBASE::Data gs20_isotope("081 0360   81Kr   -77696.2        1.1                                 229     ky 11   "
                                "  7/2+*         08          1950 EC=100",
                                2020);

      NUBASE::Data isomer20_isotope("034 0141   34Si m -15735.6        0.9      4256.1        0.4         <210     ns  "
                                    "      (3-)          12          1989 IT=100",
                                    2020);


      gs20_isotope.setDecayMode();
      REQUIRE_THAT(gs20_isotope.decay, Catch::Matches("EC"));

      isomer20_isotope.setDecayMode();
      REQUIRE_THAT(isomer20_isotope.decay, Catch::Matches("IT"));
    }

    SECTION("Convert guess into unkown")
    {
      NUBASE::Data data{ "183 0700   183Yb  -35000#       400#                                   30#     s >300ns "
                         "3/2-#         16          2012 B- ?",
                         2020 };

      data.setDecayMode();
      REQUIRE_THAT(data.decay, Catch::Matches("unknown"));
    }

    // Scenario doesn't exist for the major decay mode in 2020 data
    // SECTION("Convert e+ -> B+")
    //{
    //  NUBASE::Data data{ "", 2020 };
    //
    //  data.setDecayMode();
    //  REQUIRE_FALSE(data.decay.compare("B+"));
    //}

    SECTION("Remove unwanted characters")
    {
      NUBASE::Data data{ "244 1000   244Fm   68960#       200#                                    3.12  ms 0.08   0+   "
                         "         17          1967 SF>97;B+<2;A<1",
                         2020 };

      data.setDecayMode();
      REQUIRE_FALSE(data.decay.compare("SF"));
    }
  }
}


TEST_CASE("Read the year", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs12_isotope("108 0460   108Pd  -89524.4      1.1                          stbl              0+       "
                              "     08          1935 IS=26.46 9",
                              2012);
    gs12_isotope.setYear();
    REQUIRE(gs12_isotope.year == 1935);

    NUBASE::Data isomer12_isotope("075 0322W  75Gen  -71664.78     0.09    192.18    0.07       216     ns 5      5/2+ "
                                  "         99          1982 IT=100",
                                  2012);
    isomer12_isotope.setYear();
    REQUIRE(isomer12_isotope.year == 1982);

    NUBASE::Data missing_year("087 0431W  87Tcm  -57683        4         7       1       *    2#     s        1/2-#    "
                              "        09Ga40e       B+ ?;IT ?",
                              2012);
    missing_year.setYear();
    REQUIRE(missing_year.year == 1900);
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("053 0200   53Ca   -29390         40                                   461     ms 90     "
                              "1/2-#         14          1983 B-=100;B-n=40 10",
                              2020);
    gs20_isotope.setYear();
    REQUIRE(gs20_isotope.year == 1983);

    NUBASE::Data isomer20_isotope("101 0471   101Agm -81060          5         274.1        0.3            3.10   s "
                                  "0.10   (1/2)-*       06          1975 IT=100",
                                  2020);
    isomer20_isotope.setYear();
    REQUIRE(isomer20_isotope.year == 1975);

    NUBASE::Data missing_year("046 0248   46Cr i -20328         13        9143         17       RQ                     "
                              "(4+)    T=2   10               p=?",
                              2020);
    missing_year.setYear();
    REQUIRE(missing_year.year == 1900);
  }
}


TEST_CASE("Read mass number", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope("103 0490   103In  -74630        9                             60      s 1      9/2+#    "
                              "     09 97Sz04t  1978 B+=100, 2003",
                              2003);
    gs03_isotope.setA();
    REQUIRE(gs03_isotope.A == 103);
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("055 0230   55V    -49125         27                                     6.54   s 0.15   "
                              "7/2-#         08          1977 B-=100",
                              2020);
    gs20_isotope.setA();
    REQUIRE(gs20_isotope.A == 55);
  }
}


TEST_CASE("Read proton number", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope("120 0480   120Cd  -83957        4                             50.80   s 0.21   0+       "
                              "     02          1973 B-=100",
                              2003);
    gs03_isotope.setZ();
    REQUIRE(gs03_isotope.Z == 48);
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("137 0490   137In  -35830#       400#                                   70     ms 40     "
                              "9/2+#         15          2015 B-=100;B-n ?;B-2n ?",
                              2020);
    gs20_isotope.setZ();
    REQUIRE(gs20_isotope.Z == 49);
  }
}


// This is not reliant on reading a value from the line, it is just a subtraction of 2 numbers.
// No need to do multiple tests
TEST_CASE("Calculate neutron number", "[NUBASEData]")
{
  NUBASE::Data gs03_isotope("012 0080   12O     31915       24                             >6.3   zs        0+         "
                            "   00 12Ja11t  1978 2p=60 30",
                            2003);

  gs03_isotope.setA();
  gs03_isotope.setZ();

  gs03_isotope.setN();

  REQUIRE(gs03_isotope.N == 4);
}


TEST_CASE("Read mass excess", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope("164 0670   164Ho  -64980.8      2.3                           29      m 1      1+       "
                              "     01          1938 EC=60 5;B-=40 5",
                              2003);

    gs03_isotope.setMassExcess();

    REQUIRE(gs03_isotope.mass_excess == Approx(-64980.8));
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("014 0060   14C      3019.893      0.004                                 5.70  ky 0.03   "
                              "0+            01          1936 B-=100",
                              2020);

    gs20_isotope.setMassExcess();

    REQUIRE(gs20_isotope.mass_excess == Approx(3019.893));
  }
}


TEST_CASE("Read mass excess error", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    NUBASE::Data gs03_isotope("141 0620   141Sm  -75934        9                             10.2    m 0.2    1/2+     "
                              "     01          1967 B+=100",
                              2003);
    gs03_isotope.setMassExcessError();
    REQUIRE(gs03_isotope.dmass_excess == Approx(9));
  }

  SECTION("Post 2020")
  {
    NUBASE::Data gs20_isotope("057 0250   57Mn   -57486.3        1.5                                  85.4    s 1.8    "
                              "5/2-*         98          1954 B-=100",
                              2020);
    gs20_isotope.setMassExcessError();
    REQUIRE(gs20_isotope.dmass_excess == Approx(1.5));
  }
}


TEST_CASE("Calculate relative error on mass excess", "[NUBASEData]")
{
  SECTION("Pre 2020")
  {
    SECTION("Random isotope is processed correctly")
    {
      NUBASE::Data gs03_isotope("119 0530   119I   -83766       28                             19.1    m 0.4    5/2+   "
                                "       09          1954 e+=51 4;EC=49 4",
                                2003);

      gs03_isotope.setMassExcess();
      gs03_isotope.setMassExcessError();

      REQUIRE(gs03_isotope.getRelativeMassExcessError(1.0e-7) == Approx(std::fabs(28.0 / -83766.0)));
      REQUIRE(gs03_isotope.getRelativeMassExcessError(0.1) == Approx(0.1));
    }

    SECTION("12C and it's annoying 0.0 ME is handled correctly")
    {
      NUBASE::Data nubase03_12C_isotope("012 0060   12C         0.0      0.0                          stbl             "
                                        " 0+            00           IS=98.938 ",
                                        2012);

      nubase03_12C_isotope.setA();
      nubase03_12C_isotope.setZ();
      nubase03_12C_isotope.setMassExcess();
      nubase03_12C_isotope.setMassExcessError();

      REQUIRE(nubase03_12C_isotope.getRelativeMassExcessError(1.0e-5) == Approx(1.0e-5));
    }
  }

  SECTION("Post 2020")
  {
    SECTION("Random isotope is processed correctly")
    {
      NUBASE::Data gs20_isotope("169 0770   169Ir  -22093         23                                   353     ms 4    "
                                "  (1/2+)        08          1978 A=53 7;B+ ?",
                                2020);

      gs20_isotope.setMassExcess();
      gs20_isotope.setMassExcessError();

      REQUIRE(gs20_isotope.getRelativeMassExcessError(1.0e-7) == Approx(std::fabs(23.0 / -22093.0)));
      REQUIRE(gs20_isotope.getRelativeMassExcessError(0.1) == Approx(0.1));
    }

    SECTION("12C and it's annoying 0.0 ME is handled correctly")
    {
      NUBASE::Data nubase20_12C_isotope("012 0060   12C         0.0        0.0                                 stbl    "
                                        "          0+            17          1919 IS=98.94 6",
                                        2020);

      nubase20_12C_isotope.setA();
      nubase20_12C_isotope.setZ();
      nubase20_12C_isotope.setMassExcess();
      nubase20_12C_isotope.setMassExcessError();

      REQUIRE(nubase20_12C_isotope.getRelativeMassExcessError(1.0e-5) == Approx(1.0e-5));
    }
  }
}

// Nothing related to reading a line from the file so not need to test pre and post 2020
TEST_CASE("Set the neutron/proton rich value", "[NUBASEData]")
{
  NUBASE::Data data = NUBASE::Data("", 2003);

  SECTION("Proton rich")
  {
    data.setNeutronOrProtonRich(false);
    REQUIRE(data.rich == NUBASE::Richness::PROTON);
  }

  SECTION("Neutron rich")
  {
    data.setNeutronOrProtonRich(true);
    REQUIRE(data.rich == NUBASE::Richness::NEUTRON);
  }

  SECTION("Stable")
  {
    data.decay = "stable";
    data.setNeutronOrProtonRich(true);
    REQUIRE(data.rich == NUBASE::Richness::STABLE);
  }

  SECTION("Special cases of 96Tc and 144Pm that have no stable isotope")
  {
    SECTION("96Tc")
    {
      data.Z = 43;

      data.A = 90;
      data.setNeutronOrProtonRich(true);
      REQUIRE(data.rich == NUBASE::Richness::PROTON);

      data.A = 97;
      data.setNeutronOrProtonRich(true);
      REQUIRE(data.rich == NUBASE::Richness::NEUTRON);
    }

    SECTION("144Pm")
    {
      data.Z = 61;

      data.A = 140;
      data.setNeutronOrProtonRich(true);
      REQUIRE(data.rich == NUBASE::Richness::PROTON);

      data.A = 145;
      data.setNeutronOrProtonRich(true);
      REQUIRE(data.rich == NUBASE::Richness::NEUTRON);
    }
  }
}

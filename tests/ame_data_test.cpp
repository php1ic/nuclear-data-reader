#include "nuclear-data-reader/ame_data.hpp"

#include <catch2/catch.hpp>

// Lines from the files were picked at random with the `shuf` command
// $ shuf -n1 FILE

TEST_CASE("A", "[AMEData]")
{
  SECTION("From mass table")
  {
    const std::string line{ "  34   85   51  136 Sb    x  -64879#       298#        8255#       2#    B-   9547#     "
                            "301#    135 930350#       320#" };
    AME::Data data(line);
    data.setA();
    REQUIRE(data.A == 136);
  }

  SECTION("From reaction file number 1")
  {
    const std::string line{ " 240 Am  95   13053.94   52.53  10522.43   13.79   5707.57   52.27  -4154#    151#    "
                            "-5089.57   13.83  -7752#    101#" };
    REQUIRE(AME::Data::getReaction_1_A(line) == 240);
  }

  SECTION("From reaction file number 2")
  {
    const std::string line{ " 285 Ee 114       *              2056#   1304#        *             17461#   1261#        "
                            "*             18625#   1249#" };
    REQUIRE(AME::Data::getReaction_2_A(line) == 285);
  }
}


TEST_CASE("Z", "[AMEData]")
{
  SECTION("From mass table")
  {
    const std::string line{ "  29  105   76  181 Os    x  -43552.934     31.671     7983.442    0.175 B-  -4081.152   "
                            "40.741 180 953244.000     34.000" };
    AME::Data data(line);
    data.setZ();
    REQUIRE(data.Z == 76);
  }

  SECTION("From reaction file number 1")
  {
    const std::string line{ "  62 Cr  24   10053.31  398.88  33345#    869#   -12072#    776#    18486.20  337.30 "
                            "-32054#    964#     3069.87  406.83" };
    REQUIRE(AME::Data::getReaction_1_Z(line) == 24);
  }

  SECTION("From reaction file number 2")
  {
    const std::string line{ " 105 Pd  46    7094.10    0.70   8751.97    4.62 -15150.30   80.69  10320.16    4.62   "
                            "3226.23    6.14   6331.62    4.15" };
    REQUIRE(AME::Data::getReaction_2_Z(line) == 46);
  }
}


TEST_CASE("Calculate N", "[AMEData]")
{
  AME::Data data("");
  data.A = 56;
  data.Z = 23;
  data.setN();

  REQUIRE(data.N == 33);
}


TEST_CASE("Mass Excess", "[AMEData]")
{
  const std::string line{ "  37  124   87  211 Fr        -4157.682     21.101     7768.443    0.100 B-  -4994.152   "
                          "33.674 210 995536.544     22.652" };
  AME::Data data(line);
  data.setMassExcess();
  REQUIRE(data.mass_excess == Approx(-4157.682));
}


TEST_CASE("Error on mass excess", "[AMEData]")
{
  // const std::string line{ "  54  166  112  278 Ec   -a  153056#       531#        7206#       2#    B-      * "
  //                        "    278 164312#       570#" };
  // AME::Data data(line);
  // data.setMassExcessError();
  // REQUIRE(data.dmass_excess == Approx(531));

  const std::string line{ "0  1    1    0    1  n         8071.31710    0.00053      0.0      0.0   B-    782.347    "
                          "0.001   1 008664.91574    0.00056" };
  AME::Data data(line);
  data.setMassExcessError();
  REQUIRE(data.dmass_excess == Approx(0.00053));
}


TEST_CASE("Relative mass excess error", "[AMEData]")
{
  SECTION("Random isotope line")
  {
    const std::string line{
      "  -6   13   19   32 K     x   20418#       503#        6969#      16#    B-      *          "
      "     32 021920#       540#"
    };
    AME::Data data(line);
    constexpr double min{ 1.0e-7 };
    data.setMassExcess();
    data.setMassExcessError();
    REQUIRE(data.getRelativeMassExcessError(min) == Approx(0.024635));
  }

  SECTION("12C with it's definition mass and thus zero error")
  {
    const std::string line{ "   0    6    6   12 C             0.0        0.0       7680.144    0.000 B- -17338.082    "
                            "1.000  12 000000.0        0.0" };
    AME::Data data(line);
    constexpr double min{ 1.0e-7 };
    data.setA();
    data.setZ();
    data.setMassExcess();
    data.setMassExcessError();
    REQUIRE(data.getRelativeMassExcessError(min) == Approx(min));
  }
}


TEST_CASE("Experimental assignment", "[AMEData]")
{
  SECTION("Automatically set by reading the line from the file")
  {
    const std::string line{ "0 52  136   84  220 Po   +a   15465#       359#        7702#       2#    B-   1113#     "
                            "363#    220 016602#       385#" };
    AME::Data data(line);
    data.setExperimental();
    REQUIRE(data.exp == 1);
  }

  SECTION("Manually set")
  {
    AME::Data data("");
    data.setExperimental(0);
    REQUIRE(data.exp == 0);
    data.setExperimental(1);
    REQUIRE(data.exp == 1);
  }
}


TEST_CASE("Binding energy per A", "[AMEData]")
{
  const std::string line{ "  21   60   39   99 Y        -70200.924     24.391     8472.220    0.246 B-   7567.548   "
                          "13.983  98 924636.204     26.185" };
  AME::Data data(line);
  data.setBindingEnergyPerA();
  REQUIRE(data.binding_energy_per_A == Approx(8472.220));
}


TEST_CASE("Error on binding energy per A", "[AMEData]")
{
  const std::string line{ "  11   48   37   85 Rb       -82167.331      0.011     8697.441    0.000 B-  -1064.758    "
                          "2.837  84 911789.737      0.012" };
  AME::Data data(line);
  data.setBindingEnergyPerAError();
  REQUIRE(data.dbinding_energy_per_A == Approx(0.0));
}


TEST_CASE("Beta decay energy", "[AMEData]")
{
  const std::string line{ "  26   83   57  140 La       -84321.031      2.415     8355.083    0.017 B-   3762.248    "
                          "1.829 139 909477.645      2.592" };
  AME::Data data(line);
  data.setBetaDecayEnergy();
  REQUIRE(data.beta_decay_energy == Approx(3762.248));
}


TEST_CASE("Error on beta decay energy", "[AMEData]")
{
  const std::string line{ "  12   74   62  136 Sm    x  -66810.917     12.497     8205.916    0.092 B- -10549#     "
                          "196#    135 928275.527     13.416" };
  AME::Data data(line);
  data.setBetaDecayEnergyError();
  REQUIRE(data.dbeta_decay_energy == Approx(196));
}


TEST_CASE("Atomic mass", "[AMEData]")
{
  const std::string line{ "  48  150  102  252 No   -a   82881.097     12.993     7425.760    0.052 B-  -5956#     "
                          "249#    252 088976.521     13.948" };
  AME::Data data(line);
  data.setAtomicMass();
  REQUIRE(data.atomic_mass == Approx(88976.521));
}


TEST_CASE("Error on atomic mass", "[AMEData]")
{
  const std::string line{ "   7   17   10   27 Ne    x    7069.949    110.384     7519.709    4.088 B-  12587.386  "
                          "110.439  27 007589.903    118.501" };
  AME::Data data(line);
  data.setAtomicMassError();
  REQUIRE(data.datomic_mass == Approx(118.501));
}


TEST_CASE("Two neutron separation energy", "[AMEData]")
{
  const std::string line{ " 107 Ag  47   17476.39   10.56  15134.04    3.56  -2804.47    4.85  -4842.17   10.81  "
                          "-9329.24    6.89  -9340.56    7.20" };
  AME::Data data(line);
  data.setTwoNeutronSeparationEnergy();
  REQUIRE(data.s_2n == Approx(17476.39));
}


TEST_CASE("Error on two neutron separation energy", "[AMEData]")
{
  const std::string line{ "0216 Bi  83    9068.49   15.84      *              5100#    300#     3616.70   11.76      * "
                          "            -1657.09   11.46" };
  AME::Data data(line);
  data.setTwoNeutronSeparationEnergyError();
  REQUIRE(data.ds_2n == Approx(15.84));
}


TEST_CASE("Two proton separation energy", "[AMEData]")
{
  const std::string line{
    "  93 Ag  47       *              2254#    718#    -1546#    746#        *              9292#    718#        *"
  };
  AME::Data data(line);
  data.setTwoProtonSeparationEnergy();
  REQUIRE(data.s_2p == Approx(2254));
}


TEST_CASE("Error on two proton separation energy", "[AMEData]")
{
  const std::string line{ " 137 Sm  62   21310.80  160.33   6389.56   46.58   1882.08   62.98 -16812#    403#     "
                          "3884.96   44.01 -19834#    200#" };
  AME::Data data(line);
  data.setTwoProtonSeparationEnergyError();
  REQUIRE(data.ds_2p == Approx(46.58));
}


TEST_CASE("Q alpha", "[AMEData]")
{
  const std::string line{ " 199 Pt  78   13112.57    3.04      *              -127.45  500.01   2154.70    3.15      * "
                          "            -5881.57    3.10" };
  AME::Data data(line);
  data.setQAlphaEnergy();
  REQUIRE(data.q_a == Approx(-127.45));
}


TEST_CASE("Error on Q alpha", "[AMEData]")
{
  const std::string line{ "  24 Na  11   19378.31    0.42  25789.43   12.40 -10825.63    0.11  -8361.17    2.78 "
                          "-19036.83   79.54 -11015.66    1.29" };
  AME::Data data(line);
  data.setQAlphaEnergyError();
  REQUIRE(data.dq_a == Approx(0.11));
}


TEST_CASE("Q double beta", "[AMEData]")
{
  const std::string line{ " 236 U   92   11842.93    0.33  12745.90    3.17   4573.10    0.90   -456.39    1.66  "
                          "-9097.99   50.01  -6669.66    0.89" };
  AME::Data data(line);
  data.setQDoubleBetaMinusEnergy();
  REQUIRE(data.q_2bm == Approx(-456.39));
}


TEST_CASE("Error on Q double beta", "[AMEData]")
{
  const std::string line{ "  46 S   16    6328#    802#    46724#   1063#   -20161#    861#    30419#    700#        * "
                          "            10990#    709#" };
  AME::Data data(line);
  data.setQDoubleBetaMinusEnergyError();
  REQUIRE(data.dq_2bm == Approx(700));
}


TEST_CASE("Q ep", "[AMEData]")
{
  const std::string line{
    "  10 N    7       *             -1300.72  400.00  -7228#    805#        *             19095.50  400.00      *"
  };
  AME::Data data(line);
  data.setQEpsilonPEnergy();
  REQUIRE(data.q_ep == Approx(19095.50));
}


TEST_CASE("Error on Q ep", "[AMEData]")
{
  const std::string line{
    "   1 H    1       *                 *                 *                 *                 *                 *"
  };
  AME::Data data(line);
  data.setQEpsilonPEnergyError();
  REQUIRE(data.dq_ep == Approx(std::numeric_limits<double>::max()));
}


TEST_CASE("Q B- n", "[AMEData]")
{
  const std::string line{ " 125 Xe  54   18086.15    9.70  12598.11    2.20  -1065.83   25.99  -7524.09   11.27  "
                          "-3956.49    2.19 -13532.05    8.51" };
  AME::Data data(line);
  data.setQBetaMinusNEnergy();
  REQUIRE(data.q_bm_n == Approx(-13532.05));
}


TEST_CASE("Error on Q B- n", "[AMEData]")
{
  const std::string line{ " 115 Te  52   19858.36   39.52   8307.66   28.23   1457.12   28.77 -13405.99   30.46   "
                          "1209.17   28.12 -17338#    299#" };
  AME::Data data(line);
  data.setQBetaMinusNEnergyError();
  REQUIRE(data.dq_bm_n == Approx(299));
}


TEST_CASE("One neutron separation energy", "[AMEData]")
{
  const std::string line{ " 151 Ho  67    9755.50   18.50   1604.10   12.43 -33431#    401#    14793.87   14.60   "
                          "9091.46   16.01  12554.77   18.35" };
  AME::Data data(line);
  data.setOneNeutronSeparationEnergy();
  REQUIRE(data.s_n == Approx(9755.50));
}


TEST_CASE("Error on one neutron separation energy", "[AMEData]")
{
  const std::string line{ " 187 Tl  81   10324.70  184.61   1193.17   13.86      *             14443.13   17.52   "
                          "8769.67   12.92  13521.60   23.70" };
  AME::Data data(line);
  data.setOneNeutronSeparationEnergyError();
  REQUIRE(data.ds_n == Approx(184.61));
}


TEST_CASE("One proton separation energy", "[AMEData]")
{
  const std::string line{ "  18 Na  11       *              -440.10   57.07      *             10904.31   54.31      * "
                          "            13061.00  142.94" };
  AME::Data data(line);
  data.setOneProtonSeparationEnergy();
  REQUIRE(data.s_p == Approx(-440.10));
}


TEST_CASE("Error on one proton separation energy", "[AMEData]")
{
  const std::string line{ "  42 Ca  20   11480.63    0.06  10276.97    0.19 -44537#    298#     5698.94    0.18    "
                          "123.99    0.17    341.34    5.00" };
  AME::Data data(line);
  data.setOneProtonSeparationEnergyError();
  REQUIRE(data.ds_p == Approx(0.19));
}


TEST_CASE("Q value on quadruple Beta minus", "[AMEData]")
{
  const std::string line{ " 213 Rn  86    5109.97    6.27   4366.04    9.01 -17817.13   71.27  16659.70    6.07  "
                          "11137.63    9.52  15901.21    5.54" };
  AME::Data data(line);
  data.setQQuadrupleBetaMinusEnergy();
  REQUIRE(data.q_4bm == Approx(-17817.13));
}


TEST_CASE("Error on Q value of quadruple Beta minus", "[AMEData]")
{
  const std::string line{ " 206 Ra  88   10345.37   88.32   2414.17   19.65      *             13667.43   30.49   "
                          "7567.83   24.00  15371.74   29.67" };
  AME::Data data(line);
  data.setQQuadrupleBetaMinusEnergyError();
  REQUIRE(data.dq_4bm == Approx(std::numeric_limits<double>::max()));
}


TEST_CASE("Q value of d alpha", "[AMEData]")
{
  const std::string line{ "  45 Mn  25   19585#    585#    -1058#    302#        *              7730#    370#    "
                          "-6239#    422#     8702#    357#" };
  AME::Data data(line);
  data.setQDAlphaEnergy();
  REQUIRE(data.q_da == Approx(7730));
}


TEST_CASE("Error on Q value of d alpha", "[AMEData]")
{
  const std::string line{ "  72 Se  34   12849.39   33.80   7289.04   12.76      *              7159.51   51.44     "
                          "56.31   33.46   4852.60   12.13" };
  AME::Data data(line);
  data.setQDAlphaEnergyError();
  REQUIRE(data.dq_da == Approx(51.44));
}


TEST_CASE("Q value of p alpha", "[AMEData]")
{
  const std::string line{ "  87 Mo  42   11209.89  491.98   5157.55  239.06      *             10165.00  316.25   "
                          "-952#    372#     9444#    297#" };
  AME::Data data(line);
  data.setQPAlphaEnergy();
  REQUIRE(data.q_pa == Approx(-952));
}


TEST_CASE("Error on Q value of p alpha", "[AMEData]")
{
  const std::string line{
    "0 67 Cr  24    2324#    918#        *             44694#    699#     2914#   1063#     1213#    988#        *"
  };
  AME::Data data(line);
  data.setQPAlphaEnergyError();
  REQUIRE(data.dq_pa == Approx(988));
}


TEST_CASE("Q value on n alpha", "[AMEData]")
{
  const std::string line{ " 157 Sm  62    5434.42   51.07   9802.16   60.82   2694.46   50.62  10950.62   58.51   "
                          "6629.02   67.26   4604.44  124.56" };
  AME::Data data(line);
  data.setQNAlphaEnergy();
  REQUIRE(data.q_na == Approx(4604.44));
}


TEST_CASE("Error on Q value of n alpha", "[AMEData]")
{
  const std::string line{ " 137 Sb  51    3451#    499#    11043#    643#    26287#    401#    11251#    566#    "
                          "11401#    413#    -2588#    566#" };
  AME::Data data(line);
  data.setQNAlphaEnergyError();
  REQUIRE(data.dq_na == Approx(566));
}

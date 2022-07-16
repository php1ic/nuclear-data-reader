#include "nuclear-data-reader/ame_data.hpp"

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

// Lines from the files were picked at random with the `shuf` command
// $ shuf -n1 FILE

TEST_CASE("A", "[AMEData]")
{
  SECTION("From mass table")
  {
    SECTION("Pre 2020")
    {
      const std::string line{ "  34   85   51  136 Sb    x  -64879#       298#        8255#       2#    B-   9547#     "
                              "301#    135 930350#       320#" };
      const AME::Data data(line, 2003);
      data.setA();
      REQUIRE(data.A == 136);
    }

    SECTION("Post 2020")
    {
      const std::string line{ "  39  129   90  219 Th   -a   14462.780      56.460      7683.7655     0.2578  B-  "
                              "-4120.4434    89.7010  219 015526.432      60.611" };
      const AME::Data data(line, 2020);
      data.setA();
      REQUIRE(data.A == 219);
    }
  }

  SECTION("From reaction file number 1")
  {
    SECTION("Pre 2020")
    {
      const std::string line{ " 240 Am  95   13053.94   52.53  10522.43   13.79   5707.57   52.27  -4154#    151#    "
                              "-5089.57   13.83  -7752#    101#" };
      const AME::Data data(line, 2003);
      REQUIRE(data.getReaction_1_A(line) == 240);
    }

    SECTION("Post 2020")
    {
      const std::string line{ " 202 Ra  88       *                  1502.5879   16.0920   7880.3036    6.7407      *   "
                              "               5893.3415   18.1099      *" };
      const AME::Data data(line, 2020);
      REQUIRE(data.getReaction_1_A(line) == 202);
    }
  }

  SECTION("From reaction file number 2")
  {
    SECTION("Pre 2020")
    {
      const std::string line{ " 285 Ee 114       *              2056#   1304#        *             17461#   1261#      "
                              "  *             18625#   1249#" };
      const AME::Data data(line, 2003);
      REQUIRE(data.getReaction_2_A(line) == 285);
    }

    SECTION("Post 2020")
    {
      const std::string line{ " 151 Eu  63    7932.0467    6.1782   4890.7336    0.5472 -11030.3521    8.3288  "
                              "13193.6069    0.6611   9547.0236    1.0137   7859.1808    5.6099" };
      const AME::Data data(line, 2020);
      REQUIRE(data.getReaction_2_A(line) == 151);
    }
  }
}


TEST_CASE("Z", "[AMEData]")
{
  SECTION("From mass table")
  {
    SECTION("Pre 2020")
    {
      const std::string line{
        "  29  105   76  181 Os    x  -43552.934     31.671     7983.442    0.175 B-  -4081.152   "
        "40.741 180 953244.000     34.000"
      };
      const AME::Data data(line, 2003);
      data.setZ();
      REQUIRE(data.Z == 76);
    }

    SECTION("Post 2020")
    {
      const std::string line{ "   5   53   48  101 Cd    x  -75836.466       1.490      8450.3657     0.0148  B-  "
                              "-7291.5642    11.7569  100 918586.209       1.600" };
      const AME::Data data(line, 2020);
      data.setZ();
      REQUIRE(data.Z == 48);
    }
  }

  SECTION("From reaction file number 1")
  {
    SECTION("Pre 2020")
    {
      const std::string line{ "  62 Cr  24   10053.31  398.88  33345#    869#   -12072#    776#    18486.20  337.30 "
                              "-32054#    964#     3069.87  406.83" };
      const AME::Data data(line, 2003);
      REQUIRE(data.getReaction_1_Z(line) == 24);
    }
    SECTION("Post 2020")
    {
      const std::string line{ " 101 Rb  37    7588.9036   20.8855      *                -10992#      400#      "
                              "22487.3691   21.6816      *                  9178.7690   21.6290" };
      const AME::Data data(line, 2020);
      REQUIRE(data.getReaction_1_Z(line) == 37);
    }
  }

  SECTION("From reaction file number 2")
  {
    SECTION("Pre 2020")
    {
      const std::string line{ " 105 Pd  46    7094.10    0.70   8751.97    4.62 -15150.30   80.69  10320.16    4.62   "
                              "3226.23    6.14   6331.62    4.15" };
      const AME::Data data(line, 2003);
      REQUIRE(data.getReaction_2_Z(line) == 46);
    }
    SECTION("Post 2020")
    {
      const std::string line{ "  65 As  33   15476#      220#        -89.4755   84.8478      *                 "
                              "10694.9716   92.5935     67#      163#      10696.3676   84.7684" };
      const AME::Data data(line, 2020);
      REQUIRE(data.getReaction_2_Z(line) == 33);
    }
  }
}


TEST_CASE("Calculate N", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const AME::Data data("", 2003);
    data.A = 56;
    data.Z = 23;
    data.setN();

    REQUIRE(data.N == 33);
  }

  SECTION("Post 2020")
  {
    const AME::Data data("", 2020);
    data.A = 187;
    data.Z = 80;
    data.setN();

    REQUIRE(data.N == 107);
  }
}


TEST_CASE("Mass Excess", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  37  124   87  211 Fr        -4157.682     21.101     7768.443    0.100 B-  -4994.152   "
                            "33.674 210 995536.544     22.652" };
    const AME::Data data(line, 2003);
    data.setMassExcess();
    REQUIRE(data.mass_excess == Catch::Approx(-4157.682));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  25  100   75  175 Re    x  -45288.312      27.945      7994.8168     0.1597  B-  "
                            "-5182.9513    30.3243  174 951381.000      30.000" };
    const AME::Data data(line, 2020);
    data.setMassExcess();
    REQUIRE(data.mass_excess == Catch::Approx(-45288.213));
  }
}


TEST_CASE("Error on mass excess", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "0  1    1    0    1  n         8071.31710    0.00053      0.0      0.0   B-    782.347    "
                            "0.001   1 008664.91574    0.00056" };
    const AME::Data data(line, 2003);
    data.setMassExcessError();
    REQUIRE(data.dmass_excess == Catch::Approx(0.00053));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "0 25   56   31   87 Ga    x  -28870#        500#         8124#         6#      B-  14720# "
                            "      583#       86 969007#        537#" };
    const AME::Data data(line, 2020);
    data.setMassExcessError();
    REQUIRE(data.dmass_excess == Catch::Approx(500.0));
  }
}


TEST_CASE("Relative mass excess error", "[AMEData]")
{
  SECTION("Random isotope line")
  {
    SECTION("Pre 2020")
    {
      const std::string line{
        "  -6   13   19   32 K     x   20418#       503#        6969#      16#    B-      *          "
        "     32 021920#       540#"
      };
      const AME::Data data(line, 2003);
      constexpr double min{ 1.0e-7 };
      data.setMassExcess();
      data.setMassExcessError();
      REQUIRE(data.getRelativeMassExcessError(min) == Catch::Approx(0.024635));
    }

    SECTION("Post 2020")
    {
      const std::string line{ "  25   83   58  141 Ce       -85431.058       1.315      8355.3956     0.0093  B-    "
                              "583.4758     1.1784  140 908285.991       1.411" };
      const AME::Data data(line, 2020);
      constexpr double min{ 1.0e-7 };
      data.setMassExcess();
      data.setMassExcessError();
      REQUIRE(data.getRelativeMassExcessError(min) == Catch::Approx(0.0000153925));
    }
  }

  SECTION("12C with it's definition mass and thus zero error")
  {
    SECTION("Pre 2020")
    {
      const std::string line{
        "   0    6    6   12 C             0.0        0.0       7680.144    0.000 B- -17338.082    "
        "1.000  12 000000.0        0.0"
      };
      const AME::Data data(line, 2003);
      constexpr double min{ 1.0e-7 };
      data.setA();
      data.setZ();
      data.setMassExcess();
      data.setMassExcessError();
      REQUIRE(data.getRelativeMassExcessError(min) == Catch::Approx(min));
    }

    SECTION("Post 2020")
    {
      const std::string line{ "   0    6    6   12 C             0.0         0.0        7680.1446     0.0002  B- "
                              "-17338.0681     0.9999   12 000000.0         0.0" };
      const AME::Data data(line, 2020);
      constexpr double min{ 1.0e-7 };
      data.setA();
      data.setZ();
      data.setMassExcess();
      data.setMassExcessError();
      REQUIRE(data.getRelativeMassExcessError(min) == Catch::Approx(min));
    }
  }
}


TEST_CASE("Experimental assignment", "[AMEData]")
{
  SECTION("Automatically set by reading the line from the file")
  {
    SECTION("Pre 2020")
    {
      const std::string line{ "0 52  136   84  220 Po   +a   15465#       359#        7702#       2#    B-   1113#     "
                              "363#    220 016602#       385#" };
      const AME::Data data(line, 2003);
      data.setExperimental();
      REQUIRE(data.exp == AME::Measured::THEORETICAL);
    }

    SECTION("Post 2020")
    {
      const std::string line{ "  48  130   82  212 Pb        -7548.929       1.840      7804.3203     0.0087  B-    "
                              "569.0133     1.8246  211 991895.891       1.975" };
      const AME::Data data(line, 2020);
      data.setExperimental();
      REQUIRE(data.exp == AME::Measured::EXPERIMENTAL);
    }
  }

  SECTION("Manually set")
  {
    SECTION("Pre 2020")
    {
      const AME::Data data("", 2003);
      data.setExperimental(AME::Measured::EXPERIMENTAL);
      REQUIRE(data.exp == AME::Measured::EXPERIMENTAL);
      data.setExperimental(AME::Measured::THEORETICAL);
      REQUIRE(data.exp == AME::Measured::THEORETICAL);
    }

    SECTION("Post 2020")
    {
      const AME::Data data("", 2020);
      data.setExperimental(AME::Measured::EXPERIMENTAL);
      REQUIRE(data.exp == AME::Measured::EXPERIMENTAL);
      data.setExperimental(AME::Measured::THEORETICAL);
      REQUIRE(data.exp == AME::Measured::THEORETICAL);
    }
  }
}


TEST_CASE("Binding energy per A", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  21   60   39   99 Y        -70200.924     24.391     8472.220    0.246 B-   7567.548   "
                            "13.983  98 924636.204     26.185" };
    const AME::Data data(line, 2003);
    data.setBindingEnergyPerA();
    REQUIRE(data.binding_energy_per_A == Catch::Approx(8472.220));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "0 59  165  106  271 Sg   -a  124617#        591#         7305#         2#      B-  -1242# "
                            "      705#      271 133782#        634#" };
    const AME::Data data(line, 2020);
    data.setBindingEnergyPerA();
    REQUIRE(data.binding_energy_per_A == Catch::Approx(7305));
  }
}


TEST_CASE("Error on binding energy per A", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  11   48   37   85 Rb       -82167.331      0.011     8697.441    0.000 B-  -1064.758    "
                            "2.837  84 911789.737      0.012" };
    const AME::Data data(line, 2003);
    data.setBindingEnergyPerAError();
    REQUIRE(data.dbinding_energy_per_A == Catch::Approx(0.0));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  28   94   66  160 Dy       -69672.425       0.700      8184.0526     0.0044  B-  "
                            "-3290.0000    15.0000  159 925203.578       0.751" };
    const AME::Data data(line, 2020);
    data.setBindingEnergyPerAError();
    REQUIRE(data.dbinding_energy_per_A == Catch::Approx(0.0044));
  }
}


TEST_CASE("Beta decay energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  26   83   57  140 La       -84321.031      2.415     8355.083    0.017 B-   3762.248    "
                            "1.829 139 909477.645      2.592" };
    const AME::Data data(line, 2003);
    data.setBetaDecayEnergy();
    REQUIRE(data.beta_decay_energy == Catch::Approx(3762.248));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "   8   46   38   84 Sr       -80649.580       1.243      8677.5132     0.0148  B-  "
                            "-6755.1411     4.4114   83 913419.118       1.334" };
    const AME::Data data(line, 2020);
    data.setBetaDecayEnergy();
    REQUIRE(data.beta_decay_energy == Catch::Approx(-6755.1411));
  }
}


TEST_CASE("Error on beta decay energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  12   74   62  136 Sm    x  -66810.917     12.497     8205.916    0.092 B- -10549#     "
                            "196#    135 928275.527     13.416" };
    const AME::Data data(line, 2003);
    data.setBetaDecayEnergyError();
    REQUIRE(data.dbeta_decay_energy == Catch::Approx(196));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  19   43   24   67 Cr    x  -19270#        400#         8079#         6#      B-  14311# "
                            "      447#       66 979313#        429#" };
    const AME::Data data(line, 2020);
    data.setBetaDecayEnergyError();
    REQUIRE(data.dbeta_decay_energy == Catch::Approx(447));
  }
}


TEST_CASE("Atomic mass", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  48  150  102  252 No   -a   82881.097     12.993     7425.760    0.052 B-  -5956#     "
                            "249#    252 088976.521     13.948" };
    const AME::Data data(line, 2003);
    data.setAtomicMass();
    REQUIRE(data.atomic_mass == Catch::Approx(88976.521));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  13   90   77  167 Ir       -17072.448      18.346      7812.8254     0.1099  B- -10319# "
                            "      307#      166 981671.973      19.694" };
    const AME::Data data(line, 2020);
    data.setAtomicMass();
    REQUIRE(data.atomic_mass == Catch::Approx(981671.973));
  }
}


TEST_CASE("Error on atomic mass", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "   7   17   10   27 Ne    x    7069.949    110.384     7519.709    4.088 B-  12587.386  "
                            "110.439  27 007589.903    118.501" };
    const AME::Data data(line, 2003);
    data.setAtomicMassError();
    REQUIRE(data.datomic_mass == Catch::Approx(118.501));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "   9   72   63  135 Eu    x  -54148#        196#         8107#         1#      B-  -9898# "
                            "      445#      134 941870#        210#" };
    const AME::Data data(line, 2020);
    data.setAtomicMassError();
    REQUIRE(data.datomic_mass == Catch::Approx(210));
  }
}


TEST_CASE("Two neutron separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 107 Ag  47   17476.39   10.56  15134.04    3.56  -2804.47    4.85  -4842.17   10.81  "
                            "-9329.24    6.89  -9340.56    7.20" };
    const AME::Data data(line, 2003);
    data.setTwoNeutronSeparationEnergy();
    REQUIRE(data.s_2n == Catch::Approx(17476.39));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 200 Pb  82   16326.0512   13.2430   9874.4844   10.0154   3150.1704   10.4286  "
                            "-9309.1742   12.4688  -3993.7623   10.0095 -13524.5593   14.5741" };
    const AME::Data data(line, 2020);
    data.setTwoNeutronSeparationEnergy();
    REQUIRE(data.s_2n == Catch::Approx(16326.0512));
  }
}


TEST_CASE("Error on two neutron separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "0216 Bi  83    9068.49   15.84      *              5100#    300#     3616.70   11.76      * "
      "            -1657.09   11.46"
    };
    const AME::Data data(line, 2003);
    data.setTwoNeutronSeparationEnergyError();
    REQUIRE(data.ds_2n == Catch::Approx(15.84));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 127 I   53   16289.8290    3.5638  15306.0650    4.1469  -2185.2421    3.5651  "
                            "-2743.1898    6.2505  -9879.0523   32.0195  -7908.1479    3.6209" };
    const AME::Data data(line, 2020);
    data.setTwoNeutronSeparationEnergyError();
    REQUIRE(data.ds_2n == Catch::Approx(3.5638));
  }
}


TEST_CASE("Two proton separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "  93 Ag  47       *              2254#    718#    -1546#    746#        *              9292#    718#        *"
    };
    const AME::Data data(line, 2003);
    data.setTwoProtonSeparationEnergy();
    REQUIRE(data.s_2p == Catch::Approx(2254));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  87 As  33    8571.3977    4.2818  30451.7968   37.3791  -8785.7008    3.9608  "
                            "18273.7719    4.3492 -29147#      400#       6813.9437    3.9001" };
    const AME::Data data(line, 2020);
    data.setTwoProtonSeparationEnergy();
    REQUIRE(data.s_2p == Catch::Approx(30451.7968));
  }
}


TEST_CASE("Error on two proton separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 137 Sm  62   21310.80  160.33   6389.56   46.58   1882.08   62.98 -16812#    403#     "
                            "3884.96   44.01 -19834#    200#" };
    const AME::Data data(line, 2003);
    data.setTwoProtonSeparationEnergyError();
    REQUIRE(data.ds_2p == Catch::Approx(46.58));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  31 Ne  10    3360.8452  305.3058      *                -15913#      567#      "
                            "34303.7458  266.2130      *                 14635.6061  266.2373" };
    const AME::Data data(line, 2020);
    data.setTwoProtonSeparationEnergyError();
    REQUIRE(data.ds_2p == Catch::Approx(std::numeric_limits<double>::max()));
  }
}


TEST_CASE("Q alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      " 199 Pt  78   13112.57    3.04      *              -127.45  500.01   2154.70    3.15      * "
      "            -5881.57    3.10"
    };
    const AME::Data data(line, 2003);
    data.setQAlphaEnergy();
    REQUIRE(data.q_a == Catch::Approx(-127.45));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 254 Es  99   11443.0952   50.1345  11118#      200#       6617.2299    0.4743  -1458#    "
                            "  100#      -6223#      359#      -5422.7153    2.9980" };
    const AME::Data data(line, 2020);
    data.setQAlphaEnergy();
    REQUIRE(data.q_a == Catch::Approx(6617.229));
  }
}


TEST_CASE("Error on Q alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  24 Na  11   19378.31    0.42  25789.43   12.40 -10825.63    0.11  -8361.17    2.78 "
                            "-19036.83   79.54 -11015.66    1.29" };
    const AME::Data data(line, 2003);
    data.setQAlphaEnergyError();
    REQUIRE(data.dq_a == Catch::Approx(0.11));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  79 Cu  29    9687.8470  104.9864  35076#      609#     -14272#      413#      "
                            "20140.3165  104.9863 -34377#      707#       7003.8851  104.9974" };
    const AME::Data data(line, 2020);
    data.setQAlphaEnergyError();
    REQUIRE(data.dq_a == Catch::Approx(413));
  }
}


TEST_CASE("Q double beta", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 236 U   92   11842.93    0.33  12745.90    3.17   4573.10    0.90   -456.39    1.66  "
                            "-9097.99   50.01  -6669.66    0.89" };
    const AME::Data data(line, 2003);
    data.setQDoubleBetaMinusEnergy();
    REQUIRE(data.q_2bm == Catch::Approx(-456.39));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 282 Rg 111   12287#      793#       7346#      840#       9550#      104#      -5987#    "
                            "  711#      -1947#      840#      -8277#      710#" };
    const AME::Data data(line, 2020);
    data.setQDoubleBetaMinusEnergy();
    REQUIRE(data.q_2bm == Catch::Approx(-5987));
  }
}


TEST_CASE("Error on Q double beta", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "  46 S   16    6328#    802#    46724#   1063#   -20161#    861#    30419#    700#        * "
      "            10990#    709#"
    };
    const AME::Data data(line, 2003);
    data.setQDoubleBetaMinusEnergyError();
    REQUIRE(data.dq_2bm == Catch::Approx(700));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "0268 Rf 104   10803#      780#          *                  8040#      300#      -1324#    "
                            "  812#          *                 -6610#      761#" };
    const AME::Data data(line, 2020);
    data.setQDoubleBetaMinusEnergyError();
    REQUIRE(data.dq_2bm == Catch::Approx(812));
  }
}


TEST_CASE("Q ep", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "  10 N    7       *             -1300.72  400.00  -7228#    805#        *             19095.50  400.00      *"
    };
    const AME::Data data(line, 2003);
    data.setQEpsilonPEnergy();
    REQUIRE(data.q_ep == Catch::Approx(19095.50));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  79 Kr  36   20415.4743    3.9925  14420.7348    3.4807  -4697.7164    3.4812  "
                            "-8962.6255    8.1923  -4705.3092    3.4830 -15578.1804    4.7534" };
    const AME::Data data(line, 2020);
    data.setQEpsilonPEnergy();
    REQUIRE(data.q_ep == Catch::Approx(-4705.3092));
  }
}


TEST_CASE("Error on Q ep", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "   1 H    1       *                 *                 *                 *                 *                 *"
    };
    const AME::Data data(line, 2003);
    data.setQEpsilonPEnergyError();
    REQUIRE(data.dq_ep == Catch::Approx(std::numeric_limits<double>::max()));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 146 Pm  61   14180.8800    4.8688  13281.7143    4.7616   1907.0907    4.3238  "
                            "-2336.7573    6.5908  -7117.3410    8.1535  -6874.2711    4.1393" };
    const AME::Data data(line, 2020);
    data.setQEpsilonPEnergyError();
    REQUIRE(data.dq_ep == Catch::Approx(8.1535));
  }
}


TEST_CASE("Q B- n", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 125 Xe  54   18086.15    9.70  12598.11    2.20  -1065.83   25.99  -7524.09   11.27  "
                            "-3956.49    2.19 -13532.05    8.51" };
    const AME::Data data(line, 2003);
    data.setQBetaMinusNEnergy();
    REQUIRE(data.q_bm_n == Catch::Approx(-13532.05));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 248 Es  99   13662#      104#       8246#       80#       7160#       50#      -6649#    "
                            "  191#      -2479#       53#      -9445#      188#" };
    const AME::Data data(line, 2020);
    data.setQBetaMinusNEnergy();
    REQUIRE(data.q_bm_n == Catch::Approx(-9445));
  }
}


TEST_CASE("Error on Q B- n", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 115 Te  52   19858.36   39.52   8307.66   28.23   1457.12   28.77 -13405.99   30.46   "
                            "1209.17   28.12 -17338#    299#" };
    const AME::Data data(line, 2003);
    data.setQBetaMinusNEnergyError();
    REQUIRE(data.dq_bm_n == Catch::Approx(299));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  88 Br  35   11226.3421    4.4136  26331.7598    4.6807  -7287.3150    4.4789  "
                            "11893.0373    3.1671 -22387.0248    4.3492   1922.2456    3.1806" };
    const AME::Data data(line, 2020);
    data.setQBetaMinusNEnergyError();
    REQUIRE(data.dq_bm_n == Catch::Approx(3.1806));
  }
}


TEST_CASE("One neutron separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 151 Ho  67    9755.50   18.50   1604.10   12.43 -33431#    401#    14793.87   14.60   "
                            "9091.46   16.01  12554.77   18.35" };
    const AME::Data data(line, 2003);
    data.setOneNeutronSeparationEnergy();
    REQUIRE(data.s_n == Catch::Approx(9755.50));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 212 Bi  83    4330.1448    5.5996   4913.9019    2.6802  -4601.9446    8.9581  "
                            "17321.2932    1.8957  14360.6860    2.1336  11173.2521    6.2704" };
    const AME::Data data(line, 2020);
    data.setOneNeutronSeparationEnergy();
    REQUIRE(data.s_n == Catch::Approx(4330.1448));
  }
}


TEST_CASE("Error on one neutron separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 187 Tl  81   10324.70  184.61   1193.17   13.86      *             14443.13   17.52   "
                            "8769.67   12.92  13521.60   23.70" };
    const AME::Data data(line, 2003);
    data.setOneNeutronSeparationEnergyError();
    REQUIRE(data.ds_n == Catch::Approx(184.61));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  61 Cr  24    3659.3198    2.1726  16698.0714  181.9557  21725.5269    1.8965   "
                            "5824.9221  137.4131   2798.1368   95.8342  -5932.4313  183.3491" };
    const AME::Data data(line, 2020);
    data.setOneNeutronSeparationEnergyError();
    REQUIRE(data.ds_n == Catch::Approx(2.1726));
  }
}


TEST_CASE("One proton separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "  18 Na  11       *              -440.10   57.07      *             10904.31   54.31      * "
      "            13061.00  142.94"
    };
    const AME::Data data(line, 2003);
    data.setOneProtonSeparationEnergy();
    REQUIRE(data.s_p == Catch::Approx(-440.10));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  95 Rb  37    5398.8340   20.3074  11831.4984   23.5902  20895.9644   20.2512   "
                            "8956.5023   20.4006   7743.0766   20.4244  -4011.0925   21.3273" };
    const AME::Data data(line, 2020);
    data.setOneProtonSeparationEnergy();
    REQUIRE(data.s_p == Catch::Approx(11831.4984));
  }
}


TEST_CASE("Error on one proton separation energy", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  42 Ca  20   11480.63    0.06  10276.97    0.19 -44537#    298#     5698.94    0.18    "
                            "123.99    0.17    341.34    5.00" };
    const AME::Data data(line, 2003);
    data.setOneProtonSeparationEnergyError();
    REQUIRE(data.ds_p == Catch::Approx(0.19));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  94 Ag  47   14074#      566#        709#      545#          *                 13088#    "
                            "  528#      -1369#      582#      11814#      499#" };
    const AME::Data data(line, 2020);
    data.setOneProtonSeparationEnergyError();
    REQUIRE(data.ds_p == Catch::Approx(545));
  }
}


TEST_CASE("Q value on quadruple Beta minus", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 213 Rn  86    5109.97    6.27   4366.04    9.01 -17817.13   71.27  16659.70    6.07  "
                            "11137.63    9.52  15901.21    5.54" };
    const AME::Data data(line, 2003);
    data.setQQuadrupleBetaMinusEnergy();
    REQUIRE(data.q_4bm == Catch::Approx(-17817.13));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 160 Pm  61    4411.6056   10.2459  10459.6027   29.8782  14941.8349    2.3307  "
                            "11651.2916    2.4290   8463.5331    2.9609   1186.5692    3.7723" };
    const AME::Data data(line, 2020);
    data.setQQuadrupleBetaMinusEnergy();
    REQUIRE(data.q_4bm == Catch::Approx(14941.8349));
  }
}


TEST_CASE("Error on Q value of quadruple Beta minus", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 206 Ra  88   10345.37   88.32   2414.17   19.65      *             13667.43   30.49   "
                            "7567.83   24.00  15371.74   29.67" };
    const AME::Data data(line, 2003);
    data.setQQuadrupleBetaMinusEnergyError();
    REQUIRE(data.dq_4bm == Catch::Approx(std::numeric_limits<double>::max()));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 171 Os  76    8442.8644   20.6931   2682.2128   21.5303 -37636#      307#      "
                            "14822.8161   21.4990   6361.7064   35.8173  16242.0946   22.5546" };
    const AME::Data data(line, 2020);
    data.setQQuadrupleBetaMinusEnergyError();
    REQUIRE(data.dq_4bm == Catch::Approx(307));
  }
}


TEST_CASE("Q value of d alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  45 Mn  25   19585#    585#    -1058#    302#        *              7730#    370#    "
                            "-6239#    422#     8702#    357#" };
    const AME::Data data(line, 2003);
    data.setQDAlphaEnergy();
    REQUIRE(data.q_da == Catch::Approx(7730));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 128 Nd  60   12692#      361#       3276#      280#          *                 10504#    "
                            "  280#       2404#      361#      11774#      280#" };
    const AME::Data data(line, 2020);
    data.setQDAlphaEnergy();
    REQUIRE(data.q_da == Catch::Approx(10504));
  }
}


TEST_CASE("Error on Q value of d alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  72 Se  34   12849.39   33.80   7289.04   12.76      *              7159.51   51.44     "
                            "56.31   33.46   4852.60   12.13" };
    const AME::Data data(line, 2003);
    data.setQDAlphaEnergyError();
    REQUIRE(data.dq_da == Catch::Approx(51.44));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 116 Mo  42    4732#      640#      17619#      707#      38621#      500#       4460#    "
                            "  707#       3864#      640#      -9224#      583#" };
    const AME::Data data(line, 2020);
    data.setQDAlphaEnergyError();
    REQUIRE(data.dq_da == Catch::Approx(707));
  }
}


TEST_CASE("Q value of p alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ "  87 Mo  42   11209.89  491.98   5157.55  239.06      *             10165.00  316.25   "
                            "-952#    372#     9444#    297#" };
    const AME::Data data(line, 2003);
    data.setQPAlphaEnergy();
    REQUIRE(data.q_pa == Catch::Approx(-952));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 248 No 102       *                  2536#      305#          *                 15284#    "
                            "  343#      10228#      343#      16143#      297#" };
    const AME::Data data(line, 2020);
    data.setQPAlphaEnergy();
    REQUIRE(data.q_pa == Catch::Approx(10228));
  }
}


TEST_CASE("Error on Q value of p alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{
      "0 67 Cr  24    2324#    918#        *             44694#    699#     2914#   1063#     1213#    988#        *"
    };
    const AME::Data data(line, 2003);
    data.setQPAlphaEnergyError();
    REQUIRE(data.dq_pa == Catch::Approx(988));
  }

  SECTION("Post 2020")
  {
    const std::string line{ "  38 S   16    8036.1081    7.1745  15154.1746   38.6196  -4802.7126    7.1747   "
                            "4100.6358   14.9467   2860.6485    7.4107  -6823.3336   36.5675" };
    const AME::Data data(line, 2020);
    data.setQPAlphaEnergyError();
    REQUIRE(data.dq_pa == Catch::Approx(7.4107));
  }
}


TEST_CASE("Q value on n alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 157 Sm  62    5434.42   51.07   9802.16   60.82   2694.46   50.62  10950.62   58.51   "
                            "6629.02   67.26   4604.44  124.56" };
    const AME::Data data(line, 2003);
    data.setQNAlphaEnergy();
    REQUIRE(data.q_na == Catch::Approx(4604.44));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 163 Re  75   11621#      201#       -707.9928    5.6330      *                 15216#    "
                            "  201#       8190.8755  150.9526  15467.8491   57.3909" };
    const AME::Data data(line, 2020);
    data.setQNAlphaEnergy();
    REQUIRE(data.q_na == Catch::Approx(15467.8491));
  }
}


TEST_CASE("Error on Q value of n alpha", "[AMEData]")
{
  SECTION("Pre 2020")
  {
    const std::string line{ " 137 Sb  51    3451#    499#    11043#    643#    26287#    401#    11251#    566#    "
                            "11401#    413#    -2588#    566#" };
    const AME::Data data(line, 2003);
    data.setQNAlphaEnergyError();
    REQUIRE(data.dq_na == Catch::Approx(566));
  }

  SECTION("Post 2020")
  {
    const std::string line{ " 127 Ba  56    8219.3598   16.8867   5756.2431   15.3714 -26908#      300#      "
                            "11982.5943   13.7412   3787.1606   14.5849  10495.8520   11.4377" };
    const AME::Data data(line, 2020);
    data.setQNAlphaEnergyError();
    REQUIRE(data.dq_na == Catch::Approx(11.4377));
  }
}


TEST_CASE("Reading values", "[.Benchmark]")
{
  SECTION("uint8")
  {
    const std::string line{ " 127 Ba  56    8219.3598   16.8867   5756.2431   15.3714 -26908#      300#      "
                            "11982.5943   13.7412   3787.1606   14.5849  10495.8520   11.4377" };
    const AME::Data data(line, 2020);

    BENCHMARK_ADVANCED("Set an 8 bit unsigned int")(Catch::Benchmark::Chronometer meter)
    {
      meter.measure([&data]() { data.setZ(); });
    };
  }

  SECTION("uint16")
  {
    const std::string line{ " 127 Ba  56    8219.3598   16.8867   5756.2431   15.3714 -26908#      300#      "
                            "11982.5943   13.7412   3787.1606   14.5849  10495.8520   11.4377" };
    const AME::Data data(line, 2020);

    BENCHMARK_ADVANCED("Set a 16 bit unsigned int")(Catch::Benchmark::Chronometer meter)
    {
      meter.measure([&data]() { data.setA(); });
    };
  }
}

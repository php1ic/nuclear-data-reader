#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER

#include "nuclear-data-reader/isotope.hpp"

#include <catch2/catch.hpp>


TEST_CASE("", "[Isotope]")
{
}


// TEST_CASE("CSV header line", "[Isotope]")
//{
//  const std::string nubase_gs03{
//    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
//  };
//  Isotope nubase_gs03_isotope(nubase_gs03);
//
//  std::string header{
//    "A,Z,N,Symbol,DecayMode,ExperimentallyMeasured,Own,NUBASEMass,ErrorNUBASEMass,AMEMass,ErrorAMEMass,HalfLife,"
//    "SingleNeutronSeparationEnergy,ErrorSingleNeutronSeparationEnergy,SingleProtonSeparationEnergy,"
//    "ErrorSingleProtonSeparationEnergy,DoubleNeutronSeparationEnergy,ErrorDoubleNeutronSeparationEnergy,"
//    "DoubleProtonSeparationEnergy,ErrorDoubleProtonSeparationEnergy,DiscoveryYear"
//  };
//
//  REQUIRE_THAT(nubase_gs03_isotope.CSVHeader(), Catch::Matches(header));
//}


// TEST_CASE("Write as csv", "[Isotope]")
//{
//  const std::string nubase_gs03{
//    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
//  };
//  Isotope nubase_gs03_isotope(nubase_gs03);
//
//  int state{ 0 };
//  nubase_gs03_isotope.setExperimental();
//  nubase_gs03_isotope.setA();
//  nubase_gs03_isotope.setZ();
//  nubase_gs03_isotope.setSymbol(Converter::ZToSymbol(nubase_gs03_isotope.Z));
//  nubase_gs03_isotope.setN();
//  nubase_gs03_isotope.setState(state);
//  nubase_gs03_isotope.setNubaseMassExcess();
//  nubase_gs03_isotope.setNubaseMassExcessError();
//  nubase_gs03_isotope.setSpinParity();
//  nubase_gs03_isotope.setHalfLife();
//  nubase_gs03_isotope.decay = "stable";
//  // nubase_gs03_isotope.setDecayMode(pnSide, table_year);
//  // nubase_gs03_isotope.setNeutronOrProtonRich(pnSide);
//
//  std::string line{
//    "10,3,7,Li,stable,0,true,33051.0,15.0,-4737.00141,0.00016,2.00e-21,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1900"
//  };
//
//  REQUIRE_THAT(nubase_gs03_isotope.writeAsCSV(), Catch::Matches(line));
//}

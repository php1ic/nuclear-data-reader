#include "nuclear-data-reader/isotope.hpp"
#include "nuclear-data-reader/massTable.hpp"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("A json item is correctly output", "[Isotope]")
{
  MassTable table(2003);
  table.setFilePaths();
  REQUIRE(table.populateInternalMassTable());

  // Pick a random isotope to check output of
  const int id{ 56 };
  std::string data{
    "{\"A\":14,\"Z\":5,\"N\":9,\"Symbol\":\"B\",\"Decay\":\"B-\",\"Experimental\":0,\"NubaseMassExcess\":23664.0000,"
    "\"ErrorNubaseMassExcess\":21.0000,\"AMEMassExcess\":23663.6830,\"ErrorAMEMassExcess\":21.2130,\"HalfLife\":1."
    "250e-02,\"SingleNeutronSeparationEnergy\":969.8000,\"ErrorSingleNeutronSeparationEnergy\":21.2400,"
    "\"SingleProtonSeparationEnergy\":16873.1000,\"ErrorSingleProtonSeparationEnergy\":74.6700,"
    "\"DoubleNeutronSeparationEnergy\":5847.8000,\"ErrorDoubleNeutronSeparationEnergy\":21.2600,"
    "\"DoubleProtonSeparationEnergy\":41010.0000,\"ErrorDoubleProtonSeparationEnergy\":997.0000,"
    "\"BindingEnergyPerA\":6101.6450,\"ErrorBindingEnergyPerA\":1.5150,\"AtomicMass\":25404.0090,\"ErrorAtomicMass\":"
    "22.7730,\"BetaDecayEnergy\":20643.7900,\"ErrorBetaDecayEnergy\":21.2130,\"QAlpha\":11811.8100,\"ErrorQAlpha\":"
    "26.0500,\"Q2B-\":20800.2700,\"ErrorQ2B-\":21.2100,\"Qepsilon_p\":null,\"ErrorQepsilon_p\":null,\"QB-n\":12467."
    "3000,\"ErrorQB-n\":21.2100,\"Q4B-\":-8994.0000,\"ErrorQ4B-\":401.0000,\"QdAlpha\":9297.9000,\"ErrorQdAlpha\":25."
    "9800,\"QpAlpha\":8353.6700,\"ErrorQpAlpha\":22.1500,\"QnAlpha\":-11487.2000,\"ErrorQnAlpha\":28.6800,\"Year\":"
    "1900}"
  };
  REQUIRE(table.fullDataTable.at(id).writeAsJSON(false) == data);
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
//  // nubase_gs03_isotope.setDecayMode(neutron_rich, table_year);
//  // nubase_gs03_isotope.setNeutronOrProtonRich(neutron_rich);
//
//  std::string line{
//    "10,3,7,Li,stable,0,true,33051.0,15.0,-4737.00141,0.00016,2.00e-21,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1900"
//  };
//
//  REQUIRE_THAT(nubase_gs03_isotope.writeAsCSV(), Catch::Matches(line));
//}

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
  const std::string data{
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


TEST_CASE("A csv item is correctly output", "[Isotope]")
{
  MassTable table(2020);
  table.setFilePaths();
  REQUIRE(table.populateInternalMassTable());

  // Pick a random isotope to check output of
  const int id{ 95 };
  const std::string data{
    "19,8,11,O,B-,0,3332.9000,2.6000,3332.8580,2.6370,2.647e+01,3955.6439,2.6370,17069.2807,18.7558,12001.0141,2.6370,"
    "32276.9638,17.5639,7566.4952,0.1388,3577.9690,2.8300,4820.3029,2.6370,-8965.2032,2.7556,1580.8043,2.6418,-28875."
    "3789,30.1157,-5611.5727,2.6774,-28505.5362,60.0590,6173.5861,15.2300,2513.0058,3.5000,-4714.8732,4.4445,1936"
  };
  REQUIRE(table.fullDataTable.at(id).writeAsCSV() == data);
}


TEST_CASE("The csv header is populated correctly", "[Isotope]")
{
  const std::string header{
    "A,Z,N,Symbol,Decay,Experimental,NubaseMassExcess,ErrorNubaseMassExcess,AMEMassExcess,ErrorAMEMassExcess,HalfLife,"
    "SingleNeutronSeparationEnergy,ErrorSingleNeutronSeparationEnergy,SingleProtonSeparationEnergy,"
    "ErrorSingleProtonSeparationEnergy,DoubleNeutronSeparationEnergy,ErrorDoubleNeutronSeparationEnergy,"
    "DoubleProtonSeparationEnergy,ErrorDoubleProtonSeparationEnergy,BindingEnergyPerA,ErrorBindingEnergyPerA,"
    "AtomicMass,ErrorAtomicMass,BetaDecayEnergy,ErrorBetaDecayEnergy,QAlpha,ErrorQAlpha,Q2B-,ErrorQ2B-,Qepsilon_p,"
    "ErrorQepsilon_p,QB-n,ErrorQB-n,Q4B-,ErrorQ4B-,QdAlpha,ErrorQdAlpha,QpAlpha,ErrorQpAlpha,QnAlpha,ErrorQnAlpha,Year"
  };
  REQUIRE(Isotope::writeCSVHeader() == header);
}

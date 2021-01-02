#include "nuclear-data-reader/massTable.hpp"

#include "nuclear-data-reader/ame_line_position.hpp"
#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/isotope.hpp"
#include "nuclear-data-reader/nubase_data.hpp"
#include <type_traits>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <array>
#include <climits>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <utility>


bool MassTable::populateInternalMassTable()
{
  setFilePaths(table_year);

  // Read mass table
  if (!readNUBASE(NUBASE_masstable))
    {
      fmt::print("Nuclear data has not been read, exiting...");
      return false;
    }

  if (!readAME(AME_masstable))
    {
      fmt::print("Values from AME were not read.\n");
    }

  if (!readAMEReactionFile(AME_reaction_1, 1))
    {
      fmt::print("Reaction values from first file not read.\n");
    }

  if (!readAMEReactionFile(AME_reaction_2, 2))
    {
      fmt::print("Reaction values from first file not read.\n");
    }

  return mergeData();
}


AME::Data MassTable::parseAMEMassFormat(const std::string& line) const
{
  AME::Data data(line);

  data.setA();
  data.setZ();
  data.setN();

  data.setMassExcess();
  data.setMassExcessError();

  data.setBetaDecayEnergy();
  data.setBetaDecayEnergyError();

  data.setAtomicMass();
  data.setAtomicMassError();

  return data;
}


std::vector<AME::Data>::iterator MassTable::getMatchingIsotope(const std::string& line, const int reactionFile) const
{
  // Check that the mass table has already been populated
  if (ameDataTable.empty())
    {
      fmt::print("Mass table must be read first\n");
      return ameDataTable.end();
    }

  // A & Z are in the same place for both reaction files, but lets not assume they will be forever
  const int A = (reactionFile == 1) ? AME::Data::getReaction_1_A(line) : AME::Data::getReaction_2_A(line);
  const int Z = (reactionFile == 1) ? AME::Data::getReaction_1_Z(line) : AME::Data::getReaction_2_Z(line);

  // Look for the correct isotope in the existing data table
  auto isotope = std::find_if(
      ameDataTable.begin(), ameDataTable.end(), [A, Z](const auto n) -> bool { return (n.A == A && n.Z == Z); });

  // Get out if it doesn't exist
  if (isotope == ameDataTable.end())
    {
      fmt::print("**WARNING**: No matching mass data found for A={}, Z={}\n", A, Z);
      return ameDataTable.end();
    }

  // As we don't construct a AME::Data instance we need to make sure we are reading the correct line/data
  isotope->full_data = line;
  // Make sure the line is the correct length so accessing line locations doesn't cause a crash
  isotope->full_data.resize(125, ' ');
  // Strip '#' characters (May not be needed)
  isotope->setExperimental();

  return isotope;
}


bool MassTable::parseAMEReactionTwoFormat(const std::string& line) const
{
  auto isotope = getMatchingIsotope(line, 2);

  if (isotope == ameDataTable.end())
    {
      return false;
    }

  isotope->setOneNeutronSeparationEnergy();
  isotope->setOneNeutronSeparationEnergyError();

  isotope->setOneProtonSeparationEnergy();
  isotope->setOneProtonSeparationEnergyError();

  isotope->setQQuadrupleBetaMinusEnergy();
  isotope->setQQuadrupleBetaMinusEnergyError();

  isotope->setQDAlphaEnergy();
  isotope->setQDAlphaEnergyError();

  isotope->setQPAlphaEnergy();
  isotope->setQPAlphaEnergyError();

  isotope->setQNAlphaEnergy();
  isotope->setQNAlphaEnergyError();

  return true;
}


bool MassTable::parseAMEReactionOneFormat(const std::string& line) const
{
  auto isotope = getMatchingIsotope(line, 2);

  if (isotope == ameDataTable.end())
    {
      return false;
    }

  isotope->setTwoNeutronSeparationEnergy();
  isotope->setTwoNeutronSeparationEnergyError();

  isotope->setTwoProtonSeparationEnergy();
  isotope->setTwoProtonSeparationEnergyError();

  isotope->setQAlphaEnergy();
  isotope->setQAlphaEnergyError();

  isotope->setQDoubleBetaMinusEnergy();
  isotope->setQDoubleBetaMinusEnergyError();

  isotope->setQEpsilonPEnergy();
  isotope->setQEpsilonPEnergyError();

  isotope->setQBetaMinusNEnergy();
  isotope->setQBetaMinusNEnergyError();

  return true;
}


bool MassTable::readAME(const std::filesystem::path& ameTable) const
{
  fmt::print("Reading {} for AME mass excess values <--", ameTable);

  std::ifstream file(ameTable, std::ios::binary);

  if (!file.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened, does it exist?\n\n", ameTable);
      return false;
    }

  // Skip the header of the file
  for (int i = 0; i < AME::HEADER_LENGTH; ++i)
    {
      file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  std::string line;
  while (std::getline(file, line))
    {
      auto nuclide = parseAMEMassFormat(line);

      ameDataTable.emplace_back(nuclide);
    }

  file.close();

  fmt::print("--> done\n");
  return true;
}


bool MassTable::readAMEReactionFile(const std::filesystem::path& reactionFile, const int fileNumber) const
{
  fmt::print("Reading {} for reaction data <--", reactionFile);

  std::ifstream file(reactionFile, std::ios::binary);

  if (!file.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened, does it exist?\n\n", reactionFile);
      return false;
    }

  // Skip the header of the file
  for (int i = 0; i < AME::HEADER_LENGTH; ++i)
    {
      file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  std::string line;
  while (std::getline(file, line))
    {
      if (fileNumber == 1)
        {
          parseAMEReactionOneFormat(line);
        }
      else if (fileNumber == 2)
        {
          parseAMEReactionTwoFormat(line);
        }
    }

  fmt::print("--> done\n");
  return true;
}


NUBASE::Data MassTable::parseNUBASEFormat(const std::string& line) const
{
  NUBASE::Data data = NUBASE::Data(line);

  data.setExperimental();

  data.setA();
  data.setZ();
  data.setSymbol(Converter::ZToSymbol(data.Z));
  data.setN();

  data.setState();

  // For the non ground-state entires we have enough data to attach this level to the appropriate isotope
  if (data.level > 0)
    {
      return data;
    }

  data.setMassExcess();
  data.setMassExcessError();

  data.setSpinParity();

  data.setHalfLife();

  // Discovery year was added after 2003
  if (table_year != 3)
    {
      data.setYear();
    }

  data.setDecayMode(table_year);

  if (data.decay == "stable")
    {
      pnSide.at(data.Z) = true;
    }

  data.setNeutronOrProtonRich(pnSide.at(data.Z));

  return data;
}


bool MassTable::readNUBASE(const std::filesystem::path& nubaseTable)
{
  fmt::print("Reading {} for nuclear values <--", nubaseTable);
  std::ifstream file(nubaseTable, std::ios::binary);

  if (!file.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened, does it exist?\n\n", nubaseTable);
      return false;
    }

  std::string line;

  while (std::getline(file, line))
    {
      // Sorry to these authors, but the format for the data from your paper is particularly annoying, so is skipped
      //
      // 2012 Theoretical but don't contain a '#' and give no ME value (26 times)
      // 168 0630W  168Eu                                                        >300ns                  12Ku26i  2012
      // B- ?;B-n ?
      //
      if (line.find("non-exist") != std::string::npos || line.find("12Ku26i") != std::string::npos)
        {
          continue;
        }

      auto nuclide = parseNUBASEFormat(line);

      // Merge isomers with their existing ground-state
      // We don't want to add a new entry to the vector so skip that step
      if (nuclide.level != 0)
        {
          nuclide.setIsomerData(nubaseDataTable);
          continue;
        }

      nubaseDataTable.emplace_back(nuclide);
    }

  file.close();

  fmt::print("--> done\n");
  return true;
}


bool MassTable::mergeData()
{
  if (ameDataTable.size() == nubaseDataTable.size())
    {
      fmt::print(":) Tables are equal size\n");
    }
  else
    {
      fmt::print(":( Tables have different sizes\n");
    }

  for (const auto& nubase_data : nubaseDataTable)
    {
      const auto ame_data =
          std::find_if(ameDataTable.cbegin(), ameDataTable.cend(), [&nubase_data](const auto n) -> bool {
            return (n.A == nubase_data.A && n.Z == nubase_data.Z);
          });

      fullDataTable.emplace_back(Isotope(*ame_data, nubase_data));
    }

  return true;
}


void MassTable::setFilePaths(const int tableYear) const noexcept
{
  switch (tableYear)
    {
      default:
      case 3:
        NUBASE_masstable = MassTable::getAbsolutePath() / "2003" / "nubtab03.asc";
        AME_masstable    = MassTable::getAbsolutePath() / "2003" / "mass.mas03";
        AME_reaction_1   = MassTable::getAbsolutePath() / "2003" / "rct1.mas03";
        AME_reaction_2   = MassTable::getAbsolutePath() / "2003" / "rct2.mas03";
        break;
      case 12:
        NUBASE_masstable = MassTable::getAbsolutePath() / "2012" / "nubase.mas12";
        AME_masstable    = MassTable::getAbsolutePath() / "2012" / "mass.mas12";
        AME_reaction_1   = MassTable::getAbsolutePath() / "2012" / "rct1.mas12";
        AME_reaction_2   = MassTable::getAbsolutePath() / "2012" / "rct2.mas12";
        break;
      case 16:
        NUBASE_masstable = MassTable::getAbsolutePath() / "2016" / "nubase2016.txt";
        AME_masstable    = MassTable::getAbsolutePath() / "2016" / "mass16.txt";
        AME_reaction_1   = MassTable::getAbsolutePath() / "2016" / "rct1.mas16";
        AME_reaction_2   = MassTable::getAbsolutePath() / "2016" / "rct2.mas16";
        break;
    }
}


bool MassTable::outputTableToCSV() const
{
  auto outfile = NUBASE_masstable;
  outfile.replace_extension(".csv");

  fmt::print("New file: {}\n", outfile);
  std::ofstream out(outfile);

  if (!out.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened", outfile);
      return false;
    }

  // fmt::print(out, "{}\n", Isotope::CSVHeader());
  // for (const auto& isotope : theTable)
  //  {
  //    fmt::print(out, "{}\n", isotope.writeAsCSV());
  //  }
  out.close();

  return true;
}


bool MassTable::outputTableToJSON() const
{
  const std::filesystem::path outfile{ "test_output.json" };

  fmt::print("New file: {}\n", outfile);
  std::ofstream out(outfile);

  if (!out.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened", outfile);
      return false;
    }

  fmt::print(out, "[\n");
  // The final element can't have a trailing comma, otherwise we'd use a range loop here
  for (auto isotope = fullDataTable.cbegin(); isotope != fullDataTable.cend(); ++isotope)
    {
      fmt::print(out, "{}{}", isotope->writeAsJSON(), (isotope != std::prev(fullDataTable.end(), 1)) ? ",\n" : "");
    }
  fmt::print(out, "\n]\n");
  out.close();

  return true;
}

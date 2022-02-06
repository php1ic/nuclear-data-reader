#include "nuclear-data-reader/massTable.hpp"

#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/isotope.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <fmt/os.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>

void MassTable::setFilePaths() const
{
  const auto data_path{ std::filesystem::path{ NDR_DATA_PATH } / fmt::to_string(year) };

  switch (year)
    {
      default:
      case 2003:
        NUBASE_masstable = data_path / "nubtab03.asc";
        AME_masstable    = data_path / "mass.mas03";
        AME_reaction_1   = data_path / "rct1.mas03";
        AME_reaction_2   = data_path / "rct2.mas03";
        break;
      case 2012:
        NUBASE_masstable = data_path / "nubtab12.asc";
        AME_masstable    = data_path / "mass.mas12";
        AME_reaction_1   = data_path / "rct1.mas12";
        AME_reaction_2   = data_path / "rct2.mas12";
        break;
      case 2016:
        NUBASE_masstable = data_path / "nubase2016.txt";
        AME_masstable    = data_path / "mass16.txt";
        AME_reaction_1   = data_path / "rct1-16.txt";
        AME_reaction_2   = data_path / "rct2-16.txt";
        break;
      case 2020:
        NUBASE_masstable = data_path / "nubase_1.mas20";
        AME_masstable    = data_path / "mass.mas20";
        AME_reaction_1   = data_path / "rct1.mas20";
        AME_reaction_2   = data_path / "rct2.mas20";
        break;
    }
}


bool MassTable::populateInternalMassTable()
{
  setFilePaths();

  // Read mass table
  if (!readNUBASE(NUBASE_masstable))
    {
      fmt::print("Nuclear data has not been read, exiting...");
      return false;
    }

  if (!readAMEMassFile(AME_masstable))
    {
      fmt::print("Values from AME were not read.\n");
    }

  if (!readAMEReactionFileOne(AME_reaction_1))
    {
      fmt::print("Reaction values from first file not read.\n");
    }

  if (!readAMEReactionFileTwo(AME_reaction_2))
    {
      fmt::print("Reaction values from first file not read.\n");
    }

  return mergeData();
}


AME::Data MassTable::parseAMEMassFormat(const std::string& line) const
{
  AME::Data data(line, year);

  data.setA();
  data.setZ();
  data.setN();

  data.setMassExcess();
  data.setMassExcessError();

  data.setBindingEnergyPerA();
  data.setBindingEnergyPerAError();

  data.setBetaDecayEnergy();
  data.setBetaDecayEnergyError();

  data.setAtomicMass();
  data.setAtomicMassError();

  return data;
}


std::vector<AME::Data>::iterator MassTable::getMatchingIsotope(const std::string& line,
                                                               const uint8_t reactionFile) const
{
  // Check that the mass table has already been populated
  if (ameDataTable.empty())
    {
      fmt::print("Mass table must be read first\n");
      return ameDataTable.end();
    }

  const AME::Data data(line, year);

  // A & Z are in the same place for both reaction files, but lets not assume they will be forever
  const auto A = (reactionFile == 1) ? data.getReaction_1_A(line) : data.getReaction_2_A(line);
  const auto Z = (reactionFile == 1) ? data.getReaction_1_Z(line) : data.getReaction_2_Z(line);

  // Look for the correct isotope in the existing data table
  auto isotope = std::find_if(
      ameDataTable.begin(), ameDataTable.end(), [A, Z](const auto& ame) -> bool { return (ame.A == A && ame.Z == Z); });

  // Get out if it doesn't exist
  if (isotope == ameDataTable.end())
    {
      fmt::print("**WARNING**: No matching mass data found for A={}, Z={}\n", A, Z);
      return ameDataTable.end();
    }

  // As we don't construct a AME::Data instance we need to make sure we are reading the correct line/data
  isotope->full_data = line;
  // Make sure the line is the correct length so accessing line locations doesn't cause a crash
  isotope->full_data.resize(line_length, ' ');
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
  auto isotope = getMatchingIsotope(line, 1);

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


bool MassTable::readAMEMassFile(const std::filesystem::path& ameTable) const
{
  fmt::print("Reading {} for AME mass excess values <--", ameTable);

  if (!std::filesystem::exists(ameTable))
    {
      fmt::print("\n***ERROR***: {} does not exist?\n\n", ameTable);
      return false;
    }

  std::ifstream file(ameTable, std::ios::binary);

  const AME::Data data("", year);
  uint16_t line_number = 0;
  for (line_number = 0; line_number < data.mass_position.HEADER; ++line_number)
    {
      file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  std::string line;
  while (std::getline(file, line) && line_number < data.mass_position.FOOTER)
    {
      ameDataTable.emplace_back(parseAMEMassFormat(line));
      ++line_number;
    }

  fmt::print("--> done\n");
  return true;
}


bool MassTable::readAMEReactionFileOne(const std::filesystem::path& reactionFile) const
{
  fmt::print("Reading {} for reaction data <--", reactionFile);

  if (!std::filesystem::exists(reactionFile))
    {
      fmt::print("\n***ERROR***: {} does not exist?\n\n", reactionFile);
      return false;
    }

  std::ifstream file(reactionFile, std::ios::binary);

  const AME::Data data("", year);
  uint16_t line_number = 0;
  for (line_number = 0; line_number < data.r1_position.R1_HEADER; ++line_number)
    {
      file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  std::string line;
  while (std::getline(file, line) && line_number < data.r1_position.R1_FOOTER)
    {
      ++line_number;
      if (!parseAMEReactionOneFormat(line))
        {
          fmt::print("**WARNING**: No matching isotope found for\n{}\n", line);
        }
    }

  fmt::print("--> done\n");
  return true;
}


bool MassTable::readAMEReactionFileTwo(const std::filesystem::path& reactionFile) const
{
  fmt::print("Reading {} for reaction data <--", reactionFile);

  if (!std::filesystem::exists(reactionFile))
    {
      fmt::print("\n***ERROR***: {} does not exist?\n\n", reactionFile);
      return false;
    }

  std::ifstream file(reactionFile, std::ios::binary);

  const AME::Data data("", year);
  uint16_t line_number = 0;
  for (line_number = 0; line_number < data.r2_position.R2_HEADER; ++line_number)
    {
      file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  std::string line;
  while (std::getline(file, line) && line_number < data.r2_position.R2_FOOTER)
    {
      ++line_number;
      // skip repeated header which only happens in the 2020 file (so far)
      if (year == uint16_t{ 2020 } && line.find("1 A  elt") != std::string::npos)
        {
          continue;
        }

      if (!parseAMEReactionTwoFormat(line))
        {
          fmt::print("**WARNING**: No matching isotope found for\n{}\n", line);
        }
    }

  fmt::print("--> done\n");
  return true;
}


NUBASE::Data MassTable::parseNUBASEFormat(const std::string& line) const
{
  NUBASE::Data data(line, year);

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

  data.setYear();

  data.setDecayMode();

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

  if (!std::filesystem::exists(nubaseTable))
    {
      fmt::print("\n***ERROR***: {} couldn't be opened, does it exist?\n\n", nubaseTable);
      return false;
    }

  std::ifstream file(nubaseTable, std::ios::binary);

  const NUBASE::Data data("", year);
  uint16_t line_number = 0;
  for (line_number = 0; line_number < data.position.HEADER; ++line_number)
    {
      file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  std::string line;

  while (std::getline(file, line) && line_number < data.position.FOOTER)
    {
      ++line_number;
      if (line.find("non-exist") != std::string::npos)
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

  fmt::print("--> done\n");
  return true;
}


bool MassTable::mergeData() const
{
  fmt::print("Merging AME and NUBASE data <--");
  if (ameDataTable.size() != nubaseDataTable.size())
    {
      fmt::print("\n**WARNING** The AME data ({}) has a different number of isotopes to NUBASE ({})\n",
                 ameDataTable.size(),
                 nubaseDataTable.size());
    }

  for (const auto& nubase : nubaseDataTable)
    {
      const auto ame = std::find_if(ameDataTable.cbegin(), ameDataTable.cend(), [&nubase](const auto& AME) -> bool {
        return (AME.A == nubase.A && AME.Z == nubase.Z);
      });

      if (ame != ameDataTable.cend())
        {
          // Isotope(AME, NUBASE))
          fullDataTable.emplace_back(Isotope(*ame, nubase));
        }
      // fmt::print("{} {}\n", nubase.A, nubase.Z);
    }
  fmt::print("--> done\n");
  return true;
}


// bool MassTable::outputTableToCSV() const
//{
//  auto outfile = NUBASE_masstable;
//  outfile.replace_extension(".csv");
//
//  fmt::print("New file: {}\n", outfile);
//  std::ofstream out(outfile);
//
//  if (!out.is_open())
//    {
//      fmt::print("\n***ERROR***: {} couldn't be opened", outfile);
//      return false;
//    }
//
//  // fmt::print(out, "{}\n", Isotope::CSVHeader());
//  // for (const auto& isotope : theTable)
//  //  {
//  //    fmt::print(out, "{}\n", isotope.writeAsCSV());
//  //  }
//
//  return true;
//}


bool MassTable::outputTableToJSON() const
{
  const auto outfile = fmt::format("masstable_{}.json", year);

  fmt::print("New file: {}\n", outfile);
  auto out = fmt::output_file(outfile);

  out.print("[\n");
  // The final element can't have a trailing comma, otherwise we'd use a range loop here
  for (auto isotope = fullDataTable.cbegin(); isotope != fullDataTable.cend(); ++isotope)
    {
      out.print("{}{}", isotope->writeAsJSON(), (isotope != std::prev(fullDataTable.end(), 1)) ? ",\n" : "");
    }
  out.print("\n]\n");

  return true;
}

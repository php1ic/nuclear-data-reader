#include "nuclear-data-reader/massTable.hpp"

#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/isotope.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fmt/std.h>

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
      case 1983:
        // No NUBASE table published this year
        AME_masstable  = data_path / "mass.mas83";
        AME_reaction_1 = data_path / "rct1.mas83";
        AME_reaction_2 = data_path / "rct2.mas83";
        break;
      case 1993:
        // No NUBASE table published this year
        AME_masstable  = data_path / "mass_exp.mas93";
        AME_reaction_1 = data_path / "rct1_exp.mas93";
        AME_reaction_2 = data_path / "rct2_exp.mas93";
        break;
      case 1995:
      case 1997:
        // Quote from G. Audi in a conference proceedings from 2000. See README for link
        //
        // The most recently published mass table from the "Atomic Mass Evaluation" is of December 1995 (AME'95).
        // Urgency in having the first NUBASE evaluation completed, delayed the planned update of an AME for
        // 1997, since the two evaluators of the AME are also collaborators of NUBASE. The NUBASE evaluation was
        // thus published for the first time in September 1997. In order to have consistency between the two
        // tables, it was decided that the masses in NUBASE'97 should be exactly those from AME'95. The few cases
        // for which new data required a change were only mentioned in the table and discussed in the accompanying text
        NUBASE_masstable = data_path / "nubtab97.asc";
        AME_masstable    = data_path / "mass_exp.mas95";
        AME_reaction_1   = data_path / "rct1_exp.mas95";
        AME_reaction_2   = data_path / "rct2_exp.mas95";
        break;
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
      default:
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

  // There is always AME data
  readAME();

  // There is only NUBASE data after 1993
  const auto attempt_combination = (year > LAST_YEAR_AME_ONLY);
  const auto read_nubase         = attempt_combination ? readNUBASE(NUBASE_masstable) : false;

  bool successful_combination = false;
  if (attempt_combination && read_nubase)
    {
      successful_combination = mergeData();
    }
  else if (attempt_combination)
    {
      if (!read_nubase)
        {
          fmt::print("NUBASE data has not been read\n");
        }

      const auto nubase_data = NUBASE::Data("", LAST_YEAR_AME_ONLY);
      for (const auto& ame : ameDataTable)
        {
          fullDataTable.emplace_back(ame, nubase_data);
        }

      successful_combination = true;
    }

  return successful_combination;
}


bool MassTable::readAME() const
{
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

  return true;
}


AME::Data MassTable::parseAMEMassFormat(const std::string& line) const
{
  AME::Data data(line, year);

  data.setZ();
  data.setN();
  data.setA(year);

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


std::vector<AME::Data>::iterator
MassTable::getMatchingIsotope(const std::string& line, const uint16_t table_A, const uint16_t table_Z) const
{
  // Check that the mass table has already been populated
  if (ameDataTable.empty())
    {
      fmt::print("Mass table must be read first\n");
      return ameDataTable.end();
    }

  // Look for the correct isotope in the existing data table
  auto isotope = std::find_if(ameDataTable.begin(), ameDataTable.end(), [table_A, table_Z](const auto& ame) -> bool {
    return (ame.A == table_A && ame.Z == table_Z);
  });

  // Get out if it doesn't exist
  if (isotope == ameDataTable.end())
    {
      fmt::print("**WARNING**: No matching mass data found for A={}, Z={}\n", table_A, table_Z);
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


bool MassTable::parseAMEReactionTwoFormat(const std::string& line, const uint16_t table_A, const uint16_t table_Z) const
{
  auto isotope = getMatchingIsotope(line, table_A, table_Z);

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


bool MassTable::parseAMEReactionOneFormat(const std::string& line, const uint16_t table_A, const uint16_t table_Z) const
{
  auto isotope = getMatchingIsotope(line, table_A, table_Z);

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
      ++line_number;
      // skip repeated header
      if (year == uint16_t{ 1983 }
          && (line.find("MASS EXCESS") != std::string::npos || line.find("(keV)") != std::string::npos))
        {
          continue;
        }

      ameDataTable.emplace_back(parseAMEMassFormat(line));
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
  uint16_t current_A{ 1 };
  while (std::getline(file, line) && line_number < data.r1_position.R1_FOOTER)
    {
      ++line_number;
      // skip repeated header
      if (year == uint16_t{ 1983 } && (line.find("A  EL") != std::string::npos || line.starts_with("1")))
        {
          continue;
        }

      if (line.starts_with("0"))
        {
          current_A = data.getReaction_1_A(line);
        }

      const auto current_Z = data.getReaction_1_Z(line);

      // std::cout << line << std::endl;
      // std::cout << "Running: " << current_A << " | " << current_Z << std::endl;
      if (!parseAMEReactionOneFormat(line, current_A, current_Z))
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
  uint16_t current_A{ 1 };
  while (std::getline(file, line) && line_number < data.r2_position.R2_FOOTER)
    {
      ++line_number;
      // skip repeated header which only happens in the 2020 file (so far)
      if (year == uint16_t{ 1983 } && (line.find("A  EL") != std::string::npos || line.starts_with("1")))
        {
          continue;
        }

      // skip repeated header which only happens in the 2020 file (so far)
      if (year == uint16_t{ 2020 } && line.find("1 A  elt") != std::string::npos)
        {
          continue;
        }

      if (line.starts_with("0"))
        {
          current_A = data.getReaction_2_A(line);
        }

      const auto current_Z = data.getReaction_2_Z(line);

      if (!parseAMEReactionTwoFormat(line, current_A, current_Z))
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

  data.setSpinParity();

  data.setExperimental();

  data.setA();
  data.setZ();
  data.setN();

  // Confirm a valid Z value has been read before trying to get it's symbol
  if (const auto symbol = Converter::ZToSymbol(data.Z); symbol)
    {
      data.setSymbol(symbol.value());
    }
  // Set the symbol as an obviously wrong value if an invalid Z was read
  else
    {
      data.setSymbol("Xx");
    }

  data.setState();

  // For the non ground-state entries we have enough data to attach this level to the appropriate isotope
  if (data.level > 0)
    {
      return data;
    }

  data.setMassExcess();
  data.setMassExcessError();

  data.setHalfLife();

  data.setYear();

  data.setDecayMode();

  if (data.decay == "stable")
    {
      neutron_rich.at(data.Z) = true;
    }

  data.setNeutronOrProtonRich(neutron_rich.at(data.Z));

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
          fullDataTable.emplace_back(*ame, nubase);
        }
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

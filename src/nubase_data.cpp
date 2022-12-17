#include "nuclear-data-reader/nubase_data.hpp"

#include "nuclear-data-reader/nubase_line_position.hpp"
#include <string_view>

#include <fmt/format.h>

#include <algorithm>
#include <cmath>
#include <cstdint>


double NUBASE::Data::getRelativeMassExcessError(const double min_allowed) const
{
  // 12C has an mass excess of 0.0 by definition.
  // This is the only place it currently trips us up and this path does not always happen so, in terms of doing as
  // little work as possible, this is the best place to check and make adjustments.
  // The value we set does not matter as the error on the value is also 0.0 so relative error is guaranteed to be 0.0
  if (A == 12 && Z == 6)
    {
      mass_excess = 0.0001;
    }

  return std::max(std::fabs(dmass_excess / mass_excess), min_allowed);
}


std::string NUBASE::Data::cleanSpinParityString(std::string& spin_parity) const
{
  // HACKS for those nuclei with non-unique assignments.
  // Remove 'unhelpful' characters
  if (const auto unwanted = spin_parity.find_first_of("<>*"); unwanted != std::string::npos)
    {
      spin_parity.erase(unwanted, 1);
    }
  // 131I isomer 2 has (19/2+..23/2+) change to (19/2+)
  else if (spin_parity.find("(19/2+..23/2+)") != std::string::npos)
    {
      spin_parity.replace(6, spin_parity.length(), ")");
    }
  // 118Rh has gs J=(4-10), take as 4
  else if (const auto spin_alter = spin_parity.find("(4-10)"); spin_alter != std::string::npos)
    {
      spin_parity.erase(spin_alter + 2, 3);
    }
  // 176Tam has no J value, just (+). Executive decision; tentative 0+
  else if (spin_parity.substr(0, 3) == "(+)")
    {
      spin_parity = "(0+)";
    }
  // 71Se isomer 1 has 1/2- to 9/2- change to 1/2-
  else if (spin_parity.find("1/2-to9/2-") != std::string::npos)
    {
      spin_parity.replace(4, spin_parity.length(), "");
    }
  // 142Ho has (6 to 9) change to (6)
  else if (spin_parity.find("(6to9)") != std::string::npos)
    {
      spin_parity.replace(2, spin_parity.length(), ")");
    }
  // Same string in both 2003 and 2012
  // 42Sc isomer 5 has (1+ to 4+) change to (1+)
  if (spin_parity.find("(1+to4+)") != std::string::npos)
    {
      spin_parity.replace(3, spin_parity.length(), ")");
    }
  // 38P has (0- to 4-) change to (0-)
  if (spin_parity.find("0-to4-") != std::string::npos)
    {
      spin_parity.replace(3, spin_parity.length(), ")");
    }
  // 118Agm has 0(-) to 2(-) change to 0(-)
  else if (spin_parity.find("0(-)to2(-)") != std::string::npos)
    {
      spin_parity.erase(4);
    }
  // 35Sxi has (1/2:9/2)+ change to (1/2)+
  else if (spin_parity.find("(1/2:9/2)+") != std::string::npos)
    {
      spin_parity.replace(4, spin_parity.length(), ")+");
    }
  // 2016
  // Nothing additional to what has already been done
  // 2020
  // Nothing additional to what has already been done

  return spin_parity;
}


void NUBASE::Data::setSpinParity() const
{
  // The first value of spin and/or parity will be taken

  // The information starts at character position.START_SPIN(79),
  // so if the line is not at least that length, set values to 'unknown' and get out.
  // OR
  // The isotope can have no spin/parity, but a decay method, in which case we
  // pass the above condition but there is no need to do any processing.
  // Set all properties to default and get out.
  if (full_data.size() <= position.START_SPIN || full_data.at(position.START_SPIN) == ' ')
    {
      setAllSpinParityValuesAsUnknown();
      return;
    }

  // Extract full assignment into a separated string and remove parts as we assign properties
  std::string jpi = full_data.substr(position.START_SPIN, (position.END_SPIN - position.START_SPIN));
  // Assume we have measured the spin and parity
  // We'll alter these if it's not true
  J_exp  = Measured::EXPERIMENTAL;
  pi_exp = Measured::EXPERIMENTAL;

  // Some values are set as words (high, low, mix, spmix, fsmix, am)
  // Don't want this so set all properties to default and get out.
  if (std::isalpha(jpi.at(0)) != 0)
    {
      setAllSpinParityValuesAsUnknown();
      return;
    }

  // Remove more random substrings that we aren't going to parse/use
  constexpr std::array<std::string_view, 3> randoms{ "frg", "T=", "am" };
  if (const auto str_pos =
          std::find_if(randoms.cbegin(),
                       randoms.cend(),
                       [=](const auto character) { return jpi.find(character) != std::string_view::npos; });
      str_pos != randoms.cend())
    {
      jpi.erase(jpi.find(*str_pos));
    }

  // Remove all white space
  jpi.erase(std::remove_if(jpi.begin(), jpi.end(), [](const auto character) { return std::isspace(character); }),
            jpi.end());

  // Convert overly complicated assignments into more parseable ones
  jpi = cleanSpinParityString(jpi);

  // Brackets mean tentative values and hash (#) means theoretical
  // FIXME: It's likely that any and all # characters has been removed by the time this function is run.
  // For the moment, we will call both of those theoretical.
  // If there is an opening ( then there will be also be a closing ) so don't bother checking
  if (jpi.find('(') != std::string::npos || jpi.find('#') != std::string::npos)
    {
      std::erase_if(jpi, [](const auto character) { return character == '(' || character == ')' || character == '#'; });
      pi_exp = Measured::THEORETICAL;
      J_exp  = Measured::THEORETICAL;
    }

  // We are only storing one value per isotope.
  // For those with multiple values, separated by a comma, drop everything after the first comma
  if (const auto pos = jpi.find(','); pos != std::string::npos)
    {
      jpi.resize(pos);
    }

  // What is the parity of the state
  if (jpi.ends_with('-') || jpi.ends_with('+'))
    {
      pi = jpi.ends_with('-') ? Parity::NEGATIVE : Parity::POSITIVE;
      jpi.pop_back();
    }
  // If no parity symbol, set to 'unknown'
  else
    {
      pi = Parity::UNKNOWN;
    }

  // Our string should now only be a number. Either x or x/2.
  J = (jpi.ends_with("/2")) ? 0.5 * std::stod(jpi.substr(0, jpi.find('/'))) : std::stod(jpi);
}


void NUBASE::Data::setExperimental() const
{
  // Will use mass excess for the measured, or not, criteria, the last digit is char position.END_DME
  // so if there is a '#' but it's after this we will still say experimental
  const auto measured = full_data.find_first_of('#');

  if (measured != std::string::npos)
    {
      std::replace(full_data.begin(), full_data.end(), '#', ' ');
    }

  exp = (measured > position.END_DME) ? NUBASE::Measured::EXPERIMENTAL : NUBASE::Measured::THEORETICAL;
}


void NUBASE::Data::setIsomerData(std::vector<NUBASE::Data>& nuc) const
{
  // Loop backwards through the existing isotopes to look for the correct ground state
  // Original order is ground state followed by ascending states,
  // theoretically we could just modify nuc.back(), but that's not safe
  if (auto ground_state = std::find_if(
          nuc.rbegin(), nuc.rend(), [this](const auto& isotope) { return A == isotope.A && Z == isotope.Z; });
      ground_state != nuc.rend())
    {
      const auto energy = setIsomerEnergy();
      const auto error  = setIsomerEnergyError();

      // Some isomers(3 in total) are measured via beta difference so come out -ve
      ground_state->energy_levels.emplace_back(level, energy < 0.0 ? energy : std::fabs(energy), error);
    }
  else
    {
      // Should never get here
      fmt::print(stderr, "**WARNING**: This isomer has no matching ground-state\n");
    }
}


void NUBASE::Data::setHalfLife() const
{
  // Annoying data file format strikes again
  // Line length is not always as long as the half life position
  // Create a temporary string with either the half life or a known value
  const std::string noUnit{ "no_units" };

  std::string lifetime =
      (full_data.size() + 1 < position.START_HALFLIFEVALUE)
          ? noUnit
          : full_data.substr(position.START_HALFLIFEVALUE, (position.END_HALFLIFEVALUE - position.START_HALFLIFEVALUE));

  // If there is no unit on the half life, or the string contains certain characters, we shouldn't bother trying to
  // parse the values. Set as a very short half life and get out
  if (lifetime == noUnit || lifetime.find_first_not_of(' ') == std::string::npos
      || lifetime.find("p-unst") != std::string::npos || lifetime.find('R') != std::string::npos)
    {
      hl       = Converter::seconds{ 1.0e-24 };
      hl_error = Converter::seconds{ 1.0 };
      return;
    }

  // If stable set to very long and get out
  if (lifetime.find("stbl") != std::string::npos)
    {
      hl       = Converter::seconds{ 1.0e24 };
      hl_error = Converter::seconds{ 1.0 };
      return;
    }

  // Not currently interested in approximations or limits
  std::string_view remove{ "<>~" };
  std::transform(lifetime.begin(), lifetime.end(), lifetime.begin(), [remove](const auto character) {
    return remove.find(character) != std::string::npos ? ' ' : character;
  });

  // Get the numerical part of the half life that we can use to create a chrono value later
  const auto hl_double =
      Converter::StringToNum<double>(lifetime, 0, position.END_HALFLIFEVALUE - position.START_HALFLIFEVALUE);

  const auto hl_error_double = getNumericalHalfLifeError();

  setHalfLifeUnit();

  hl       = Converter::ToDuration(hl_double, halflife_unit);
  hl_error = Converter::ToDuration(hl_error_double, halflife_unit);
}


void NUBASE::Data::setDecayMode() const
{
  // Create a std::string we can play with and modify in this function
  auto Decay =
      (full_data.size() >= position.START_DECAYSTRING) ? full_data.substr(position.START_DECAYSTRING) : "isomer?";

  // The string format is ... complicated, see Section 2.5 of the 2016 paper
  // 10.1088/1674-1137/41/3/030001
  // Let's be relatively simple and take up to the first ';' only
  if (Decay.find(';') != std::string::npos)
    {
      Decay.erase(Decay.find(';'));
    }

  // Chop out everything after the '='
  if (Decay.find('=') != std::string::npos)
    {
      Decay.erase(Decay.find('='));
    }
  // Or convert a guess/estimate to unknown
  else if (Decay.find(" ?") != std::string::npos)
    {
      Decay = "unknown";
    }

  // Remove from remaining unwanted characters to end
  const auto found = Decay.find_first_of("~<> ");

  if (found != std::string::npos)
    {
      Decay.erase(found);
    }

  // Book keeping
  // swap e+ for B+
  if (Decay == "e+")
    {
      Decay = "B+";
    }
  // use "stable" instead of "IS"
  else if (Decay == "IS")
    {
      Decay = "stable";
    }

  decay = Decay;
}


void NUBASE::Data::setNeutronOrProtonRich(const bool pnSide) const noexcept
{
  rich = (!pnSide)             ? NUBASE::Richness::PROTON
         : (decay == "stable") ? NUBASE::Richness::STABLE
                               : NUBASE::Richness::NEUTRON;

  // Tc(43) and Pm(61) have no stable isotopes so set the 'stable' point by hand
  switch (Z)
    {
      case 43:
        rich = (A <= 96) ? NUBASE::Richness::PROTON : NUBASE::Richness::NEUTRON;
        break;
      case 61:
        rich = (A <= 144) ? NUBASE::Richness::PROTON : NUBASE::Richness::NEUTRON;
        break;
    }
}

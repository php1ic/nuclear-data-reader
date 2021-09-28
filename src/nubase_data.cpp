#include "nuclear-data-reader/nubase_data.hpp"

#include "nuclear-data-reader/nubase_line_position.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <cmath>
#include <cstdint>


double NUBASE::Data::getRelativeMassExcessError(const double min_allowed) const
{
  // 12C has an mass excess of 0.0 by definition.
  // This is the only place it currently trips us up and this path does not always happen so, in terms of doing as
  // little work as possible, this is the best place to check and make adjustments.
  // The value we set does not matter as the error on the value is also 0.0 so relative error is guarenteed to be 0.0
  if (A == 12 && Z == 6)
    {
      mass_excess = 0.0001;
    }

  return (fabs(dmass_excess / mass_excess) < min_allowed) ? min_allowed : fabs(dmass_excess / mass_excess);
}


void NUBASE::Data::setSpinParity() const
{
  // This is dirty.

  // Do this prior to replacing all '#' with ' '
  // As a general rule, the first value of spin and/or parity will be taken

  // The information starts at character position.START_SPIN(79), so if the line is not at least that
  // size set values to 'unknown' and get out.
  // OR
  // The isotope can have no spin/parity, but a decay method, in which case we
  // pass the above condition but there is no need to do any processing. Set
  // the values to unknown and get out.
  if (full_data.size() <= position.START_SPIN || full_data.at(position.START_SPIN) == ' ')
    {
      J  = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Easiest to extract values by stripping away bits after use
  std::string jpi = full_data.substr(position.START_SPIN, (position.END_SPIN - position.START_SPIN));

  // Some values are set as words (high, low, mix, spmix, fsmix, am)
  // Don't want this so set to 'unknown' and get out.
  if (isalpha(jpi[0]) != 0)
    {
      J  = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // trim trailing spaces
  size_t endpos = jpi.find_last_not_of(' ');
  if (std::string::npos != endpos)
    {
      jpi = jpi.substr(0, endpos + 1);
    }

  // HACKS for those nuclei with non-unique assignments.
  //
  // 42Sc isomer 5 has (1+ to 4+) change to (1+)
  if (jpi.find("(1+ to 4+)") != std::string::npos)
    {
      jpi.replace(3, jpi.length(), ")");
    }
  // 71Se isomer 1 has 1/2- to 9/2- change to 9/2-
  if (jpi.find("1/2- to 9/2-") != std::string::npos)
    {
      jpi.replace(4, jpi.length(), " ");
    }
  // 142Ho has (6 to 9) change to (6)
  if (jpi.find("(6 to 9)") != std::string::npos)
    {
      jpi.replace(2, jpi.length(), ")");
    }
  // 131I isomer 2 has (19/2+..23/2+) change to (19/2+)
  if (jpi.find("(19/2+..23/2+)") != std::string::npos)
    {
      jpi.replace(6, jpi.length(), ")");
    }
  // 118Rh has gs J=4-10, take as 4
  if (jpi.find("(4-10)") != std::string::npos)
    {
      jpi.erase(jpi.find("(4-10)") + 1, 3);
    }
  // Remove 'unhelpful' characters
  if (jpi.find('>') != std::string::npos)
    {
      jpi.erase(jpi.find('>'), 1);
    }
  if (jpi.find('<') != std::string::npos)
    {
      jpi.erase(jpi.find('<'), 1);
    }
  // 176Tam has no J value, just (+). Easiest to say both are unknown
  if (jpi.substr(0, 3) == "(+)")
    {
      jpi = "?";
    }

  // If no parity is in the copied section then there is no assignment
  // Set to 'unknown' and get out.
  if (jpi.find('+') == std::string::npos && jpi.find('-') == std::string::npos)
    {
      J  = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Take only the first parity if two are specified for the same state
  if (jpi.find('+') != std::string::npos && jpi.find('-') != std::string::npos)
    {
      if (jpi.find_first_of('+') > jpi.find_first_of('-'))
        {
          jpi.erase(jpi.find('+'), 1);
        }
      else
        {
          jpi.erase(jpi.find('-'), 1);
        }
    }

  // Member pi is the parity (0)+ve or (1)-ve
  // Member pi_exp is the state experimental(0) or theory/extrapolated(1)
  // We will remove the sign once we record it.
  if (jpi.find('+') != std::string::npos)
    {
      pi = 0;
      bool experimental{ false };
      do
        {
          if (jpi.size() > (jpi.find('+') + 1) && jpi.at(jpi.find('+') + 1) == '#')
            {
              jpi.erase(jpi.find('+'), 2);
            }
          else
            {
              jpi.erase(jpi.find('+'), 1);
              experimental = true;
            }
        }
      while (jpi.find('+') != std::string::npos);

      pi_exp = experimental ? 0 : 1;
    }
  else if (jpi.find('-') != std::string::npos)
    {
      pi = 1;
      bool experimental{ false };
      do
        {
          if (jpi.size() > (jpi.find('-') + 1) && jpi.at(jpi.find('-') + 1) == '#')
            {
              jpi.erase(jpi.find('-'), 2);
            }
          else
            {
              jpi.erase(jpi.find('-'), 1);
              experimental = true;
            }
        }
      while (jpi.find('-') != std::string::npos);

      pi_exp = experimental ? 0 : 1;
    }

  // Stripping away the +/- will leave some () so remove them
  if (jpi.find("()") != std::string::npos)
    {
      jpi.erase(jpi.find("()"), 2);
    }

  // Member J_tent shows either definite(0) or tentative(1) assignment
  if (jpi.find('(') != std::string::npos)
    {
      jpi.erase(jpi.find('('), 1);
      jpi.erase(jpi.find(')'), 1);
      J_tent = 1;
    }
  else
    {
      J_tent = 0;
    }

  // If multiple spins are given, take only the first
  if (jpi.find(',') != std::string::npos)
    {
      jpi.erase(jpi.find(','));
    }

  // Member J_exp either experiment(0) or theory/extrapolated(1) assigment
  if (jpi.find('#') != std::string::npos)
    {
      jpi.erase(jpi.find('#'), 1);
      J_exp = 1;
    }
  else
    {
      J_exp = 0;
    }

  // Member J stores the spin as a double
  if (jpi.find('/') == std::string::npos)
    {
      J = Converter::StringToNum<double>(jpi, 0, jpi.length());
    }
  else
    {
      J = 0.5 * Converter::StringToNum<double>(jpi, 0, jpi.find('/'));
    }
}


void NUBASE::Data::setExperimental() const
{
  // Member exp has false(experiment) or true(theory/extrapolation) value
  // Will use mass excess for criteria, the last digit is char position.END_DME (38)
  // so if there is a '#' but it's after this we will still say experimental
  const auto measured = full_data.find_first_of('#');

  if (measured != std::string::npos)
    {
      std::replace(full_data.begin(), full_data.end(), '#', ' ');
    }

  exp = (measured > position.END_DME) ? 1 : 0;
}


void NUBASE::Data::setIsomerData(std::vector<NUBASE::Data>& nuc) const
{
  // Loop from the penultimate isotope towards the beginning.
  // Original order is ground state followed by ascending states,
  // theoretically we could just modify nuc.back(), but that's not safe
  for (auto previous = nuc.rbegin(); previous != nuc.rend(); ++previous)
    {
      if (A == previous->A && Z == previous->Z)
        {
          const auto energy = setIsomerEnergy();
          const auto error  = setIsomerEnergyError();

          // Some isomers(3 in total) are measured via beta difference so come out -ve
          previous->energy_levels.emplace_back(State(level, energy < 0.0 ? energy : std::fabs(energy), error));
          return;
        }
    }

  // Should never get here
  fmt::print(stderr, "**WARNING**: This isomer has no matching ground-state\n");
}


void NUBASE::Data::setHalfLife() const
{
  // Annoying data file format strikes again
  // Line length is not always as long as the half life position
  // Create a temporary string with either the half life or a know value
  std::string lifetime =
      (full_data.size() < static_cast<uint8_t>(position.START_HALFLIFEVALUE - 1))
          ? noUnit()
          : full_data.substr(position.START_HALFLIFEVALUE, (position.END_HALFLIFEVALUE - position.START_HALFLIFEVALUE));

  // Certain string mean we should not try and parse them as half lives
  // If they are found, convert to our know value
  if (lifetime.find_first_not_of(' ') == std::string::npos || lifetime.find("p-unst") != std::string::npos
      || lifetime.find('R') != std::string::npos)
    {
      lifetime = noUnit();
    }

  // Not currently interested in approximations or limits
  std::string_view remove{ "<>~" };
  std::transform(lifetime.begin(), lifetime.end(), lifetime.begin(), [&remove](const char c) {
    return remove.find(c) != std::string::npos ? ' ' : c;
  });

  // If noUnits assume unknown so very short half life
  if (lifetime == noUnit())
    {
      hl       = Converter::seconds{ 1.0e-24 };
      hl_error = Converter::seconds{ 1.0 };
    }
  // If stable set to very long
  else if (lifetime.find("stbl") != std::string::npos)
    {
      hl       = Converter::seconds{ 1.0e24 };
      hl_error = Converter::seconds{ 1.0 };
    }
  else
    {
      const auto hl_double =
          Converter::StringToNum<double>(lifetime, 0, position.END_HALFLIFEVALUE - position.START_HALFLIFEVALUE);

      // FIXME: Formatting is not consitent, extracting the error should be refactored into it's own method
      auto hle =
          full_data.substr(position.START_HALFLIFEERROR, (position.END_HALFLIFEERROR - position.START_HALFLIFEERROR));
      std::replace(hle.begin(), hle.end(), '>', ' ');
      std::replace(hle.begin(), hle.end(), '<', ' ');
      const auto hl_error_double = Converter::StringToNum<double>(hle, 0, hle.size());

      setHalfLifeUnit();

      if (halflife_unit.find_first_not_of(' ') == std::string::npos)
        {
          halflife_unit = "ys";
        }

      if (halflife_unit == "ys")
        {
          hl       = Converter::attoseconds{ 1.0e-6 * hl_double };
          hl_error = Converter::attoseconds{ 1.0e-6 * hl_error_double };
        }
      else if (halflife_unit == "zs")
        {
          hl       = Converter::attoseconds{ 1.0e-3 * hl_double };
          hl_error = Converter::attoseconds{ 1.0e-3 * hl_error_double };
        }
      else if (halflife_unit == "as")
        {
          hl       = Converter::attoseconds{ hl_double };
          hl_error = Converter::attoseconds{ hl_error_double };
        }
      else if (halflife_unit == "ps")
        {
          hl       = Converter::picoseconds{ hl_double };
          hl_error = Converter::picoseconds{ hl_error_double };
        }
      else if (halflife_unit == "ns")
        {
          hl       = Converter::nanoseconds{ hl_double };
          hl_error = Converter::nanoseconds{ hl_error_double };
        }
      else if (halflife_unit == "us")
        {
          hl       = Converter::microseconds{ hl_double };
          hl_error = Converter::microseconds{ hl_error_double };
        }
      else if (halflife_unit == "ms")
        {
          hl       = Converter::milliseconds{ hl_double };
          hl_error = Converter::milliseconds{ hl_error_double };
        }
      else if (halflife_unit == "s")
        {
          hl       = Converter::seconds{ hl_double };
          hl_error = Converter::seconds{ hl_error_double };
        }
      else if (halflife_unit == "m")
        {
          hl       = Converter::minutes{ hl_double };
          hl_error = Converter::minutes{ hl_error_double };
        }
      else if (halflife_unit == "h")
        {
          hl       = Converter::hours{ hl_double };
          hl_error = Converter::hours{ hl_error_double };
        }
      else if (halflife_unit == "d")
        {
          hl       = Converter::days{ hl_double };
          hl_error = Converter::days{ hl_error_double };
        }
      else if (halflife_unit == "y")
        {
          hl       = Converter::years{ hl_double };
          hl_error = Converter::years{ hl_error_double };
        }
      else if (halflife_unit == "ky")
        {
          hl       = Converter::kiloyears{ hl_double };
          hl_error = Converter::kiloyears{ hl_error_double };
        }
      else if (halflife_unit == "My")
        {
          hl       = Converter::millionyears{ hl_double };
          hl_error = Converter::millionyears{ hl_error_double };
        }
      else if (halflife_unit == "Gy")
        {
          hl       = Converter::billionyears{ hl_double };
          hl_error = Converter::billionyears{ hl_error_double };
        }
      else if (halflife_unit == "Ty")
        {
          hl       = Converter::billionyears{ 1.0e3 * hl_double };
          hl_error = Converter::billionyears{ 1.0e3 * hl_error_double };
        }
      else if (halflife_unit == "Py")
        {
          hl       = Converter::billionyears{ 1.0e6 * hl_double };
          hl_error = Converter::billionyears{ 1.0e6 * hl_error_double };
        }
      else if (halflife_unit == "Ey")
        {
          hl       = Converter::billionyears{ 1.0e9 * hl_double };
          hl_error = Converter::billionyears{ 1.0e9 * hl_error_double };
        }
      else if (halflife_unit == "Zy")
        {
          hl       = Converter::billionyears{ 1.0e12 * hl_double };
          hl_error = Converter::billionyears{ 1.0e12 * hl_error_double };
        }
      else if (halflife_unit == "Yy")
        {
          hl       = Converter::billionyears{ 1.0e15 * hl_double };
          hl_error = Converter::billionyears{ 1.0e15 * hl_error_double };
        }
    }
}


void NUBASE::Data::setDecayMode() const
{
  // Store how ground-state decays in member decay
  std::string Decay{ "isomer?" };

  const auto startCharacter = position.START_DECAYSTRING;

  if (full_data.size() >= startCharacter)
    {
      Decay = full_data.substr(startCharacter);
    }

  // The string format is ... complicated, see Section 2.5 of the 2016 paper
  // 10.1088/1674-1137/41/3/030001
  // Let's be relatively simple and take upto the first ';' only
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

/**
 *
 * \class NUBASEData
 *
 * \brief Parse and store all of the NUBASE data
 *
 *
 */
#ifndef NUBASEDATA_HPP
#define NUBASEDATA_HPP

#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/nubase_line_position.hpp"
#include <string_view>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>


namespace NUBASE
{
  enum class Richness : uint8_t
  {
    PROTON  = 0,
    NEUTRON = 1,
    STABLE  = 2
  };

  enum class Measured : uint8_t
  {
    EXPERIMENTAL = 0,
    THEORETICAL  = 1
  };

  class Data
  {
  public:
    Data(std::string line, const uint16_t _year) : position(_year), full_data(std::move(line)) {}

    Data(const Data&)     = default;
    Data(Data&&) noexcept = default;

    Data& operator=(const Data&) = default;
    Data& operator=(Data&&) noexcept = default;

    ~Data() = default;

    /// Where are the variables located on the line in the file
    mutable LinePosition position;

    /// Is the isotope experimental or extrapolated/theoretical
    mutable Measured exp{ Measured::EXPERIMENTAL };
    /// The mass number
    mutable uint16_t A{ 0 };
    /// The proton number
    mutable uint8_t Z{ 0 };
    /// The neutron number
    mutable uint8_t N{ 0 };
    /// The state level
    mutable uint8_t level{ 0 };
    /// The parity of the spin state
    mutable uint8_t pi{ 0 };
    /// Is the parity of the spin state experimental
    mutable uint8_t pi_exp{ 0 };
    /// The spin value of the state
    mutable uint8_t J_exp{ 0 };
    /// Is the spin value experimental
    mutable uint8_t J_tent{ 0 };
    /// The discovery year to use if non is given
    mutable uint16_t DEFAULT_YEAR{ 1900 };
    /// What year was the isotope discovered
    mutable uint16_t year{ DEFAULT_YEAR };

    /// Mass excess from the NUBASE table
    mutable double mass_excess{ 0.1 };
    /// Error on the mass excess from the NUBASE table
    mutable double dmass_excess{ 1.0e4 };
    /// Spin parity of the isotope
    mutable double J{ 0.0 };

    /// Half life of the isotope
    mutable std::chrono::duration<double> hl{};
    /// Error on the half life of the isotope
    mutable std::chrono::duration<double> hl_error{};

    /// Human readable unit for half life
    mutable std::string halflife_unit{};
    /// Isotopic symbol
    mutable std::string symbol{};
    /// Decay mode of the isotope
    mutable std::string decay{};
    /// The entire line for the isotope from the data file
    mutable std::string full_data{};

    /// Is the isotope neutron or proton rich
    /// (defined by which 'side' of stability it is on, not N=Z line)
    mutable Richness rich{ Richness::STABLE };

    /**
     * \struct State
     *
     * \brief Details additional energy levels of the isotope
     */
    struct State
    {
      State(const uint8_t _level, const double _energy, const double _error) :
          level(_level), energy(_energy), error(_error)
      {
      }

      uint8_t level{ 0 };
      double energy{ 0.0 };
      double error{ 0.0 };
    };
    /// Container for all levels that are recorded in the data file that is read
    mutable std::vector<State> energy_levels;

    /**
     * Set the neutron number
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setN() const noexcept { N = A - Z; }

    /**
     * Extract the mass number from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setA() const { A = Converter::StringToNum<uint16_t>(full_data, position.START_A, position.END_A); }

    /**
     * Extract the proton number from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setZ() const { Z = Converter::StringToNum<uint8_t>(full_data, position.START_Z, position.END_Z); }

    /**
     * Extract the mass-excess from the NUBASE data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setMassExcess() const
    {
      mass_excess = Converter::StringToNum<double>(full_data, position.START_ME, position.END_ME);
    }

    /**
     * Extract the error on mass-excess from the NUBASE data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setMassExcessError() const
    {
      dmass_excess = Converter::StringToNum<double>(full_data, position.START_DME, position.END_DME);
    }

    /**
     * Calculate the relative error on the isotope, but use a low water mark of <min_allowed>
     *
     * \param min_allowed The value to not let the relative error fall below
     *
     * \return The relative error on the isotope or min_allowed, whichever is larger
     */
    [[nodiscard]] double getRelativeMassExcessError(const double min_allowed) const;

    /**
     * Extract the discovery year from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setYear() const
    {
      year = (position.START_YEAR == 0)
                 ? DEFAULT_YEAR
                 : Converter::StringToNum<uint16_t>(full_data, position.START_YEAR, position.END_YEAR);

      // Some isotopes have no value for the year so we need to watch for that.
      // Set it as the default if no year is given
      if (year == std::numeric_limits<uint16_t>::max())
        {
          year = DEFAULT_YEAR;
        }
    }

    /**
     * Extract the units of the half life value from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setHalfLifeUnit() const
    {
      halflife_unit =
          full_data.substr(position.START_HALFLIFEUNIT, position.END_HALFLIFEUNIT - position.START_HALFLIFEUNIT);

      // Trim leading white space
      halflife_unit.erase(halflife_unit.begin(),
                          std::find_if(halflife_unit.begin(), halflife_unit.end(), [](const auto ch) {
                            return (std::isspace(ch) == 0);
                          }));
    }


    /**
     * Extract the numerical value of the error on the half life
     *
     * \param Nothing
     *
     * \return The error on the half life
     */
    inline double getNumericalHalfLifeError() const
    {
      auto hle =
          full_data.substr(position.START_HALFLIFEERROR, (position.END_HALFLIFEERROR - position.START_HALFLIFEERROR));
      std::replace(hle.begin(), hle.end(), '>', ' ');
      std::replace(hle.begin(), hle.end(), '<', ' ');
      return Converter::StringToNum<double>(hle, 0, hle.size());
    }

    /**
     * Set the isotope symbol
     *
     * \param The std::string to use as the symbol
     *
     * \return Nothing
     */
    inline void setSymbol(std::string_view _symbol) const noexcept { symbol = _symbol; }

    /**
     * Modify the line from the data file swapping '#' for ' ' to maintain positioning.
     * Also set the exp flag as required
     *
     * \param Nothing
     *
     * \return Nothing
     */
    void setExperimental() const;

    /**
     * Set the experimental flag
     *
     * \param The value to set the experimental flag
     *
     * \return Nothing
     */
    inline void setExperimental(const NUBASE::Measured val) const noexcept { exp = val; }

    /**
     * Extract the state/level from the data file
     *
     * \param The value to be set (N.B. pass by reference)
     *
     * \return Nothing
     */
    inline void setState() const
    {
      level = Converter::StringToNum<uint8_t>(full_data, position.START_STATE, position.END_STATE);
    }

    /**
     * Extract the energy of the level from the data file
     *
     * \param The value to be set (N.B. pass by reference)
     *
     * \return Nothing
     */
    [[nodiscard]] inline double setIsomerEnergy() const
    {
      return Converter::StringToNum<double>(full_data, position.START_ISOMER, position.END_ISOMER);
    }

    /**
     * Extract the error on the energy of the state
     *
     * \param The value to be set (N.B. pass by reference)
     *
     * \return Nothing
     */
    [[nodiscard]] inline double setIsomerEnergyError() const
    {
      return Converter::StringToNum<double>(full_data, position.START_DISOMER, position.END_DISOMER);
    }

    /**
     * Extract the spin and parity from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    void setSpinParity() const;

    /**
     * Extract the isomeric data; level, energy and error on energy
     *
     * \param The data read so far to allow us to populate the necessary state
     * \param The state we are getting information about
     *
     * \return Nothing
     */
    void setIsomerData(std::vector<NUBASE::Data>& nuc) const;

    /**
     * Extract the half life from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    void setHalfLife() const;

    /**
     * Extract the decay mode from the data file
     *
     * \param The year of the data file being read
     *
     * \return Nothing
     */
    void setDecayMode() const;

    /**
     * Set the rich flag for the isotope
     *
     * \param Container that keeps track of if we have read a stable isotope for this Z value
     *
     * \return Nothing
     */
    void setNeutronOrProtonRich(const bool pnSide) const noexcept;
  };
} // namespace NUBASE

#endif // NUBASEDATA_HPP

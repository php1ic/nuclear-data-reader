/**
 *
 * \class Isotope
 *
 * \brief Storage class for a single isotope
 *
 * There is a lot of data for each isotope, use this class to store, set and get
 * values associated with it
 */
#ifndef ISOTOPE_HPP
#define ISOTOPE_HPP

#include "nuclear-data-reader/ame_data.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <cstdint>
#include <string>
#include <utility>


class Isotope
{
public:
  Isotope(AME::Data _ame, NUBASE::Data _nubase) : ame(std::move(_ame)), nubase(std::move(_nubase)) {}

  Isotope(const Isotope&)     = default;
  Isotope(Isotope&&) noexcept = default;

  Isotope& operator=(const Isotope&)     = default;
  Isotope& operator=(Isotope&&) noexcept = default;

  ~Isotope() = default;

  // What decimal precision should be used if the data is printed to file
  // The AME files use 4dp so lets default to that
  static constexpr uint8_t NDP{ 4 };

  // All of the AME data
  mutable AME::Data ame;
  // All of the NUBASE data
  mutable NUBASE::Data nubase;

  /**
   * Output all of the data as a csv string
   *
   * \param Nothing
   *
   * \return All of the members in csv format
   */
  [[nodiscard]] std::string writeAsCSV() const;

  /**
   * Output all of the data as a json string
   *
   * \param A boolean flag to set if new lines are used within a json unit to make it human readable
   *
   * \return All of the members in the format of a json unit
   */
  [[nodiscard]] std::string writeAsJSON(const bool human_readable = true) const;

  /**
   * Create the header line to be used when writing as a csv
   *
   * \param Nothing
   *
   * \return The variables names in csv formatted string
   */
  [[nodiscard]] static std::string writeCSVHeader()
  {
    return std::string("A,"
                       "Z,"
                       "N,"
                       "Symbol,"
                       "Decay,"
                       "Experimental,"
                       "NubaseMassExcess,"
                       "ErrorNubaseMassExcess,"
                       "AMEMassExcess,"
                       "ErrorAMEMassExcess,"
                       "HalfLife,"
                       "SingleNeutronSeparationEnergy,"
                       "ErrorSingleNeutronSeparationEnergy,"
                       "SingleProtonSeparationEnergy,"
                       "ErrorSingleProtonSeparationEnergy,"
                       "DoubleNeutronSeparationEnergy,"
                       "ErrorDoubleNeutronSeparationEnergy,"
                       "DoubleProtonSeparationEnergy,"
                       "ErrorDoubleProtonSeparationEnergy,"
                       "BindingEnergyPerA,"
                       "ErrorBindingEnergyPerA,"
                       "AtomicMass,"
                       "ErrorAtomicMass,"
                       "BetaDecayEnergy,"
                       "ErrorBetaDecayEnergy,"
                       "QAlpha,"
                       "ErrorQAlpha,"
                       "Q2B-,"
                       "ErrorQ2B-,"
                       "Qepsilon_p,"
                       "ErrorQepsilon_p,"
                       "QB-n,"
                       "ErrorQB-n,"
                       "Q4B-,"
                       "ErrorQ4B-,"
                       "QdAlpha,"
                       "ErrorQdAlpha,"
                       "QpAlpha,"
                       "ErrorQpAlpha,"
                       "QnAlpha,"
                       "ErrorQnAlpha,"
                       "Year");
  }
};

#endif // ISOTOPE_HPP

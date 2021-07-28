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
#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/nubase_data.hpp"
#include "nuclear-data-reader/nubase_line_position.hpp"
#include <string_view>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

class Isotope
{
public:
  Isotope(AME::Data _ame, NUBASE::Data _nubase) : ame(std::move(_ame)), nubase(std::move(_nubase)) {}

  Isotope(const Isotope&) = default;
  Isotope(Isotope&&)      = default;

  // Delete due to const member
  Isotope& operator=(const Isotope&) = delete;
  Isotope& operator=(Isotope&&) = delete;

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
   * \param The string to separate values by [default = ","]
   *
   * \return All of the members separated by the given separator
   */
  //[[nodiscard]] std::string writeAsCSV(std::string_view sep = ",") const;

  /**
   * Output the header describing the string returned from Isotope::writeAsCSV()
   *
   * \param Nothing
   *
   * \return A descriptive string of all member variables
   */
  //[[nodiscard]] static std::string CSVHeader();

  /**
   * Output all of the data as a json string
   *
   * \param Nothing
   *
   * \return All of the members in the format of a json ... unit
   */
  [[nodiscard]] std::string writeAsJSON(const bool human_readable = true) const;
};

#endif // ISOTOPE_HPP

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
   * Output all of the data as a json string
   *
   * \param Nothing
   *
   * \return All of the members in the format of a json ... unit
   */
  [[nodiscard]] std::string writeAsJSON(const bool human_readable = true) const;
};

#endif // ISOTOPE_HPP

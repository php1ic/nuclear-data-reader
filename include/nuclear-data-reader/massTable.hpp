/**
 *
 * \class MassTable
 *
 * \brief Read the mass table
 *
 * Read and store the details from the necessary mass table file
 */
#ifndef MASSTABLE_HPP
#define MASSTABLE_HPP

#include "nuclear-data-reader/ame_data.hpp"
#include "nuclear-data-reader/isotope.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>


class MassTable
{
public:
  explicit MassTable(const uint16_t _year) : year(_year)
  {
    neutron_rich.fill(false);

    // If a bad year is given, default to the latest
    if (!ValidYear())
      {
        year = valid_years.back();
      }

    // 1997 NUBASE is the same as 1995 AME (see README)
    // We have decided that all files will live in the 1995 directory,
    // but the user can still select the year 1997 without worry.
    if (year == 1997)
      {
        // fmt::print(stderr, "**WARNING**: The 1997 data is the same as the 1995 data so setting the year to 1995.\n");
        year = 1995;
      }

    line_length = (year < 2020) ? AME::LINE_LENGTH::PRE_2020 : AME::LINE_LENGTH::POST_2020;
  }

  MassTable(const MassTable&)     = default;
  MassTable(MassTable&&) noexcept = default;

  MassTable& operator=(const MassTable&)     = default;
  MassTable& operator=(MassTable&&) noexcept = default;

  ~MassTable() = default;

  /// What years do we have data for
  static constexpr std::array<uint16_t, 8> valid_years{ 1983, 1993, 1995, 1997, 2003, 2012, 2016, 2020 };
  /// Final year of AME only data
  static constexpr uint16_t LAST_YEAR_AME_ONLY{ 1993 };
  /// Which year's table should we read
  mutable uint16_t year{};
  /// We expand the line to ensure it's long enough when reading the 3 AME files
  mutable uint8_t line_length{};

  /// The max Z value of any isotope
  static constexpr uint8_t MAX_Z{ 118 };
  /// Keep track of when we have gone from proton -> neutron rich
  mutable std::array<bool, MAX_Z + 1> neutron_rich{};

  /// The NUBASE table file path
  mutable std::filesystem::path NUBASE_masstable{};
  /// The AME table file path
  mutable std::filesystem::path AME_masstable{};
  /// The AME reaction_1 file path
  mutable std::filesystem::path AME_reaction_1{};
  /// The AME reaction_2 file path
  mutable std::filesystem::path AME_reaction_2{};

  /// Container to store all of the data used to create the file
  mutable std::vector<Isotope> fullDataTable;
  mutable std::vector<NUBASE::Data> nubaseDataTable;
  mutable std::vector<AME::Data> ameDataTable;


  /**
   * Check that the given year corresponds to one that we have a mass table for. If no year
   * is given, use the one currently stored in the `year` member variable
   *
   * \param The year we want to check, or nothing
   *
   * \return[TRUE] A valid year
   * \return[FALSE] An invalid year
   */
  [[nodiscard]] inline bool ValidYear(const std::optional<uint16_t> _year = std::nullopt) const
  {
    return std::find(valid_years.cbegin(), valid_years.cend(), _year ? _year.value() : year) != valid_years.cend();
  }

  /**
   * Set the year of the table who's data will be read
   *
   * \param The year table to use
   *
   * \return[TRUE] A valid year has been requested and thus set
   * \return[FALSE] An invalid year has been requested so no change has been made
   */
  [[nodiscard]] inline auto setTableYear(const uint16_t _year) const noexcept
  {
    if (ValidYear(_year))
      {
        year = _year;
        return true;
      }

    return false;
  }

  /**
   *
   *
   **/
  bool readAME() const;

  /**
   * Combine the data from NUBASE and AME into a single instance by looking for common A & Z values
   *
   * \param How much info to print to screen if thinks don't go perfectly
   *
   * \return[TRUE] Successful merge
   * \return[FALSE] TBD
   */
  [[nodiscard]] bool mergeData() const;

  /**
   * Read a single line of the NUBASE data table for isotopic information
   *
   * \param The line to read
   *
   * \return A populated instance of NUBASE::Data with details from the line that was passed in
   */
  [[nodiscard]] NUBASE::Data parseNUBASEFormat(const std::string& line) const;

  /**
   * Read a single line of the AME data table for isotopic information
   *
   * \param The line to read
   *
   * \return A populated instance of AME::Data
   */
  [[nodiscard]] AME::Data parseAMEMassFormat(const std::string& line) const;

  /**
   * Read a single line of the first AME reaction file (rct1) and update the appropriate instance of the isotope in the
   * mass table
   *
   * \param The line to read
   *
   * \return[TRUE] The line as been read correctly
   * \return[FALSE] The line has not been read correctly
   */
  [[nodiscard]] bool
  parseAMEReactionOneFormat(const std::string& line, const uint16_t table_A, const uint16_t table_Z) const;

  /**
   * Read a single line of the second AME reaction file (rct2) and update the appropriate instance of the isotope in the
   * mass table
   *
   * \param The line to read
   *
   * \return[TRUE] The line as been read correctly
   * \return[FALSE] The line has not been read correctly
   */
  [[nodiscard]] bool
  parseAMEReactionTwoFormat(const std::string& line, const uint16_t table_A, const uint16_t table_Z) const;

  /**
   * Having read the reaction file, find the matching isotope in the mass table so we can update the values
   *
   * \param The line from the reaction file
   * \param Are we looking at the first or second reaction file
   *
   * \return[PASS] The iterator to the appropriate isotope
   * \return[FAIL] The end() iterator
   */
  [[nodiscard]] std::vector<AME::Data>::iterator
  getMatchingIsotope(const std::string& line, const uint16_t table_A, const uint16_t table_Z) const;

  /**
   * Fill the main container with the data that will be used to create the chart
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] bool populateInternalMassTable();

  /**
   * Use the given year to set the absolute file paths of all data files
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setFilePaths() const;

  /**
   * Read the AME datafile for isotopic values
   *
   * \param The absolute path to the data file
   *
   * \return Nothing
   */
  [[nodiscard]] bool readAMEMassFile(const std::filesystem::path& ameTable) const;

  /**
   * Read the the AME reaction data files for values
   *
   * \param The absolute path to the reaction data file
   * \param Which number reaction file are we reading
   *
   * \return[TRUE] The file has been read without issue
   * \return[FALSE] There has been some issue with the reading of the file
   */
  [[nodiscard]] bool readAMEReactionFileOne(const std::filesystem::path& reactionFile) const;
  [[nodiscard]] bool readAMEReactionFileTwo(const std::filesystem::path& reactionFile) const;

  /**
   * Read the NUBASE datafile for isotopic values
   *
   * \param The absolute path to the data file
   *
   * \return Nothing
   */
  [[nodiscard]] bool readNUBASE(const std::filesystem::path& nubaseTable);

  /**
   * Convert singular file format to json
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] bool outputTableToJSON() const;

  /**
   * Convert singular file format to csv
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] bool outputTableToCSV() const;
};

#endif // MASSTABLE_HPP

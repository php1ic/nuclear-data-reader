/**
 *
 * \class MassTable
 *
 * \brief Read the mass table
 *
 * Read and store the deatils from the necessary mass table file
 */
#ifndef MASSTABLE_HPP
#define MASSTABLE_HPP

#include "nuclear-data-reader/ame_data.hpp"
#include "nuclear-data-reader/isotope.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <cstdint>
#include <filesystem>
#include <regex>
#include <vector>

class MassTable
{
public:
  explicit MassTable(const uint16_t _year) : year(_year)
  {
    pnSide.fill(false);

    // If a bad year is given, default to the latest
    if (!checkValidYear())
      {
        year = valid_years.back();
      }

    line_length = (year < 2020) ? 125 : 144;
  }

  MassTable(const MassTable&)     = default;
  MassTable(MassTable&&) noexcept = default;

  /// Delete both due to const members
  MassTable& operator=(const MassTable&) = delete;
  MassTable& operator=(MassTable&&) noexcept = delete;

  ~MassTable() = default;

  /// What years do we have data for
  static constexpr std::array<uint16_t, 4> valid_years{ 2003, 2012, 2016, 2020 };
  /// Which year's table should we read
  mutable uint16_t year{};
  /// We expand the line to ensure it's long enough when reading the 3 AME files
  mutable uint8_t line_length{};

  /// The max Z value of any isotope
  static constexpr uint8_t MAX_Z{ 118 };
  /// Keep track of when we have gone from proton -> neutron rich
  mutable std::array<bool, MAX_Z + 1> pnSide{};

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
   * Find the absolute path to the location of the data files, indepdent of the filesystem.
   *
   * Get the location of this source file
   * This will(should) always be - /some/path/nuclear-data-reader/include/nuclear-data-reader/massTable.hpp
   * Only the directory separator will be different, depending on OS
   * We want to replace "include/nuclear-data-reader/massTable.hpp" with "data/", using the appropriate separator
   *
   * \param Nothing
   *
   * \return The absolute path to the data_files directory
   */
  static const auto& getAbsolutePath()
  {
    static const std::filesystem::path abs_path = std::filesystem::absolute(
        std::regex_replace(__FILE__, std::regex("(include.nuclear-data-reader)(.)(.*)"), "data$2"));

    return abs_path;
  }

  /**
   * Set the year of the table who's data will be read
   *
   * \param The year table to use
   *
   * \return[TRUE] A valid year has been requested and thus set
   * \return[FALSE] An invalid year has been requested so no change has been made
   */
  [[nodiscard]] inline bool setTableYear(const uint16_t _year) const noexcept
  {
    const uint16_t original_year = year;
    year                         = _year;

    if (!checkValidYear())
      {
        year = original_year;
        return false;
      }

    return true;
  }

  /**
   * Check that the given year corresponds to one that we have a mass table for
   *
   * \param The year we wnat to check
   *
   * \return[TRUE] A valid year
   * \return[FALSE] An invalid year
   */
  [[nodiscard]] inline bool checkValidYear(const uint16_t _year) const
  {
    return (std::find(valid_years.cbegin(), valid_years.cend(), _year) != valid_years.end());
  }

  /**
   * Check that the table year that has been set is when a table was actually released.
   *
   * \param Nothing
   *
   * \return[TRUE] A valid year
   * \return[FALSE] An invalid year
   */
  [[nodiscard]] inline bool checkValidYear() const
  {
    return (std::find(valid_years.cbegin(), valid_years.cend(), year) != valid_years.end());
  }

  /**
   * Combine the data from NUBASE and AME into a single instance by looking for common A & Z values
   *
   * \param How much info to print to screen if thinks don't go perfectly
   *
   * \return[TRUE] Successful merge
   * \return[FALSE] TBD
   */
  [[nodiscard]] bool mergeData(const uint8_t verbosity = 0) const;

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
  [[nodiscard]] bool parseAMEReactionOneFormat(const std::string& line) const;

  /**
   * Read a single line of the first AME reaction file (rct2) and update the appropriate instance of the isotope in the
   * mass table
   *
   * \param The line to read
   *
   * \return[TRUE] The line as been read correctly
   * \return[FALSE] The line has not been read correctly
   */
  [[nodiscard]] bool parseAMEReactionTwoFormat(const std::string& line) const;

  /**
   * Having read the reaction file, find the matching isotope in the mass table so we can update the values
   *
   * \param The line from the reaction file
   * \param Are we looking at the first or second reaction file
   *
   * \return[PASS] The iterator to the appropriate isotope
   * \return[FAIL] The end() iterator
   */
  [[nodiscard]] std::vector<AME::Data>::iterator getMatchingIsotope(const std::string& line,
                                                                    const uint8_t reactionFile) const;

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
   * Convert ... singular file format to csv
   *
   * \param Nothing
   *
   * \return Nothing
   */
  // [[nodiscard]] bool outputTableToCSV() const;

  /**
   * Convert ... singular file format to json
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] bool outputTableToJSON() const;
};

#endif // MASSTABLE_HPP

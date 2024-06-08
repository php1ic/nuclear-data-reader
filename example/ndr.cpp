#include <nuclear-data-reader/massTable.hpp>

#include <array>

int main()
{
  std::array<int, 7> years = { 1983, 1993, 1997, 2003, 2012, 2016, 2020 };

  for (const auto year : years)
    {
      auto table = MassTable(year);

      if (table.populateInternalMassTable())
        {
          [[maybe_unused]] auto json_ignore = table.outputTableToJSON();
          [[maybe_unused]] auto csv_ignore  = table.outputTableToCSV();
        }
    }

  return 0;
}

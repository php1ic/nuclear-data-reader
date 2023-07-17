#include <nuclear-data-reader/massTable.hpp>

#include <array>

int main()
{
  std::array<int, 7> years = { 1983, 1993, 1997, 2003, 2012, 2016, 2020 };
  // std::array<int, 1> years = {1995};

  for (const auto year : years)
    {
      auto table = MassTable(year);

      if (table.populateInternalMassTable())
        {
          [[maybe_unused]] auto ignore = table.outputTableToJSON();
        }
    }

  return 0;
}

#include <nuclear-data-reader/massTable.hpp>

#include <array>

int main()
{
  std::array<int, 4> years = { 2003, 2012, 2016, 2020 };

  for (auto year : years)
    {
      auto table = MassTable(year);

      if (table.populateInternalMassTable())
        {
          auto ignore = table.outputTableToJSON();
        }
    }

  return 0;
}

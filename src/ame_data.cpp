#include "nuclear-data-reader/ame_data.hpp"

#include <algorithm>
#include <cmath>

double AME::Data::getRelativeMassExcessError(const double min_allowed) const
{
  // 12C has an mass excess of 0.0 by definition.
  // This is the only place it currently trips us up and this path does not always happen so, in terms of doing as
  // little work as possible, this is the best place to check and make adjustments.
  // The value we set does not matter as the error on the value is also 0.0 so relative error is guaranteed to be 0.0
  if (A == 12 && Z == 6)
    {
      mass_excess.amount = 0.0001;
    }

  const auto rel = mass_excess.relativeUncertainty().value();
  return std::max(rel, min_allowed);
}


void AME::Data::setExperimental() const
{
  // Will use mass excess for the measured, or not, criteria, the last digit is char AME::LinePosition::END_DME
  // so if there is a '#' but it's after this we will still say experimental
  const auto measured = full_data.find_first_of('#');

  if (measured != std::string::npos)
    {
      std::replace(full_data.begin(), full_data.end(), '#', ' ');
    }

  exp = (measured > mass_position.END_DME) ? AME::Measured::EXPERIMENTAL : AME::Measured::THEORETICAL;
}

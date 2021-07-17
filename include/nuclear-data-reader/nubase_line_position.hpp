#ifndef NUBASE_LINE_POSITION_HPP
#define NUBASE_LINE_POSITION_HPP

#include <limits>

namespace NUBASE
{
  struct LinePosition
  {
    explicit LinePosition(const int _year)
    {
      if (_year < 2020)
        {
          HEADER              = 0;
          FOOTER              = std::numeric_limits<int>::max();
          START_A             = 0;
          END_A               = 3;
          START_Z             = 4;
          END_Z               = 7;
          START_STATE         = 7;
          END_STATE           = 8;
          START_ME            = 18;
          END_ME              = 29;
          START_DME           = 29;
          END_DME             = 38;
          START_ISOMER        = 39;
          END_ISOMER          = 46;
          START_DISOMER       = 48;
          END_DISOMER         = 56;
          START_HALFLIFEVALUE = 60;
          END_HALFLIFEVALUE   = 69;
          START_HALFLIFEUNIT  = 69;
          END_HALFLIFEUNIT    = 71;
          START_HALFLIFEERROR = 72;
          END_HALFLIFEERROR   = 77;
          START_SPIN          = 79;
          END_SPIN            = 93;

          // After the 2003 table, the discovery
          // year was added, alterting the positions
          if (_year != 2003)
            {
              START_YEAR        = 105;
              END_YEAR          = 109;
              START_DECAYSTRING = 110;
            }
          else
            {
              START_YEAR        = 0;
              END_YEAR          = 0;
              START_DECAYSTRING = 106;
            }
          // The decay string goes to EOL, put here, commented,
          // to show that we haven't just forgotten about it.
          // END_DECAYSTRING = EOL;
        }
      else
        {
          HEADER              = 25;
          FOOTER              = std::numeric_limits<int>::max();
          START_A             = 0;
          END_A               = 3;
          START_Z             = 4;
          END_Z               = 7;
          START_STATE         = 7;
          END_STATE           = 8;
          START_ME            = 18;
          END_ME              = 30;
          START_DME           = 31;
          END_DME             = 41;
          START_ISOMER        = 42;
          END_ISOMER          = 53;
          START_DISOMER       = 54;
          END_DISOMER         = 64;
          START_HALFLIFEVALUE = 69;
          END_HALFLIFEVALUE   = 77;
          START_HALFLIFEUNIT  = 78;
          END_HALFLIFEUNIT    = 80;
          START_HALFLIFEERROR = 81;
          END_HALFLIFEERROR   = 87;
          START_SPIN          = 88;
          END_SPIN            = 101;
          START_ENSDF         = 102;
          END_ENSDF           = 103;
          START_YEAR          = 114;
          END_YEAR            = 118;
          START_DECAYSTRING   = 119;
          // The decay string goes to EOL, put here, commented,
          // to show that we haven't just forgotten about it.
          // END_DECAYSTRING = EOL;
        }
    }

    mutable int HEADER;
    mutable int FOOTER;
    mutable int START_A;
    mutable int END_A;
    mutable int START_Z;
    mutable int END_Z;
    mutable int START_STATE;
    mutable int END_STATE;
    mutable int START_ME;
    mutable int END_ME;
    mutable int START_DME;
    mutable int END_DME;
    mutable int START_ISOMER;
    mutable int END_ISOMER;
    mutable int START_DISOMER;
    mutable int END_DISOMER;
    mutable int START_HALFLIFEVALUE;
    mutable int END_HALFLIFEVALUE;
    mutable int START_HALFLIFEUNIT;
    mutable int END_HALFLIFEUNIT;
    mutable int START_HALFLIFEERROR;
    mutable int END_HALFLIFEERROR;
    mutable int START_SPIN;
    mutable int END_SPIN;
    mutable int START_ENSDF;
    mutable int END_ENSDF;
    mutable int START_YEAR;
    mutable int END_YEAR;
    mutable int START_DECAYSTRING;
  };
} // namespace NUBASE
#endif // NUBASE_LINE_POSITION_HPP

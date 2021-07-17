#ifndef NUBASE_LINE_POSITION_HPP
#define NUBASE_LINE_POSITION_HPP

#include <cstdint>
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
          FOOTER              = std::numeric_limits<uint16_t>::max();
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
          FOOTER              = std::numeric_limits<uint16_t>::max();
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

    // Max values
    // uint8_t  = 255
    // uint16_t = 65535
    mutable uint8_t HEADER;
    mutable uint16_t FOOTER;
    mutable uint8_t START_A;
    mutable uint8_t END_A;
    mutable uint8_t START_Z;
    mutable uint8_t END_Z;
    mutable uint8_t START_STATE;
    mutable uint8_t END_STATE;
    mutable uint8_t START_ME;
    mutable uint8_t END_ME;
    mutable uint8_t START_DME;
    mutable uint8_t END_DME;
    mutable uint8_t START_ISOMER;
    mutable uint8_t END_ISOMER;
    mutable uint8_t START_DISOMER;
    mutable uint8_t END_DISOMER;
    mutable uint8_t START_HALFLIFEVALUE;
    mutable uint8_t END_HALFLIFEVALUE;
    mutable uint8_t START_HALFLIFEUNIT;
    mutable uint8_t END_HALFLIFEUNIT;
    mutable uint8_t START_HALFLIFEERROR;
    mutable uint8_t END_HALFLIFEERROR;
    mutable uint8_t START_SPIN;
    mutable uint8_t END_SPIN;
    mutable uint8_t START_ENSDF;
    mutable uint8_t END_ENSDF;
    mutable uint8_t START_YEAR;
    mutable uint8_t END_YEAR;
    mutable uint8_t START_DECAYSTRING;
  };
} // namespace NUBASE
#endif // NUBASE_LINE_POSITION_HPP

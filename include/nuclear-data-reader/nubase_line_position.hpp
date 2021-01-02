#ifndef NUBASE_LINE_POSITION_HPP
#define NUBASE_LINE_POSITION_HPP

namespace NUBASE
{
  enum LinePosition
  {
    START_A             = 0,
    END_A               = 3,
    START_Z             = 4,
    END_Z               = 7,
    START_STATE         = 7,
    END_STATE           = 8,
    START_ME            = 18,
    END_ME              = 29,
    START_DME           = 29,
    END_DME             = 38,
    START_ISOMER        = 39,
    END_ISOMER          = 46,
    START_DISOMER       = 48,
    END_DISOMER         = 56,
    START_HALFLIFEVALUE = 60,
    END_HALFLIFEVALUE   = 69,
    START_HALFLIFEUNIT  = 69,
    END_HALFLIFEUNIT    = 71,
    START_HALFLIFEERROR = 72,
    END_HALFLIFEERROR   = 77,
    START_SPIN          = 79,
    END_SPIN            = 93,
    // After the 2003 table, the discovery
    // year was added, alterting the positions
    START_YEAR = 105,
    END_YEAR   = 109,
    // Let the 03 position be the odd-one-out and thus
    // have the slightly awkward name
    START_DECAYSTRING_03 = 106,
    START_DECAYSTRING    = 110,
    // The decay string goes to EOL, put here, commented,
    // to show that we haven't just forgotten about it.
    // END_DECAYSTRING = EOL;
  };
} // namespace NUBASE

#endif // NUBASE_LINE_POSITION_HPP

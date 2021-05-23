#ifndef AME_MASS_POSITION_HPP
#define AME_MASS_POSITION_HPP

namespace AME
{
  struct MassPosition
  {
    explicit MassPosition(const int _year)
    {
      if (_year < 2020)
        {
          HEADER                   = 39;
          FOOTER                   = 0;
          START_A                  = 16;
          END_A                    = 19;
          START_Z                  = 11;
          END_Z                    = 14;
          START_ME                 = 29;
          END_ME                   = 41;
          START_DME                = 42;
          END_DME                  = 53;
          START_BE_PER_A           = 54;
          END_BE_PER_A             = 64;
          START_DBE_PER_A          = 65;
          END_DBE_PER_A            = 72;
          START_BETA_DECAY_ENERGY  = 76;
          END_BETA_DECAY_ENERGY    = 86;
          START_DBETA_DECAY_ENERGY = 87;
          END_DBETA_DECAY_ENERGY   = 95;
          START_MICRO_U            = 100;
          END_MICRO_U              = 112;
          START_MICRO_DU           = 113;
          END_MICRO_DU             = 125;
        }
      else
        {
          HEADER                   = 36;
          FOOTER                   = 0;
          START_A                  = 16;
          END_A                    = 19;
          START_Z                  = 11;
          END_Z                    = 14;
          START_ME                 = 29;
          END_ME                   = 42;
          START_DME                = 43;
          END_DME                  = 53;
          START_BE_PER_A           = 56;
          END_BE_PER_A             = 66;
          START_DBE_PER_A          = 69;
          END_DBE_PER_A            = 77;
          START_BETA_DECAY_ENERGY  = 82;
          END_BETA_DECAY_ENERGY    = 93;
          START_DBETA_DECAY_ENERGY = 95;
          END_DBETA_DECAY_ENERGY   = 104;
          START_MICRO_U            = 110;
          END_MICRO_U              = 120;
          START_MICRO_DU           = 124;
          END_MICRO_DU             = 135;
        }
    }

    mutable int HEADER;
    mutable int FOOTER;
    mutable int START_A;
    mutable int END_A;
    mutable int START_Z;
    mutable int END_Z;
    mutable int START_ME;
    mutable int END_ME;
    mutable int START_DME;
    mutable int END_DME;
    mutable int START_BE_PER_A;
    mutable int END_BE_PER_A;
    mutable int START_DBE_PER_A;
    mutable int END_DBE_PER_A;
    mutable int START_BETA_DECAY_ENERGY;
    mutable int END_BETA_DECAY_ENERGY;
    mutable int START_DBETA_DECAY_ENERGY;
    mutable int END_DBETA_DECAY_ENERGY;
    mutable int START_MICRO_U;
    mutable int END_MICRO_U;
    mutable int START_MICRO_DU;
    mutable int END_MICRO_DU;
  };
} // namespace AME

#endif // AME_MASS_POSITION_HPP

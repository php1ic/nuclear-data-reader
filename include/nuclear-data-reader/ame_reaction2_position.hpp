#ifndef AME_REACTION2_POSITION_HPP
#define AME_REACTION2_POSITION_HPP

#include <cstdint>
#include <limits>

namespace AME
{
  struct Reaction2Position
  {
    constexpr explicit Reaction2Position(const uint16_t _year)
    {
      if (_year < 2020)
        {
          R2_HEADER  = 39;
          R2_FOOTER  = std::numeric_limits<uint16_t>::max();
          START_R2_A = 1;
          END_R2_A   = 4;
          START_R2_Z = 8;
          END_R2_Z   = 11;
          START_SN   = 14;
          END_SN     = 22;
          START_DSN  = 23;
          END_DSN    = 30;
          START_SP   = 32;
          END_SP     = 39;
          START_DSP  = 41;
          END_DSP    = 48;
          START_Q4B  = 49;
          END_Q4B    = 58;
          START_DQ4B = 59;
          END_DQ4B   = 66;
          START_QDA  = 67;
          END_QDA    = 75;
          START_DQDA = 77;
          END_DQDA   = 84;
          START_QPA  = 85;
          END_QPA    = 94;
          START_DQPA = 95;
          END_DQPA   = 102;
          START_QNA  = 103;
          END_QNA    = 111;
          START_DQNA = 113;
          END_DQNA   = 125;
        }
      else
        {
          R2_HEADER  = 38;
          R2_FOOTER  = 3644;
          START_R2_A = 1;
          END_R2_A   = 4;
          START_R2_Z = 8;
          END_R2_Z   = 11;
          START_SN   = 14;
          END_SN     = 24;
          START_DSN  = 25;
          END_DSN    = 34;
          START_SP   = 36;
          END_SP     = 46;
          START_DSP  = 47;
          END_DSP    = 56;
          START_Q4B  = 57;
          END_Q4B    = 68;
          START_DQ4B = 69;
          END_DQ4B   = 78;
          START_QDA  = 79;
          END_QDA    = 90;
          START_DQDA = 91;
          END_DQDA   = 100;
          START_QPA  = 101;
          END_QPA    = 112;
          START_DQPA = 113;
          END_DQPA   = 122;
          START_QNA  = 123;
          END_QNA    = 134;
          START_DQNA = 135;
          END_DQNA   = 144;
        }
    }

    mutable uint8_t R2_HEADER;
    mutable uint16_t R2_FOOTER;
    mutable uint8_t START_R2_A;
    mutable uint8_t END_R2_A;
    mutable uint8_t START_R2_Z;
    mutable uint8_t END_R2_Z;
    mutable uint8_t START_SN;
    mutable uint8_t END_SN;
    mutable uint8_t START_DSN;
    mutable uint8_t END_DSN;
    mutable uint8_t START_SP;
    mutable uint8_t END_SP;
    mutable uint8_t START_DSP;
    mutable uint8_t END_DSP;
    mutable uint8_t START_Q4B;
    mutable uint8_t END_Q4B;
    mutable uint8_t START_DQ4B;
    mutable uint8_t END_DQ4B;
    mutable uint8_t START_QDA;
    mutable uint8_t END_QDA;
    mutable uint8_t START_DQDA;
    mutable uint8_t END_DQDA;
    mutable uint8_t START_QPA;
    mutable uint8_t END_QPA;
    mutable uint8_t START_DQPA;
    mutable uint8_t END_DQPA;
    mutable uint8_t START_QNA;
    mutable uint8_t END_QNA;
    mutable uint8_t START_DQNA;
    mutable uint8_t END_DQNA;
  };
} // namespace AME

#endif // AME_REACTION2_POSITION_HPP

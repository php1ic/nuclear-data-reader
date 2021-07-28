#ifndef AME_REACTION1_POSITION_HPP
#define AME_REACTION1_POSITION_HPP

#include <cstdint>
#include <limits>

namespace AME
{
  struct Reaction1Position
  {
    explicit Reaction1Position(const uint16_t _year)
    {
      if (_year < 2020)
        {
          R1_HEADER  = 39;
          R1_FOOTER  = std::numeric_limits<uint16_t>::max();
          START_R1_A = 1;
          END_R1_A   = 4;
          START_R1_Z = 8;
          END_R1_Z   = 11;
          START_S2N  = 14;
          END_S2N    = 21;
          START_DS2N = 23;
          END_DS2N   = 30;
          START_S2P  = 32;
          END_S2P    = 39;
          START_DS2P = 41;
          END_DS2P   = 48;
          START_QA   = 50;
          END_QA     = 58;
          START_DQA  = 59;
          END_DQA    = 66;
          START_Q2B  = 67;
          END_Q2B    = 76;
          START_DQ2B = 77;
          END_DQ2B   = 84;
          START_QEP  = 85;
          END_QEP    = 94;
          START_DQEP = 95;
          END_DQEP   = 102;
          START_QBN  = 103;
          END_QBN    = 111;
          START_DQBN = 113;
          END_DQBN   = 125;
        }
      else
        {
          R1_HEADER  = 36;
          R1_FOOTER  = std::numeric_limits<uint16_t>::max();
          START_R1_A = 1;
          END_R1_A   = 4;
          START_R1_Z = 8;
          END_R1_Z   = 11;
          START_S2N  = 14;
          END_S2N    = 24;
          START_DS2N = 25;
          END_DS2N   = 34;
          START_S2P  = 36;
          END_S2P    = 46;
          START_DS2P = 47;
          END_DS2P   = 56;
          START_QA   = 58;
          END_QA     = 68;
          START_DQA  = 69;
          END_DQA    = 78;
          START_Q2B  = 79;
          END_Q2B    = 90;
          START_DQ2B = 91;
          END_DQ2B   = 100;
          START_QEP  = 101;
          END_QEP    = 112;
          START_DQEP = 113;
          END_DQEP   = 122;
          START_QBN  = 123;
          END_QBN    = 134;
          START_DQBN = 135;
          END_DQBN   = 144;
        }
    }

    mutable uint8_t R1_HEADER;
    mutable uint16_t R1_FOOTER;
    mutable uint8_t START_R1_A;
    mutable uint8_t END_R1_A;
    mutable uint8_t START_R1_Z;
    mutable uint8_t END_R1_Z;
    mutable uint8_t START_S2N;
    mutable uint8_t END_S2N;
    mutable uint8_t START_DS2N;
    mutable uint8_t END_DS2N;
    mutable uint8_t START_S2P;
    mutable uint8_t END_S2P;
    mutable uint8_t START_DS2P;
    mutable uint8_t END_DS2P;
    mutable uint8_t START_QA;
    mutable uint8_t END_QA;
    mutable uint8_t START_DQA;
    mutable uint8_t END_DQA;
    mutable uint8_t START_Q2B;
    mutable uint8_t END_Q2B;
    mutable uint8_t START_DQ2B;
    mutable uint8_t END_DQ2B;
    mutable uint8_t START_QEP;
    mutable uint8_t END_QEP;
    mutable uint8_t START_DQEP;
    mutable uint8_t END_DQEP;
    mutable uint8_t START_QBN;
    mutable uint8_t END_QBN;
    mutable uint8_t START_DQBN;
    mutable uint8_t END_DQBN;
  };
} // namespace AME

#endif // AME_REACTION1_POSITION_HPP

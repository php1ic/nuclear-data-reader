#ifndef AME_REACTION1_POSITION_HPP
#define AME_REACTION1_POSITION_HPP

#include <limits>

namespace AME
{
  struct Reaction1Position
  {
    explicit Reaction1Position(const int _year)
    {
      if (_year < 2020)
        {
          R1_HEADER  = 39;
          R1_FOOTER  = std::numeric_limits<int>::max();
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
          R1_FOOTER  = std::numeric_limits<int>::max();
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

    mutable int R1_HEADER;
    mutable int R1_FOOTER;
    mutable int START_R1_A;
    mutable int END_R1_A;
    mutable int START_R1_Z;
    mutable int END_R1_Z;
    mutable int START_S2N;
    mutable int END_S2N;
    mutable int START_DS2N;
    mutable int END_DS2N;
    mutable int START_S2P;
    mutable int END_S2P;
    mutable int START_DS2P;
    mutable int END_DS2P;
    mutable int START_QA;
    mutable int END_QA;
    mutable int START_DQA;
    mutable int END_DQA;
    mutable int START_Q2B;
    mutable int END_Q2B;
    mutable int START_DQ2B;
    mutable int END_DQ2B;
    mutable int START_QEP;
    mutable int END_QEP;
    mutable int START_DQEP;
    mutable int END_DQEP;
    mutable int START_QBN;
    mutable int END_QBN;
    mutable int START_DQBN;
    mutable int END_DQBN;
  };
} // namespace AME

#endif // AME_REACTION1_POSITION_HPP

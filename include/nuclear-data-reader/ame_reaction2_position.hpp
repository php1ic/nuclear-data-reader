#ifndef AME_REACTION2_POSITION_HPP
#define AME_REACTION2_POSITION_HPP

#include <limits>

namespace AME
{
  struct Reaction2Position
  {
    explicit Reaction2Position(const int _year)
    {
      if (_year < 2020)
        {
          R2_HEADER  = 39;
          R2_FOOTER  = std::numeric_limits<int>::max();
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

    mutable int R2_HEADER;
    mutable int R2_FOOTER;
    mutable int START_R2_A;
    mutable int END_R2_A;
    mutable int START_R2_Z;
    mutable int END_R2_Z;
    mutable int START_SN;
    mutable int END_SN;
    mutable int START_DSN;
    mutable int END_DSN;
    mutable int START_SP;
    mutable int END_SP;
    mutable int START_DSP;
    mutable int END_DSP;
    mutable int START_Q4B;
    mutable int END_Q4B;
    mutable int START_DQ4B;
    mutable int END_DQ4B;
    mutable int START_QDA;
    mutable int END_QDA;
    mutable int START_DQDA;
    mutable int END_DQDA;
    mutable int START_QPA;
    mutable int END_QPA;
    mutable int START_DQPA;
    mutable int END_DQPA;
    mutable int START_QNA;
    mutable int END_QNA;
    mutable int START_DQNA;
    mutable int END_DQNA;
  };
} // namespace AME

#endif // AME_REACTION2_POSITION_HPP

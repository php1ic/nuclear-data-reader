#ifndef AME_LINE_POSITION_HPP
#define AME_LINE_POSITION_HPP

namespace AME
{
  struct LinePosition
  {
    explicit LinePosition(const int _year)
    {
      if (_year < 2020)
        {
          // Mass table
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

          // Reaction file 1
          R1_HEADER  = 39;
          R1_FOOTER  = 0;
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

          // Reaction file 2
          R2_HEADER  = 0;
          R2_FOOTER  = 0;
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

    // Reaction file 1
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

    // Reaction file 2
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

#endif // AME_LINE_POSITION_HPP

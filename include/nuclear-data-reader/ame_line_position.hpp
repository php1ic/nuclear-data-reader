#ifndef AME_LINE_POSITION_HPP
#define AME_LINE_POSITION_HPP

namespace AME
{
  constexpr int HEADER_LENGTH{ 39 };

  enum LinePosition
  {
    // Mass table
    START_A                  = 16,
    END_A                    = 19,
    START_Z                  = 11,
    END_Z                    = 14,
    START_ME                 = 29,
    END_ME                   = 41,
    START_DME                = 42,
    END_DME                  = 53,
    START_BE_PER_A           = 54,
    END_BE_PER_A             = 64,
    START_DBE_PER_A          = 65,
    END_DBE_PER_A            = 72,
    START_BETA_DECAY_ENERGY  = 76,
    END_BETA_DECAY_ENERGY    = 86,
    START_DBETA_DECAY_ENERGY = 87,
    END_DBETA_DECAY_ENERGY   = 95,
    START_MICRO_U            = 100,
    END_MICRO_U              = 112,
    START_MICRO_DU           = 113,
    END_MICRO_DU             = 125,

    // Reaction file 1
    START_R1_A = 1,
    END_R1_A   = 4,
    START_R1_Z = 8,
    END_R1_Z   = 11,
    START_S2N  = 14,
    END_S2N    = 21,
    START_DS2N = 23,
    END_DS2N   = 30,
    START_S2P  = 32,
    END_S2P    = 39,
    START_DS2P = 41,
    END_DS2P   = 48,
    START_QA   = 50,
    END_QA     = 58,
    START_DQA  = 59,
    END_DQA    = 66,
    START_Q2B  = 67,
    END_Q2B    = 76,
    START_DQ2B = 77,
    END_DQ2B   = 84,
    START_QEP  = 85,
    END_QEP    = 93,
    START_DQEP = 95,
    END_DQEP   = 101,
    START_QBN  = 103,
    END_QBN    = 111,
    START_DQBN = 113,
    END_DQBN   = 125,

    // Reaction file 2
    START_R2_A = 1,
    END_R2_A   = 4,
    START_R2_Z = 8,
    END_R2_Z   = 11,
    START_SN   = 14,
    END_SN     = 21,
    START_DSN  = 23,
    END_DSN    = 30,
    START_SP   = 32,
    END_SP     = 39,
    START_DSP  = 41,
    END_DSP    = 48,
    START_Q4B  = 49,
    END_Q4B    = 58,
    START_DQ4B = 59,
    END_DQ4B   = 65,
    START_QDA  = 67,
    END_QDA    = 75,
    START_DQDA = 77,
    END_DQDA   = 84,
    START_QPA  = 85,
    END_QPA    = 93,
    START_DQPA = 95,
    END_DQPA   = 101,
    START_QNA  = 103,
    END_QNA    = 111,
    START_DQNA = 113,
    END_DQNA   = 125
  };
} // namespace AME

#endif // AME_LINE_POSITION_HPP

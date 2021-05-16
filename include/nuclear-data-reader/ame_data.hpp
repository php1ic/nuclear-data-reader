/**
 *
 * \class AMEData
 *
 * \brief Parse and store all of the AME data
 *
 *
 */
#ifndef AMEDATA_HPP
#define AMEDATA_HPP

#include "nuclear-data-reader/ame_line_position.hpp"
#include "nuclear-data-reader/converter.hpp"
#include <string_view>

#include <string>

namespace AME
{
  class Data
  {
  public:
    Data(std::string line, const int _year) : position(_year), full_data(std::move(line)) {}

    Data(const Data&) = default;
    Data(Data&&)      = default;

    Data& operator=(const Data&) = default;
    Data& operator=(Data&&) = default;

    ~Data() = default;

    LinePosition position;

    /// Is the isotope experimental or extrapolated/theoretical
    mutable int exp{ 0 };
    /// The mass number
    mutable int A{ 0 };
    /// The proton number
    mutable int Z{ 0 };
    /// The neutron number
    mutable int N{ 0 };
    /// Mass excess from the AME table
    mutable double mass_excess{ 0.1 };
    /// Error on the mass excess from the AME table
    mutable double dmass_excess{ 1.0e4 };
    /// Binding energy per A
    mutable double binding_energy_per_A{ 0.0 };
    /// Error on inding energy per A
    mutable double dbinding_energy_per_A{ 0.0 };
    /// Beta decay energy
    mutable double beta_decay_energy{ 0.0 };
    /// Error on beta decay energy
    mutable double dbeta_decay_energy{ 0.0 };
    /// Atomic mass
    mutable double atomic_mass{ 0.0 };
    /// Error on atomic mass
    mutable double datomic_mass{ 0.0 };
    /// One neutron separation energy
    mutable double s_n{ 0.0 };
    /// Error on the one neutron separation energy
    mutable double ds_n{ 0.0 };
    /// Two neutron separation energy
    mutable double s_2n{ 0.0 };
    /// Error on the two neutron separation energy
    mutable double ds_2n{ 0.0 };
    /// One proton separation energy
    mutable double s_p{ 0.0 };
    /// Error on the one proton separation energy
    mutable double ds_p{ 0.0 };
    /// Two proton separation energy
    mutable double s_2p{ 0.0 };
    /// Error on the two proton separtion energy
    mutable double ds_2p{ 0.0 };
    ///
    mutable double dV_pn{ 0.0 };
    ///
    mutable double ddV_pn{ 0.0 };
    /// Q-alpha value
    mutable double q_a{ 0.0 };
    /// Error on Q-alpha
    mutable double dq_a{ 0.0 };
    /// Q 2 beta minus
    mutable double q_2bm{ 0.0 };
    /// Error on Q 2 beta minus
    mutable double dq_2bm{ 0.0 };
    /// Not sure what this is
    mutable double q_ep{ 0.0 };
    ///
    mutable double dq_ep{ 0.0 };
    /// Q beta minus, n
    mutable double q_bm_n{ 0.0 };
    /// Error on Q beta minus, n
    mutable double dq_bm_n{ 0.0 };
    /// Q value for 4 beta minus
    mutable double q_4bm{ 0.0 };
    /// Error on Q value for 4 beta minus
    mutable double dq_4bm{ 0.0 };
    /// Q value for d alpha reaction
    mutable double q_da{ 0.0 };
    /// Error on Q value for d alpha reaction
    mutable double dq_da{ 0.0 };
    /// Q value for p alpha reaction
    mutable double q_pa{ 0.0 };
    /// Error on Q value for p alpha reaction
    mutable double dq_pa{ 0.0 };
    /// Q value for n alpha reaction
    mutable double q_na{ 0.0 };
    /// Error on Q value for n alpha reaction
    mutable double dq_na{ 0.0 };

    /// Isotopic symbol
    // mutable std::string symbol{};
    /// The entire line for the isotope from the data file
    mutable std::string full_data{};

    /**
     * Set the neutron number
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setN() const noexcept { N = A - Z; }

    /**
     * Extract the mass number from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setA() const { A = Converter::StringToInt(full_data, position.START_A, position.END_A); }

    /**
     *
     */
    [[nodiscard]] inline int getReaction_1_A(const std::string& line) const
    {
      return Converter::StringToInt(line, position.START_R1_A, position.END_R1_A);
    }

    [[nodiscard]] inline int getReaction_2_A(const std::string& line) const
    {
      return Converter::StringToInt(line, position.START_R2_A, position.END_R2_A);
    }

    /**
     *
     */
    [[nodiscard]] inline int getReaction_1_Z(const std::string& line) const
    {
      return Converter::StringToInt(line, position.START_R1_Z, position.END_R1_Z);
    }

    [[nodiscard]] inline int getReaction_2_Z(const std::string& line) const
    {
      return Converter::StringToInt(line, position.START_R2_Z, position.END_R2_Z);
    }

    /**
     * Extract the proton number from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setZ() const { Z = Converter::StringToInt(full_data, position.START_Z, position.END_Z); }

    /**
     * Extract the mass excess from a AME formatted line
     *
     * \param The line to extract the mass excess from
     *
     * \return Nothing
     */
    inline void setMassExcess() const
    {
      mass_excess = Converter::StringToDouble(full_data, position.START_ME, position.END_ME);
    }

    /**
     * Extract the error on the mass excess from a AME formatted line
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setMassExcessError() const
    {
      dmass_excess = Converter::StringToDouble(full_data, position.START_DME, position.END_DME);
    }

    /**
     * Calculate the relative error on the isotope, but use a low water mark of <min_allowed>
     *
     * \param min_allowed The value to not let the relative error fall below
     *
     * \return The relative error on the isotope or min_allowed, whichever is larger
     */
    [[nodiscard]] double getRelativeMassExcessError(const double min_allowed) const;

    /**
     * Modify the line from the data file swapping '#' for ' ' to maintain positioning.
     * Also set the exp flag as required
     *
     * \param Nothing
     *
     * \return Nothing
     */
    void setExperimental() const;

    /**
     * Set the experimental flag
     *
     * \param The value to set the experimental flag
     *
     * \return Nothing
     */
    inline void setExperimental(const int val) const noexcept { exp = val; }

    /**
     * Extract the binding energy per A
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setBindingEnergyPerA() const
    {
      binding_energy_per_A = Converter::StringToDouble(full_data, position.START_BE_PER_A, position.END_BE_PER_A);
    }

    /**
     * Extract the error on the binding energy per A
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setBindingEnergyPerAError() const
    {
      dbinding_energy_per_A = Converter::StringToDouble(full_data, position.START_DBE_PER_A, position.END_DBE_PER_A);
    }

    /**
     * Extract the beta decay energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setBetaDecayEnergy() const
    {
      beta_decay_energy =
          Converter::StringToDouble(full_data, position.START_BETA_DECAY_ENERGY, position.END_BETA_DECAY_ENERGY);
    }

    /**
     * Extract the error on the beta decay energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setBetaDecayEnergyError() const
    {
      dbeta_decay_energy =
          Converter::StringToDouble(full_data, position.START_DBETA_DECAY_ENERGY, position.END_DBETA_DECAY_ENERGY);
    }

    /**
     * Extract the atomic mass
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setAtomicMass() const
    {
      atomic_mass = Converter::StringToDouble(full_data, position.START_MICRO_U, position.END_MICRO_U);
    }

    /**
     * Extract the error on atomic mass
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setAtomicMassError() const
    {
      datomic_mass = Converter::StringToDouble(full_data, position.START_MICRO_DU, position.END_MICRO_DU);
    }

    /**
     * Extract the 2 neutron separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setTwoNeutronSeparationEnergy() const
    {
      s_2n = Converter::StringToDouble(full_data, position.START_S2N, position.END_S2N);
    }

    /**
     * Extract the error on the 2 neutron separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setTwoNeutronSeparationEnergyError() const
    {
      ds_2n = Converter::StringToDouble(full_data, position.START_DS2N, position.END_DS2N);
    }

    /**
     * Extract the 2 proton separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setTwoProtonSeparationEnergy() const
    {
      s_2p = Converter::StringToDouble(full_data, position.START_S2P, position.END_S2P);
    }

    /**
     * Extract the error on the 2 proton separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setTwoProtonSeparationEnergyError() const
    {
      ds_2p = Converter::StringToDouble(full_data, position.START_DS2P, position.END_DS2P);
    }

    /**
     * Extract the Q alpha energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQAlphaEnergy() const
    {
      q_a = Converter::StringToDouble(full_data, position.START_QA, position.END_QA);
    }

    /**
     * Extract the error on Q alpha energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQAlphaEnergyError() const
    {
      dq_a = Converter::StringToDouble(full_data, position.START_DQA, position.END_DQA);
    }

    /**
     * Extract the double beta minus Q-value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQDoubleBetaMinusEnergy() const
    {
      q_2bm = Converter::StringToDouble(full_data, position.START_Q2B, position.END_Q2B);
    }

    /**
     * Extract the error on the double beta minus Q-value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQDoubleBetaMinusEnergyError() const
    {
      dq_2bm = Converter::StringToDouble(full_data, position.START_DQ2B, position.END_DQ2B);
    }

    /**
     * Extract the epsilon p Q-value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQEpsilonPEnergy() const
    {
      q_ep = Converter::StringToDouble(full_data, position.START_QEP, position.END_QEP);
    }

    /**
     * Extract the error on the epsilon p Q-value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQEpsilonPEnergyError() const
    {
      dq_ep = Converter::StringToDouble(full_data, position.START_DQEP, position.END_DQEP);
    }

    /**
     * Extract the beta minus n Q-value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQBetaMinusNEnergy() const
    {
      q_bm_n = Converter::StringToDouble(full_data, position.START_QBN, position.END_QBN);
    }

    /**
     * Extract the error on beta minus n Q-value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQBetaMinusNEnergyError() const
    {
      dq_bm_n = Converter::StringToDouble(full_data, position.START_DQBN, position.END_DQBN);
    }

    /**
     * Extract the 1 neutron separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setOneNeutronSeparationEnergy() const
    {
      s_n = Converter::StringToDouble(full_data, position.START_SN, position.END_SN);
    }

    /**
     * Extract the error on the 1 neutron separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setOneNeutronSeparationEnergyError() const
    {
      ds_n = Converter::StringToDouble(full_data, position.START_DSN, position.END_DSN);
    }

    /**
     * Extract the 1 proton separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setOneProtonSeparationEnergy() const
    {
      s_p = Converter::StringToDouble(full_data, position.START_SP, position.END_SP);
    }

    /**
     * Extract the error on the 1 proton separation energy
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setOneProtonSeparationEnergyError() const
    {
      ds_p = Converter::StringToDouble(full_data, position.START_DSP, position.END_DSP);
    }

    /**
     * Extract the 4 beta minus Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQQuadrupleBetaMinusEnergy() const
    {
      q_4bm = Converter::StringToDouble(full_data, position.START_Q4B, position.END_Q4B);
    }

    /**
     * Extract the error on 4 beta minus Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQQuadrupleBetaMinusEnergyError() const
    {
      dq_4bm = Converter::StringToDouble(full_data, position.START_DQ4B, position.END_DQ4B);
    }

    /**
     * Extract the d alpha Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQDAlphaEnergy() const
    {
      q_da = Converter::StringToDouble(full_data, position.START_QDA, position.END_QDA);
    }

    /**
     * Extract the error on the d alpha Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQDAlphaEnergyError() const
    {
      dq_da = Converter::StringToDouble(full_data, position.START_DQDA, position.END_DQDA);
    }

    /**
     * Extract the p alpha Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQPAlphaEnergy() const
    {
      q_pa = Converter::StringToDouble(full_data, position.START_QPA, position.END_QPA);
    }

    /**
     * Extract the error on the p alpha Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQPAlphaEnergyError() const
    {
      dq_pa = Converter::StringToDouble(full_data, position.START_DQPA, position.END_DQPA);
    }

    /**
     * Extract the n alpha Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQNAlphaEnergy() const
    {
      q_na = Converter::StringToDouble(full_data, position.START_QNA, position.END_QNA);
    }

    /**
     * Extract the n alpha Q value
     *
     * \param The line to extract the value from
     *
     * \return Nothing
     */
    inline void setQNAlphaEnergyError() const
    {
      dq_na = Converter::StringToDouble(full_data, position.START_DQNA, position.END_DQNA);
    }
  };
} // namespace AME

#endif // AMEDATA_HPP

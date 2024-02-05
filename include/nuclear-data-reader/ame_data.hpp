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

#include "nuclear-data-reader/ame_mass_position.hpp"
#include "nuclear-data-reader/ame_reaction1_position.hpp"
#include "nuclear-data-reader/ame_reaction2_position.hpp"
#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/number.hpp"

#include <cstdint>
#include <string>

namespace AME
{
  enum LINE_LENGTH : uint8_t
  {
    PRE_2020  = 125,
    POST_2020 = 144
  };

  enum class Measured : uint8_t
  {
    EXPERIMENTAL = 0,
    THEORETICAL  = 1
  };

  class Data
  {
  public:
    Data(std::string line, const uint16_t _year) :
        mass_position(_year), r1_position(_year), r2_position(_year), full_data(std::move(line))
    {
    }

    Data(const Data&)     = default;
    Data(Data&&) noexcept = default;

    Data& operator=(const Data&)     = default;
    Data& operator=(Data&&) noexcept = default;

    ~Data() = default;

    /// Where are the variables located on the line in the file(s)
    mutable MassPosition mass_position;
    mutable Reaction1Position r1_position;
    mutable Reaction2Position r2_position;

    /// Is the isotope experimental or extrapolated/theoretical
    mutable Measured exp{ Measured::EXPERIMENTAL };

    /// The mass number
    mutable uint16_t A{ 0 };
    /// The proton number
    mutable uint16_t Z{ 0 };
    /// The neutron number
    mutable uint16_t N{ 0 };

    /// Mass excess from the AME table
    mutable Number mass_excess{};
    /// Binding energy per A
    mutable Number binding_energy_per_A{};
    /// Beta decay energy
    mutable Number beta_decay_energy{};
    /// Atomic mass
    mutable Number atomic_mass{};
    /// One neutron separation energy
    mutable Number s_n{};
    /// Two neutron separation energy
    mutable Number s_2n{};
    /// One proton separation energy
    mutable Number s_p{};
    /// Two proton separation energy
    mutable Number s_2p{};
    ///
    mutable Number dV_pn{};
    /// Q-alpha value
    mutable Number q_a{};
    /// Q 2 beta minus
    mutable Number q_2bm{};
    /// Not sure what this is
    mutable Number q_ep{};
    /// Q beta minus, n
    mutable Number q_bm_n{};
    /// Q value for 4 beta minus
    mutable Number q_4bm{};
    /// Q value for d alpha reaction
    mutable Number q_da{};
    /// Q value for p alpha reaction
    mutable Number q_pa{};
    /// Q value for n alpha reaction
    mutable Number q_na{};

    /// The entire line for the isotope from the data file
    mutable std::string full_data{};

    /**
     * Extract the neutron number
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setN() const
    {
      N = Converter::StringToNum<uint16_t>(full_data, mass_position.START_N, mass_position.END_N);
    }

    /**
     * Extract the mass number from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setA(const uint16_t _year) const
    {
      A = (_year == 1983) ? (N + Z)
                          : Converter::StringToNum<uint16_t>(full_data, mass_position.START_A, mass_position.END_A);
    }

    /**
     *
     */
    [[nodiscard]] inline auto getReaction_1_A(std::string_view line) const
    {
      return Converter::StringToNum<uint16_t>(line, r1_position.START_R1_A, r1_position.END_R1_A);
    }

    [[nodiscard]] inline auto getReaction_2_A(std::string_view line) const
    {
      return Converter::StringToNum<uint16_t>(line, r2_position.START_R2_A, r2_position.END_R2_A);
    }

    /**
     *
     */
    [[nodiscard]] inline auto getReaction_1_Z(std::string_view line) const
    {
      return Converter::StringToNum<uint16_t>(line, r1_position.START_R1_Z, r1_position.END_R1_Z);
    }

    [[nodiscard]] inline auto getReaction_2_Z(std::string_view line) const
    {
      return Converter::StringToNum<uint16_t>(line, r2_position.START_R2_Z, r2_position.END_R2_Z);
    }

    /**
     * Extract the proton number from the data file
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setZ() const
    {
      Z = Converter::StringToNum<uint16_t>(full_data, mass_position.START_Z, mass_position.END_Z);
    }

    /**
     * Extract the mass excess from a AME formatted line
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setMassExcess() const
    {
      mass_excess.amount = Converter::StringToNum<double>(full_data, mass_position.START_ME, mass_position.END_ME);
    }

    /**
     * Extract the error on the mass excess from a AME formatted line
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setMassExcessError() const
    {
      mass_excess.uncertainty.emplace(
          Converter::StringToNum<double>(full_data, mass_position.START_DME, mass_position.END_DME));
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
    inline void setExperimental(const AME::Measured val) const noexcept { exp = val; }

    /**
     * Extract the binding energy per A
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setBindingEnergyPerA() const
    {
      binding_energy_per_A.amount =
          Converter::StringToNum<double>(full_data, mass_position.START_BE_PER_A, mass_position.END_BE_PER_A);
    }

    /**
     * Extract the error on the binding energy per A
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setBindingEnergyPerAError() const
    {
      binding_energy_per_A.uncertainty.emplace(
          Converter::StringToNum<double>(full_data, mass_position.START_DBE_PER_A, mass_position.END_DBE_PER_A));
    }

    /**
     * Extract the beta decay energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setBetaDecayEnergy() const
    {
      beta_decay_energy.amount = Converter::StringToNum<double>(
          full_data, mass_position.START_BETA_DECAY_ENERGY, mass_position.END_BETA_DECAY_ENERGY);
    }

    /**
     * Extract the error on the beta decay energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setBetaDecayEnergyError() const
    {
      beta_decay_energy.uncertainty.emplace(Converter::StringToNum<double>(
          full_data, mass_position.START_DBETA_DECAY_ENERGY, mass_position.END_DBETA_DECAY_ENERGY));
    }

    /**
     * Extract the atomic mass
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setAtomicMass() const
    {
      atomic_mass.amount =
          Converter::StringToNum<double>(full_data, mass_position.START_MICRO_U, mass_position.END_MICRO_U);
    }

    /**
     * Extract the error on atomic mass
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setAtomicMassError() const
    {
      atomic_mass.uncertainty.emplace(
          Converter::StringToNum<double>(full_data, mass_position.START_MICRO_DU, mass_position.END_MICRO_DU));
    }

    /**
     * Extract the 2 neutron separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setTwoNeutronSeparationEnergy() const
    {
      s_2n.amount = Converter::StringToNum<double>(full_data, r1_position.START_S2N, r1_position.END_S2N);
    }

    /**
     * Extract the error on the 2 neutron separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setTwoNeutronSeparationEnergyError() const
    {
      s_2n.uncertainty.emplace(Converter::StringToNum<double>(full_data, r1_position.START_DS2N, r1_position.END_DS2N));
    }

    /**
     * Extract the 2 proton separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setTwoProtonSeparationEnergy() const
    {
      s_2p.amount = Converter::StringToNum<double>(full_data, r1_position.START_S2P, r1_position.END_S2P);
    }

    /**
     * Extract the error on the 2 proton separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setTwoProtonSeparationEnergyError() const
    {
      s_2p.uncertainty.emplace(Converter::StringToNum<double>(full_data, r1_position.START_DS2P, r1_position.END_DS2P));
    }

    /**
     * Extract the Q alpha energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQAlphaEnergy() const
    {
      q_a.amount = Converter::StringToNum<double>(full_data, r1_position.START_QA, r1_position.END_QA);
    }

    /**
     * Extract the error on Q alpha energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQAlphaEnergyError() const
    {
      q_a.uncertainty.emplace(Converter::StringToNum<double>(full_data, r1_position.START_DQA, r1_position.END_DQA));
    }

    /**
     * Extract the double beta minus Q-value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQDoubleBetaMinusEnergy() const
    {
      q_2bm.amount = Converter::StringToNum<double>(full_data, r1_position.START_Q2B, r1_position.END_Q2B);
    }

    /**
     * Extract the error on the double beta minus Q-value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQDoubleBetaMinusEnergyError() const
    {
      q_2bm.uncertainty.emplace(
          Converter::StringToNum<double>(full_data, r1_position.START_DQ2B, r1_position.END_DQ2B));
    }

    /**
     * Extract the epsilon p Q-value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQEpsilonPEnergy() const
    {
      q_ep.amount = Converter::StringToNum<double>(full_data, r1_position.START_QEP, r1_position.END_QEP);
    }

    /**
     * Extract the error on the epsilon p Q-value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQEpsilonPEnergyError() const
    {
      q_ep.uncertainty.emplace(Converter::StringToNum<double>(full_data, r1_position.START_DQEP, r1_position.END_DQEP));
    }

    /**
     * Extract the beta minus n Q-value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQBetaMinusNEnergy() const
    {
      q_bm_n.amount = Converter::StringToNum<double>(full_data, r1_position.START_QBN, r1_position.END_QBN);
    }

    /**
     * Extract the error on beta minus n Q-value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQBetaMinusNEnergyError() const
    {
      q_bm_n.uncertainty.emplace(
          Converter::StringToNum<double>(full_data, r1_position.START_DQBN, r1_position.END_DQBN));
    }

    /**
     * Extract the 1 neutron separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setOneNeutronSeparationEnergy() const
    {
      s_n.amount = Converter::StringToNum<double>(full_data, r2_position.START_SN, r2_position.END_SN);
    }

    /**
     * Extract the error on the 1 neutron separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setOneNeutronSeparationEnergyError() const
    {
      s_n.uncertainty.emplace(Converter::StringToNum<double>(full_data, r2_position.START_DSN, r2_position.END_DSN));
    }

    /**
     * Extract the 1 proton separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setOneProtonSeparationEnergy() const
    {
      s_p.amount = Converter::StringToNum<double>(full_data, r2_position.START_SP, r2_position.END_SP);
    }

    /**
     * Extract the error on the 1 proton separation energy
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setOneProtonSeparationEnergyError() const
    {
      s_p.uncertainty.emplace(Converter::StringToNum<double>(full_data, r2_position.START_DSP, r2_position.END_DSP));
    }

    /**
     * Extract the 4 beta minus Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQQuadrupleBetaMinusEnergy() const
    {
      q_4bm.amount = Converter::StringToNum<double>(full_data, r2_position.START_Q4B, r2_position.END_Q4B);
    }

    /**
     * Extract the error on 4 beta minus Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQQuadrupleBetaMinusEnergyError() const
    {
      q_4bm.uncertainty.emplace(
          Converter::StringToNum<double>(full_data, r2_position.START_DQ4B, r2_position.END_DQ4B));
    }

    /**
     * Extract the d alpha Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQDAlphaEnergy() const
    {
      q_da.amount = Converter::StringToNum<double>(full_data, r2_position.START_QDA, r2_position.END_QDA);
    }

    /**
     * Extract the error on the d alpha Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQDAlphaEnergyError() const
    {
      q_da.uncertainty.emplace(Converter::StringToNum<double>(full_data, r2_position.START_DQDA, r2_position.END_DQDA));
    }

    /**
     * Extract the p alpha Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQPAlphaEnergy() const
    {
      q_pa.amount = Converter::StringToNum<double>(full_data, r2_position.START_QPA, r2_position.END_QPA);
    }

    /**
     * Extract the error on the p alpha Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQPAlphaEnergyError() const
    {
      q_pa.uncertainty.emplace(Converter::StringToNum<double>(full_data, r2_position.START_DQPA, r2_position.END_DQPA));
    }

    /**
     * Extract the n alpha Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQNAlphaEnergy() const
    {
      q_na.amount = Converter::StringToNum<double>(full_data, r2_position.START_QNA, r2_position.END_QNA);
    }

    /**
     * Extract the n alpha Q value
     *
     * \param Nothing
     *
     * \return Nothing
     */
    inline void setQNAlphaEnergyError() const
    {
      q_na.uncertainty.emplace(Converter::StringToNum<double>(full_data, r2_position.START_DQNA, r2_position.END_DQNA));
    }
  };
} // namespace AME

#endif // AMEDATA_HPP

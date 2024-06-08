#include "nuclear-data-reader/isotope.hpp"

#include "nuclear-data-reader/converter.hpp"
#include "nuclear-data-reader/nubase_data.hpp"

#include <fmt/core.h>

#include <string>

std::string Isotope::writeAsCSV() const
{
  // One item per line as I find it simpler to use
  return fmt::format("{0},"
                     "{1},"
                     "{2},"
                     "{3},"
                     "{4},"
                     "{5},"
                     "{6},"
                     "{7},"
                     "{8},"
                     "{9},"
                     "{10:.3e},"
                     "{11},"
                     "{12},"
                     "{13},"
                     "{14},"
                     "{15},"
                     "{16},"
                     "{17},"
                     "{18},"
                     "{19},"
                     "{20},"
                     "{21},"
                     "{22},"
                     "{23},"
                     "{24},"
                     "{25},"
                     "{26},"
                     "{27},"
                     "{28},"
                     "{29},"
                     "{30},"
                     "{31},"
                     "{32},"
                     "{33},"
                     "{34},"
                     "{35},"
                     "{36},"
                     "{37},"
                     "{38},"
                     "{39},"
                     "{40},"
                     "{41}",
                     ame.A,
                     ame.Z,
                     ame.N,
                     nubase.symbol,
                     nubase.decay,
                     (nubase.exp == NUBASE::Measured::EXPERIMENTAL) ? 0 : 1,
                     Converter::FloatToNdp(nubase.mass_excess.amount, NDP),
                     Converter::FloatToNdp(nubase.mass_excess.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.mass_excess.amount, NDP),
                     Converter::FloatToNdp(ame.mass_excess.uncertainty.value_or(-1.0), NDP),
                     nubase.hl.count(),
                     Converter::FloatToNdp(ame.s_n.amount, NDP),
                     Converter::FloatToNdp(ame.s_n.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.s_p.amount, NDP),
                     Converter::FloatToNdp(ame.s_p.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.s_2n.amount, NDP),
                     Converter::FloatToNdp(ame.s_2n.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.s_2p.amount, NDP),
                     Converter::FloatToNdp(ame.s_2p.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.binding_energy_per_A.amount, NDP),
                     Converter::FloatToNdp(ame.binding_energy_per_A.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.atomic_mass.amount, NDP),
                     Converter::FloatToNdp(ame.atomic_mass.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.beta_decay_energy.amount, NDP),
                     Converter::FloatToNdp(ame.beta_decay_energy.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_a.amount, NDP),
                     Converter::FloatToNdp(ame.q_a.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_2bm.amount, NDP),
                     Converter::FloatToNdp(ame.q_2bm.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_ep.amount, NDP),
                     Converter::FloatToNdp(ame.q_ep.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_bm_n.amount, NDP),
                     Converter::FloatToNdp(ame.q_bm_n.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_4bm.amount, NDP),
                     Converter::FloatToNdp(ame.q_4bm.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_da.amount, NDP),
                     Converter::FloatToNdp(ame.q_da.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_pa.amount, NDP),
                     Converter::FloatToNdp(ame.q_pa.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_na.amount, NDP),
                     Converter::FloatToNdp(ame.q_na.uncertainty.value_or(-1.0), NDP),
                     nubase.year);
}

std::string Isotope::writeAsJSON(const bool human_readable) const
{
  return fmt::format("{{{0}"
                     "\"A\":{1},{0}"
                     "\"Z\":{2},{0}"
                     "\"N\":{3},{0}"
                     "\"Symbol\":\"{4}\",{0}"
                     "\"Decay\":\"{5}\",{0}"
                     "\"Experimental\":{6},{0}"
                     "\"NubaseMassExcess\":{7},{0}"
                     "\"ErrorNubaseMassExcess\":{8},{0}"
                     "\"AMEMassExcess\":{9},{0}"
                     "\"ErrorAMEMassExcess\":{10},{0}"
                     "\"HalfLife\":{11:.3e},{0}"
                     "\"SingleNeutronSeparationEnergy\":{12},{0}"
                     "\"ErrorSingleNeutronSeparationEnergy\":{13},{0}"
                     "\"SingleProtonSeparationEnergy\":{14},{0}"
                     "\"ErrorSingleProtonSeparationEnergy\":{15},{0}"
                     "\"DoubleNeutronSeparationEnergy\":{16},{0}"
                     "\"ErrorDoubleNeutronSeparationEnergy\":{17},{0}"
                     "\"DoubleProtonSeparationEnergy\":{18},{0}"
                     "\"ErrorDoubleProtonSeparationEnergy\":{19},{0}"
                     "\"BindingEnergyPerA\":{20},{0}"
                     "\"ErrorBindingEnergyPerA\":{21},{0}"
                     "\"AtomicMass\":{22},{0}"
                     "\"ErrorAtomicMass\":{23},{0}"
                     "\"BetaDecayEnergy\":{24},{0}"
                     "\"ErrorBetaDecayEnergy\":{25},{0}"
                     "\"QAlpha\":{26},{0}"
                     "\"ErrorQAlpha\":{27},{0}"
                     "\"Q2B-\":{28},{0}"
                     "\"ErrorQ2B-\":{29},{0}"
                     "\"Qepsilon_p\":{30},{0}"
                     "\"ErrorQepsilon_p\":{31},{0}"
                     "\"QB-n\":{32},{0}"
                     "\"ErrorQB-n\":{33},{0}"
                     "\"Q4B-\":{34},{0}"
                     "\"ErrorQ4B-\":{35},{0}"
                     "\"QdAlpha\":{36},{0}"
                     "\"ErrorQdAlpha\":{37},{0}"
                     "\"QpAlpha\":{38},{0}"
                     "\"ErrorQpAlpha\":{39},{0}"
                     "\"QnAlpha\":{40},{0}"
                     "\"ErrorQnAlpha\":{41},{0}"
                     "\"Year\":{42}{0}"
                     "}}",
                     human_readable ? "\n" : "",
                     ame.A,
                     ame.Z,
                     ame.N,
                     nubase.symbol,
                     nubase.decay,
                     (nubase.exp == NUBASE::Measured::EXPERIMENTAL) ? 0 : 1,
                     Converter::FloatToNdp(nubase.mass_excess.amount, NDP),
                     Converter::FloatToNdp(nubase.mass_excess.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.mass_excess.amount, NDP),
                     Converter::FloatToNdp(ame.mass_excess.uncertainty.value_or(-1.0), NDP),
                     nubase.hl.count(),
                     Converter::FloatToNdp(ame.s_n.amount, NDP),
                     Converter::FloatToNdp(ame.s_n.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.s_p.amount, NDP),
                     Converter::FloatToNdp(ame.s_p.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.s_2n.amount, NDP),
                     Converter::FloatToNdp(ame.s_2n.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.s_2p.amount, NDP),
                     Converter::FloatToNdp(ame.s_2p.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.binding_energy_per_A.amount, NDP),
                     Converter::FloatToNdp(ame.binding_energy_per_A.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.atomic_mass.amount, NDP),
                     Converter::FloatToNdp(ame.atomic_mass.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.beta_decay_energy.amount, NDP),
                     Converter::FloatToNdp(ame.beta_decay_energy.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_a.amount, NDP),
                     Converter::FloatToNdp(ame.q_a.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_2bm.amount, NDP),
                     Converter::FloatToNdp(ame.q_2bm.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_ep.amount, NDP),
                     Converter::FloatToNdp(ame.q_ep.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_bm_n.amount, NDP),
                     Converter::FloatToNdp(ame.q_bm_n.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_4bm.amount, NDP),
                     Converter::FloatToNdp(ame.q_4bm.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_da.amount, NDP),
                     Converter::FloatToNdp(ame.q_da.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_pa.amount, NDP),
                     Converter::FloatToNdp(ame.q_pa.uncertainty.value_or(-1.0), NDP),
                     Converter::FloatToNdp(ame.q_na.amount, NDP),
                     Converter::FloatToNdp(ame.q_na.uncertainty.value_or(-1.0), NDP),
                     nubase.year);
}

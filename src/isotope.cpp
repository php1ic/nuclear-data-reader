#include "nuclear-data-reader/isotope.hpp"

#include "nuclear-data-reader/converter.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <cmath>
#include <limits>


// std::string Isotope::CSVHeader()
//{
//  return "A,Z,N,Symbol,DecayMode,ExperimentallyMeasured,Own,NUBASEMass,ErrorNUBASEMass,AMEMass,ErrorAMEMass,HalfLife,"
//         "SingleNeutronSeparationEnergy,ErrorSingleNeutronSeparationEnergy,SingleProtonSeparationEnergy,"
//         "ErrorSingleProtonSeparationEnergy,DoubleNeutronSeparationEnergy,ErrorDoubleNeutronSeparationEnergy,"
//         "DoubleProtonSeparationEnergy,ErrorDoubleProtonSeparationEnergy,DiscoveryYear";
//}


// std::string Isotope::writeAsCSV(std::string_view sep) const
//{
//  return fmt::format(
//      "{1}{0}{2}{0}{3}{0}{4}{0}{5}{0}{6}{0}{7}{0}{8}{0}{9}{0}{10}{0}{11:0.2e}{0}{12}{0}{13}{0}{14}{0}{15}{0}{"
//      "16}{0}{17}{0}{18}{0}{19}{0}{20}",
//      sep,
//      A,
//      Z,
//      N,
//      symbol,
//      decay,
//      exp,
//      NUBASE_ME,
//      NUBASE_dME,
//      AME_ME,
//      AME_dME,
//      hl.count(),
//      s_n,
//      ds_n,
//      s_p,
//      ds_p,
//      s_2n,
//      ds_2n,
//      s_2p,
//      ds_2p,
//      year);
//}


std::string Isotope::writeAsJSON(const bool human_readable) const
{
  return fmt::format("{{{0}"
                     "\"A\":{1},{0}"
                     "\"Z\":{2},{0}"
                     "\"N\":{3},{0}"
                     "\"Symbol\":\"{4}\",{0}"
                     "\"Decay\":\"{5}\",{0}"
                     "\"Experimental\":{6},{0}"
                     "\"NubaseMassExcess\":{7:.4f},{0}"
                     "\"ErrorNubaseMassExcess\":{8:.4f},{0}"
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
                     nubase.A,
                     nubase.Z,
                     nubase.N,
                     nubase.symbol,
                     nubase.decay,
                     nubase.exp,
                     nubase.mass_excess,
                     nubase.dmass_excess,
                     Converter::FloatToNdp(ame.mass_excess, NDP),
                     Converter::FloatToNdp(ame.dmass_excess, NDP),
                     nubase.hl.count(),
                     Converter::FloatToNdp(ame.s_n, NDP),
                     Converter::FloatToNdp(ame.ds_n, NDP),
                     Converter::FloatToNdp(ame.s_p, NDP),
                     Converter::FloatToNdp(ame.ds_p, NDP),
                     Converter::FloatToNdp(ame.s_2n, NDP),
                     Converter::FloatToNdp(ame.ds_2n, NDP),
                     Converter::FloatToNdp(ame.s_2p, NDP),
                     Converter::FloatToNdp(ame.ds_2p, NDP),
                     Converter::FloatToNdp(ame.binding_energy_per_A, NDP),
                     Converter::FloatToNdp(ame.dbinding_energy_per_A, NDP),
                     Converter::FloatToNdp(ame.atomic_mass, NDP),
                     Converter::FloatToNdp(ame.datomic_mass, NDP),
                     Converter::FloatToNdp(ame.beta_decay_energy, NDP),
                     Converter::FloatToNdp(ame.dbeta_decay_energy, NDP),
                     Converter::FloatToNdp(ame.q_a, NDP),
                     Converter::FloatToNdp(ame.dq_a, NDP),
                     Converter::FloatToNdp(ame.q_2bm, NDP),
                     Converter::FloatToNdp(ame.dq_2bm, NDP),
                     Converter::FloatToNdp(ame.q_ep, NDP),
                     Converter::FloatToNdp(ame.dq_ep, NDP),
                     Converter::FloatToNdp(ame.q_bm_n, NDP),
                     Converter::FloatToNdp(ame.dq_bm_n, NDP),
                     Converter::FloatToNdp(ame.q_4bm, NDP),
                     Converter::FloatToNdp(ame.dq_4bm, NDP),
                     Converter::FloatToNdp(ame.q_da, NDP),
                     Converter::FloatToNdp(ame.dq_da, NDP),
                     Converter::FloatToNdp(ame.q_pa, NDP),
                     Converter::FloatToNdp(ame.dq_pa, NDP),
                     Converter::FloatToNdp(ame.q_na, NDP),
                     Converter::FloatToNdp(ame.dq_na, NDP),
                     nubase.year);
}

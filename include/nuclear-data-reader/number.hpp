/**
 *
 * \class Number
 *
 * \brief Storage class for a value/measurement and it's uncertainty
 *
 * Here is a longer description
 */
#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <cmath>
#include <optional>

class Number
{
public:
  Number() = default;
  explicit Number(double _amount) : amount(_amount) {}
  // They are easily swappable, but sometimes that's just the way it is.
  // NOLINTNEXTLINE (bugprone-easily-swappable-parameters)
  Number(double _amount, double _uncertainty) : amount(_amount), uncertainty(_uncertainty) {}

  Number(const Number&)     = default;
  Number(Number&&) noexcept = default;

  Number& operator=(const Number&)     = default;
  Number& operator=(Number&&) noexcept = default;

  auto operator<=>(const Number& rhs) const { return (amount <=> rhs.amount); }

  ~Number() = default;


  // What is the recorded amount of the number
  // 'amount' is not a good name, but 'value' is already taken by std::optional
  mutable double amount{};
  // Is there an uncertainty associated with the number
  mutable std::optional<double> uncertainty{};

  /**
   * Calculate the relative uncertainty on the number if there is an uncertainty.
   * If there isn't an uncertainty, return an empty optional
   *
   * \param Nothing
   *
   * \return[std::optional<double>] Relative uncertainty as an optional
   */
  [[nodiscard]] inline auto relativeUncertainty() const
  {
    return uncertainty ? std::optional<double>{ std::fabs(uncertainty.value() / amount) } : uncertainty;
  }
};

#endif // NUMBER_HPP

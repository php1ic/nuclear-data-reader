/**
 *
 * \class Number
 *
 * \brief Storage class for a value/measurement and it's error
 *
 * Here is a longer description
 */
#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <optional>

class Number
{
public:
  Number(double _value, double _uncertainty) : value(_value), uncertainty(_uncertainty) {}
  explicit Number(double _value) : value(_value) {}

  Number(const Number&)     = default;
  Number(Number&&) noexcept = default;

  Number& operator=(const Number&)     = default;
  Number& operator=(Number&&) noexcept = default;

  auto operator<=>(const Number& rhs) const { return (value <=> rhs.value); }

  ~Number() = default;


  // What is the recorded value of the number
  mutable double value{};
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
    return uncertainty ? std::optional<double>{ uncertainty.value() / value } : uncertainty;
  }
};

#endif // NUMBER_HPP

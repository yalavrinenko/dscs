//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_COMPONENT_HPP
#define DSCS_COMPONENT_HPP
#include "../../utils/logger_factory.hpp"
#include <memory>

enum class component_type{
  engine,
  reactor,
  hull,
  fuel_tank,
  battery,
  radar,
  radio,
  none
};

class icomponent{
public:
  icomponent(double mass, std::string name, plogger logger, component_type type = component_type::none)
      : mass_(mass), logger_(std::move(logger)), name_(std::move(name)), type_(type) {}

  virtual void action() = 0;

  virtual void log_action() const = 0;

  [[nodiscard]]
  virtual double mass() const {
    return mass_;
  }

  auto const& name() const {return name_;};

  component_type type() const { return type_; }
protected:
  double mass_{};

  plogger& logger() const {
    return logger_;
  }

private:
  mutable plogger logger_ = nullptr;
  std::string name_;
  component_type type_;
};

using pcomponent = std::unique_ptr<icomponent>;
using pscomponent = std::shared_ptr<icomponent>;

template <unsigned num, unsigned den>
struct size_scale {
  static double constexpr scale = static_cast<double>(num) / static_cast<double>(den);
};

struct component_size{
  using tiny = size_scale<1, 4>;
  using small = size_scale<1, 2>;
  using medium = size_scale<1, 1>;
  using large = size_scale<2, 1>;
  using huge = size_scale<4, 1>;
};
#endif // DSCS_COMPONENT_HPP

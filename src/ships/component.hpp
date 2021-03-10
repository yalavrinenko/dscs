//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_COMPONENT_HPP
#define DSCS_COMPONENT_HPP
#include "../../utils/logger_factory.hpp"
#include <memory>
#include "utils/math_function.hpp"

enum class component_type{
  engine,
  reactor,
  hull,
  fuel_tank,
  battery,
  radar,
  radio,
  monitor_unit,
  warhead,
  cargo,
  weapon,
  none
};

class icomponent: public gui::igui_object{
public:
  icomponent(double mass, std::string name, plogger logger, component_type type = component_type::none)
      : gui::igui_object(std::move(logger.gui_window)), mass_(mass),
      logger_(std::move(logger.text_logger)),
      name_(std::move(name)), type_(type),
      loggers_{logger_, window_}
      {}

  virtual void action() = 0;

  virtual void log_action() const = 0;

  //void draw() override {}

  [[nodiscard]]
  virtual double mass() const {
    return mass_;
  }

  auto const& name() const {return name_;};

  component_type type() const { return type_; }

  virtual ~icomponent() = default;
protected:
  double mass_{};

  ptext_logger& logger() const {
    return logger_;
  }

  plogger& slogger() {
    return loggers_;
  }

private:
  mutable ptext_logger logger_ = nullptr;
  std::string name_;
  component_type type_;
  plogger loggers_;
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

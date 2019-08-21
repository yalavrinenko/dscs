//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_COMPONENT_HPP
#define DSCS_COMPONENT_HPP
#include <memory>
#include "../logger_factory.hpp"

class icomponent{
public:
  icomponent(double mass, std::string name, plogger &&logger)
      : mass_(mass), logger_(std::move(logger)), name_(std::move(name)) {}

  virtual void action() = 0;

  virtual void log_action() const = 0;

  [[nodiscard]]
  virtual double mass() const {
    return mass_;
  }

  plogger& logger() const {
    return logger_;
  }

  auto const& name() const {return name_;};
protected:
  double mass_{};

private:
  mutable plogger logger_ = nullptr;
  std::string name_;
};

using pcomponent = std::unique_ptr<icomponent>;
using pscomponent = std::shared_ptr<icomponent>;
#endif // DSCS_COMPONENT_HPP

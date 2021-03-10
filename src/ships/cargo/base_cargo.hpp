//
// Created by yalavrinenko on 17.02.2021.
//

#ifndef DSCS_BASE_CARGO_HPP
#define DSCS_BASE_CARGO_HPP
#include <list>
#include <ships/component.hpp>
class base_cargo: public icomponent {
public:
  base_cargo(double mass, std::string name,
             plogger logger, component_type type, double max_payload_mass);

  void action() override;
  void log_action() const override;
  void draw() override;

  bool push_payload(pcomponent &payload);

  auto count(component_type type) const {
    return std::ranges::count_if(payload_, [type](auto &e){ return e->type() == type; });
  }
  double mass() const override;

protected:
  std::list<pcomponent> payload_;

  double const max_payload_mass_;
};


#endif//DSCS_BASE_CARGO_HPP

//
// Created by yalavrinenko on 17.02.2021.
//

#ifndef DSCS_BASE_CARGO_HPP
#define DSCS_BASE_CARGO_HPP
#include <list>
#include <ships/component.hpp>
class base_cargo: public icomponent {
public:
  bool push_payload(pcomponent &payload);
  base_cargo(double mass, std::string name, plogger logger, component_type type, double max_payload_mass);

  void action() override;
  void log_action() const override;
  void draw() override;

protected:
  std::list<pcomponent> payload_;

  double const max_payload_mass_;
};


#endif//DSCS_BASE_CARGO_HPP

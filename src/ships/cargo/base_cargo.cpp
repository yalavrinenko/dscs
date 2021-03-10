//
// Created by yalavrinenko on 17.02.2021.
//

#include "base_cargo.hpp"

#include <utility>
base_cargo::base_cargo(double mass, std::string name, plogger logger, component_type type, double max_payload_mass)
    : icomponent(mass, std::move(name), std::move(logger), type), max_payload_mass_{max_payload_mass} {}

bool base_cargo::push_payload(pcomponent &payload) {
  if (this->mass() + payload->mass() <= max_payload_mass_) {
    payload_.push_back(std::move(payload));
    return true;
  } else {
    return false;
  }
}
void base_cargo::draw() {}
void base_cargo::action() {}
void base_cargo::log_action() const {}

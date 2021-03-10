//
// Created by yalavrinenko on 17.02.2021.
//

#include "base_cargo.hpp"
#include <ranges>
#include <algorithm>
#include <numeric>
#include <utils/gui_entries.hpp>
#include <utility>
base_cargo::base_cargo(double mass, std::string name, plogger logger, component_type type, double max_payload_mass)
    : icomponent(mass, std::move(name), std::move(logger), type), max_payload_mass_{max_payload_mass} {
  this->add_gui_entry<gui::numeric_entry>(this->name());
}

bool base_cargo::push_payload(pcomponent &payload) {
  if (this->mass() + payload->mass() <= max_payload_mass_) {
    payload_.push_back(std::move(payload));
    return true;
  } else {
    return false;
  }
}
void base_cargo::draw() {
  entry<gui::numeric_entry>()->log("Cargo mass:", mass(), max_payload_mass_);
  entry<gui::numeric_entry>()->log("Contains object:", payload_.size());

  //TODO: add count info by item types
}
void base_cargo::action() {}
void base_cargo::log_action() const {}
double base_cargo::mass() const {
  auto mass = std::accumulate(payload_.begin(), payload_.end(),0.0,
                          [](auto const& sum, auto const& e){
                            return sum + e->mass();
                          });
  return mass + icomponent::mass();
}
std::unique_ptr<icomponent> base_cargo::extract_payload(component_type ctype) {
  for (auto &p : payload_){
    if (p->type() == ctype){
      auto object = std::move(p);
      payload_.remove(p);

      return object;
    }
  }
  return nullptr;
}

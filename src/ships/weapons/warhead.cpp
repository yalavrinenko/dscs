//
// Created by yalavrinenko on 16.02.2021.
//

#include "warhead.hpp"
void warhead::action() {}
void warhead::log_action() const {}
double warhead::mass() const { return icomponent::mass(); }
void warhead::draw() {}
void warhead::explode() {
  if (!carrier_object_) {
    throw std::logic_error("Kabooom without carrier. Unable to determinate explosion point.");
  } else if (!call_explosion_) {
    throw std::logic_error("Kabooom without kaboom function. Unable to send explosion signal.");
  } else {
    call_explosion_(carrier_object_, explosion_radius_);
  }
}
//
// Created by yalavrinenko on 16.02.2021.
//

#include "NAR_M.hpp"
#include <ships/control/control_units/time_controller.hpp>
NAR_M::NAR_M(double ignite_at, double explode_at, std::string name, plogger logger)
    : missile(std::move(name), std::move(logger)), controller_{std::make_unique<timed_control>()} {
}

void NAR_M::arm() {
  this->add_control_unit(std::move(controller_));
}

NAR_M::~NAR_M() = default;

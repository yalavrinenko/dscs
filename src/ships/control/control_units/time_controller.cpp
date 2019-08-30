//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<pcontrol_event> timed_control::control(timestamp const &ts) {
  std::vector<pcontrol_event> events;
  if (ts == timestamp(100)) {
    auto start_event = std::make_unique<ship_events::reactor_start_event>("Master PU");
    events.emplace_back(std::move(start_event));
  }

  if (ts == timestamp(150)) {
    auto start_event = std::make_unique<ship_events::reactor_start_event>("Slave PU");
    events.emplace_back(std::move(start_event));
  }

  return events;
}

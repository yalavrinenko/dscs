//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<control_action> timed_control::control(timestamp const &ts,
                                      control_interface &ship) {
  if (!reactors.ready2use || !engines.ready2use || !payloads.ready2use)
    construct_component_groups(ship);

  return {};
}

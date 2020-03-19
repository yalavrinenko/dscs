//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<control_action> timed_control::control(timestamp const &ts,
                                      control_interface &ship) {
  auto ignite_ts = timestamp{50};

  if (!reactors.ready2use || !engines.ready2use)
    construct_component_groups(ship);

  if (ts == ignite_ts){
    auto ignite = [](preactor &reactor){
      reactor->ignite();
    };
    reactors.apply(ignite);
  }

  return {};
}

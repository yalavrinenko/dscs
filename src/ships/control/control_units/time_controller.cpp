//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<control_action> timed_control::control(timestamp const &ts,
                                      control_interface &ship) {
  auto ignite_ts_1 = timestamp{50};
  auto ignite_ts_2 = timestamp{400};

  if (!reactors.ready2use || !engines.ready2use)
    construct_component_groups(ship);

  auto ignite = [](preactor &reactor){
    reactor->ignite();
  };

  if (ts == ignite_ts_1){
    reactors.devices[0]->ignite();
  }

  if (ts == ignite_ts_2){
    reactors.devices[1]->ignite();
  }


  return {};
}

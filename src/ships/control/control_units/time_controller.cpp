//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<control_action> timed_control::control(timestamp const &ts,
                                      control_interface &ship) {
  if (!reactors.ready2use || !engines.ready2use || !payloads.ready2use)
    construct_component_groups(ship);

  if (begin_ == timestamp{})
    begin_ = ts;

  std::vector<control_action> actions{};

  while (!queue_.empty() && queue_.top().first == ts - begin_){
    auto action = queue_.top().second;

    auto act_func = [this, action](timestamp const& ts) {
      action(engines, reactors, payloads);
    };
    actions.emplace_back(act_func, ts);

    queue_.pop();
  }

  return actions;
}
void timed_control::add_action(timestamp ts, timed_control::action_function action) {
  queue_.emplace(ts, std::move(action));
}

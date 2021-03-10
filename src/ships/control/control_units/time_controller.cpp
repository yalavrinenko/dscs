//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<control_action> timed_control::control_impl(timestamp const &ts,
                                      control_interface &ship) {
  if (begin_ == timestamp{})
    begin_ = ts;

  std::vector<control_action> actions{};

  while (!queue_.empty() && queue_.top().first == ts - begin_){
    auto action = queue_.top().second;

    auto act_func = [this, action](timestamp const& ts) {
      perform_action(action);
    };
    actions.emplace_back(act_func, ts);

    queue_.pop();
  }

  return actions;
}
void timed_control::add_action(timestamp ts, time_action_function action) {
  queue_.emplace(ts, std::move(action));
}

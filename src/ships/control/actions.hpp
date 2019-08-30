//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_ACTIONS_HPP
#define DSCS_ACTIONS_HPP

#include <functional>
#include "../../common.hpp"
#include <queue>

class control_action{
public:
  using action_fuction = std::function<void(timestamp const &)>;

  explicit control_action(action_fuction action): action_(std::move(action)) {}

  control_action(action_fuction action, timestamp const &ts): action_(std::move(action)),
                                                       action_step(ts){}

  void run(timestamp const &ts) {
    action_(ts);
  }

  [[nodiscard]] timestamp const& step() const{
    return action_step;
  }

  bool operator< (control_action const &a) const {
    return this->step() < a.step();
  }

  void set_timestamp(timestamp const &ts){
    action_step = ts;
  }
protected:
  action_fuction action_;
  timestamp action_step;
};

class action_queue{
public:
  bool invoke_next(timestamp const &ts);

  size_t push_actions(std::vector<control_action> const &actions){
    auto count = 0;
    std::for_each(actions.begin(), actions.end(), [&count, this](control_action const &a){
      queue_.push(a);
      ++count;
    });
    return count;
  }
protected:
  std::priority_queue<control_action> queue_;
};


#endif // DSCS_ACTIONS_HPP

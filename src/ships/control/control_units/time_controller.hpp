//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_TIME_CONTROLLER_HPP
#define DSCS_TIME_CONTROLLER_HPP

#include "../icontrol.hpp"

class timed_control: public icontrol{
public:
  using action_function =
      std::function<void(icontrol::component_group<pengine> &engines, icontrol::component_group<preactor> &reactor,
                         component_group<std::shared_ptr<warhead>> &payloads)>;

  struct queue_cmp {
    bool operator()(std::pair<timestamp, timed_control::action_function> const &lhs,
                    std::pair<timestamp, timed_control::action_function> &rhs) const {
      return rhs.first < lhs.first;
    }
  };

  std::vector<control_action> control(timestamp const &ts, control_interface &ship) override;

  void add_action(timestamp ts, action_function action);

private:
  std::priority_queue<std::pair<timestamp, action_function>, std::vector<std::pair<timestamp, action_function>>,
                      queue_cmp> queue_;
  timestamp begin_{};
};

#endif // DSCS_TIME_CONTROLLER_HPP

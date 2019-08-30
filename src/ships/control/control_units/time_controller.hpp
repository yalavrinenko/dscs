//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_TIME_CONTROLLER_HPP
#define DSCS_TIME_CONTROLLER_HPP

#include "../icontrol.hpp"

class timed_control: public icontrol{
public:
  std::vector<pcontrol_event> control(timestamp const &ts) override;
};

#endif // DSCS_TIME_CONTROLLER_HPP

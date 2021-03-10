//
// Created by yalavrinenko on 06.11.2019.
//

#ifndef DSCS_REMOTE_CONTROL_HPP
#define DSCS_REMOTE_CONTROL_HPP

#include "../../control/icontrol.hpp"
#include "../../detection/communications.hpp"

class radio_control_unit: public icontrol{
public:
  explicit radio_control_unit(std::shared_ptr<radio_unit> radio, control_interface &ship): icontrol(ship), radio_(std::move(radio)){}

protected:
  std::vector<control_action> control_impl(timestamp const &ts, control_interface &ship) override;
private:
  std::shared_ptr<radio_unit> radio_;
  timestamp t;
  bool is_first = true;
};

#endif // DSCS_REMOTE_CONTROL_HPP

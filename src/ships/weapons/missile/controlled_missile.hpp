//
// Created by yalavrinenko on 06.11.2019.
//

#ifndef DSCS_CONTROLLED_MISSILE_HPP
#define DSCS_CONTROLLED_MISSILE_HPP

#include "../../hull.hpp"
#include "../controls/remote_control.hpp"
#include "../../engines/engine.hpp"

class missile: public ship_hull{
public:
  missile(double comm_freq, size_t uid, std::string name, plogger logger);

  bool charge(wire &source);

  bool refule(fuel_pipe &source);

  vector_2d force(timestamp const &time) override;

  double mass() const override;

private:

  std::shared_ptr<radio_unit> radio_;
  std::shared_ptr<engine> engine_;
  pbattery_line battery_;
  pfuel_tank_line fuel_;

  size_t uid{};
};
#endif // DSCS_CONTROLLED_MISSILE_HPP

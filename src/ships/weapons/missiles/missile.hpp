//
// Created by yalavrinenko on 16.02.2021.
//

#ifndef DSCS_MISSILE_HPP
#define DSCS_MISSILE_HPP


#include "../../hull.hpp"
#include "../controls/remote_control.hpp"
#include "../../engines/engine.hpp"

class warhead;

class missile: public ship_hull{
public:
  missile(std::string name, plogger logger);

  bool charge(wire &source);

  double fule() const { return fuel_->percent_load(); }
  double charge() const { return battery_->percent_load(); }

  bool refule(fuel_pipe &source);

  vector_2d force(timestamp const &time) override;

  void setup_warhead(std::unique_ptr<warhead> warhead);

  virtual void arm() = 0;

private:
  std::shared_ptr<engine> engine_;
  std::shared_ptr<warhead> warhead_;

  std::unique_ptr<icontrol> control_;

  pbattery_line battery_;
  pfuel_tank_line fuel_;
};


#endif//DSCS_MISSILE_HPP

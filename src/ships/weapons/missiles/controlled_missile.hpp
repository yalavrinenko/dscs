//
// Created by yalavrinenko on 06.11.2019.
//

#ifndef DSCS_CONTROLLED_MISSILE_HPP
#define DSCS_CONTROLLED_MISSILE_HPP
//
//#include "../../hull.hpp"
//#include "../controls/remote_control.hpp"
//#include "../../engines/engine.hpp"
//
//class warhead;
//
//class missiles: public ship_hull{
//public:
//  missiles(double comm_freq, size_t uid, std::string name, plogger logger);
//
//  bool charge(wire &source);
//
//  bool refule(fuel_pipe &source);
//
//  vector_2d force(timestamp const &time) override;
//  void draw() override;
//  void action() override;
//  void log_action() const override;
//  void update(const timestamp &time) override;
//  double mass() const override;
//
//  void setup_warhead(std::unique_ptr<warhead> warhead);
//
//private:
//
//  std::shared_ptr<radio_unit> radio_;
//  std::shared_ptr<engine> engine_;
//  std::unique_ptr<warhead> warhead_;
//
//  pbattery_line battery_;
//  pfuel_tank_line fuel_;
//
//  size_t uid{};
//};
#endif // DSCS_CONTROLLED_MISSILE_HPP

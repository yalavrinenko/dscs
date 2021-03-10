//
// Created by yalavrinenko on 16.02.2021.
//

#include "NAR_M.hpp"
#include <ships/control/control_units/time_controller.hpp>
NAR_M::NAR_M(double ignite_at, double explode_at, std::string name, plogger logger)
    : missile(std::move(name), std::move(logger)), controller_{std::make_unique<timed_control>()} {

  auto ignite_ts = timestamp(ignite_at / timestamp::delta());
  auto explode_ts = timestamp(explode_at / timestamp::delta());

  auto ignite = [name = this->name()](icontrol::component_group<pengine> &eng){
    std::clog << "Ignite engine in missile " << name << std::endl;
    eng.apply([](pengine &eng) { eng->ignite(); eng->set_max_thrust(1.0); });
  };

  auto explode = [name = this->name()](icontrol::component_group<pwarhead> &wh){
    std::clog << "Explode warhead in missile " << name << std::endl;
    wh.apply([](pwarhead &wh) { wh->explode(); });
  };

  controller_->add_action(ignite_ts, ignite);
  controller_->add_action(explode_ts, explode);
}

void NAR_M::arm() {
  this->add_control_unit(std::move(controller_));
}

NAR_M::~NAR_M() = default;

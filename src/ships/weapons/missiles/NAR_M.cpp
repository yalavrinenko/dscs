//
// Created by yalavrinenko on 16.02.2021.
//

#include "NAR_M.hpp"
#include <ships/control/control_units/time_controller.hpp>
NAR_M::NAR_M(std::string name, plogger logger)
    : missile(std::move(name), std::move(logger)), controller_{std::make_unique<timed_control>()} {
}

void NAR_M::arm() {
  this->add_control_unit(std::move(controller_));
}

void NAR_M::set_flight_parameter(flight_parameters const& opts) {
  auto ignite_ts = timestamp(static_cast<size_t>(opts.ignition_time / timestamp::delta()));
  auto explode_ts = timestamp(static_cast<size_t>(opts.explosion_time / timestamp::delta()));

  auto align_ts = timestamp(1);

  auto align = [name = this->name(), angle = opts.alignment_vector.in_polar().y](icontrol::component_group<pengine> &eng){
    std::clog << "Align engine in missile " << name << std::endl;
    eng.apply([angle](pengine &eng) {
      eng->align(angle);
    });
  };

  auto ignite = [name = this->name()](icontrol::component_group<pengine> &eng){
    std::clog << "Ignite engine in missile " << name << std::endl;
    eng.apply([](pengine &eng) { eng->ignite(); eng->set_max_thrust(1.0); });
  };

  auto explode = [name = this->name()](icontrol::component_group<pwarhead> &wh){
    std::clog << "Explode warhead in missile " << name << std::endl;
    wh.apply([](pwarhead &wh) { wh->explode(); });
  };

  controller_->add_action(align_ts, align);
  controller_->add_action(ignite_ts, ignite);
  controller_->add_action(explode_ts, explode);
}

NAR_M::~NAR_M() = default;

//
// Created by yalavrinenko on 17.02.2021.
//

#include "launcher.hpp"
#include <ships/cargo/base_cargo.hpp>
#include <utils/launcher_entry.hpp>

#include <utility>
launcher::launcher(double mass, std::string name, plogger logger, component_type type, wire power, fuel_pipe pipe,
                   size_t launch_pads)
    : icomponent(mass, std::move(name), std::move(logger), type), power_(std::move(power)),
      fuel_(std::move(pipe)), launch_pads_{launch_pads} {
  cargo_ = std::make_unique<base_cargo>(0.1, this->name() + ": cargo", slogger(), component_type::cargo, 10.0);

  add_gui_entry<gui::text_entry>(this->name());
  add_gui_entry<gui::numeric_entry>(this->name());
  add_gui_entry<gui::launcher_entry>(this->name(),
                                     gui::launcher_entry::callback_set{nullptr,
                                                                       nullptr,
                                                                       nullptr,
                                                                       nullptr,
                                                                       nullptr});
}

void launcher::action() {}

void launcher::log_action() const {}

void launcher::draw() {
  auto text_gui = entry<gui::text_entry>();
  auto num_gui = entry<gui::numeric_entry>();

  num_gui->log("Power consumption", power_consumption_);
  num_gui->log("Fuel flow", fuel_consumption_);

  gui::launcher_log_data log_data;
  log_data.total = loaded_.size() + ((cargo_) ? cargo_->count(component_type::hull) : 0);
  for (auto &pl : loaded_){
    log_data.loaded.emplace_back(gui::launcher_log_data::missile_info{
        .fuel = pl.missile_ptr->fule(),
        .charge = pl.missile_ptr->charge(),
        .warhead = pl.is_armed,
        .name = pl.missile_ptr->name(),
        .lock_target = pl.is_locked,
        .ready2fire = pl.is_ready2fire,
    });
  }
  entry<gui::launcher_entry>()->log(log_data);
}

void launcher::connect_cargo(std::shared_ptr<base_cargo> cargo) {
  cargo_ = std::move(cargo);
}

launcher::~launcher() = default;

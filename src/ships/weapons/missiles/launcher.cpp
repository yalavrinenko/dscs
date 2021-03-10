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
                                     gui::launcher_entry::callback_set{.on_arm = nullptr,
                                                                       .on_lock = nullptr,
                                                                       .on_launch = nullptr,
                                                                       .on_disarm = nullptr,
                                                                       .on_load = [this](int) {
                                                                         if (this->loaded_.size() < this->launch_pads_)
                                                                           this->load_from_cargo();
                                                                       }
                                     });
}

void launcher::action() {
  for (auto &m : loaded_){
    if (m.next_action == missile_action::load){
      auto req = (1.0 - m.load_progress) * LauncherOperationConstant::charge4load() * LauncherOperationConstant::income_current_reduction();
      auto get = power_.pull(req);
      m.load_progress += get / req * LauncherOperationConstant::income_current_reduction();
      if (m.load_progress >= 1){
        auto extracted = Utils::dynamic_unique_cast<missile>(cargo_->extract_payload(component_type::hull));
        if (extracted){
          m.missile_ptr = std::move(extracted);
        }
        m.next_action = missile_action::idle;
      }
    }
  }
}

void launcher::log_action() const {}

void launcher::draw() {
  auto text_gui = entry<gui::text_entry>();
  auto num_gui = entry<gui::numeric_entry>();

  num_gui->log("Power consumption", power_consumption_);
  num_gui->log("Fuel flow", fuel_consumption_);

  gui::launcher_log_data log_data;
  log_data.total = loaded_.size() + ((cargo_) ? cargo_->count(component_type::hull) : 0);
  for (auto &pl : loaded_) {
    log_data.loaded.emplace_back(gui::launcher_log_data::missile_info{
        .fuel = (pl.missile_ptr) ? pl.missile_ptr->fule() : 0,
        .charge = (pl.missile_ptr) ? pl.missile_ptr->charge() : 0,
        .warhead = pl.is_armed,
        .name = (pl.missile_ptr) ? pl.missile_ptr->name() : std::string{},
        .lock_target = pl.is_locked,
        .ready2fire = pl.is_ready2fire,
        .load_progress = pl.load_progress
    });
  }
  entry<gui::launcher_entry>()->log(log_data);
}

void launcher::connect_cargo(std::shared_ptr<base_cargo> cargo) { cargo_ = std::move(cargo); }
void launcher::load_from_cargo() {
  this->loaded_.emplace_back(missile_info{
      .missile_ptr = nullptr,
      .is_armed = false,
      .is_locked = false,
      .is_ready2fire = false,
      .next_action = missile_action::load,
      .load_progress = 0.0
  });
}

launcher::~launcher() = default;

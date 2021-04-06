//
// Created by yalavrinenko on 17.02.2021.
//

#include "launcher.hpp"
#include <ships/cargo/base_cargo.hpp>
#include <utils/launcher_entry.hpp>

#include <ships/weapons/missiles/NAR_M.hpp>

#include <utility>
#include <ships/control/command_unit.hpp>

auto launcher::gui_calbacks() {
  auto arm = [this](int m) {
    std::clog << "Arm " << m << " object\n";
    loaded_[m].next_action = missile_action::charge;
  };

  auto lock = [this](size_t target) {
    if (this->loaded_[target].is_armed)
      this->loaded_[target].next_action = missile_action::lock;
  };

  auto load = [this](size_t){
    if (this->loaded_.size() < this->launch_pads_) {
      this->loaded_.emplace_back(missile_info{.missile_ptr = nullptr,
                                              .is_armed = false,
                                              .is_locked = false,
                                              .is_ready2fire = false,
                                              .next_action = missile_action::load,
                                              .load_progress = 0.0});
    }
  };

  auto disarm = [this](size_t target) {
    if (this->loaded_[target].is_armed) this->loaded_[target].next_action = missile_action::unlock;
  };

  auto launch = [this](size_t target){
    if (loaded_[target].is_ready2fire)
      loaded_[target].next_action = missile_action::fire;
  };

  gui::launcher_entry::callback_set cbs{.on_arm = arm,
                                        .on_lock = lock,
                                        .on_launch = launch,
                                        .on_disarm = disarm,
                                        .on_load = load};

  return cbs;
}

launcher::launcher(double mass, std::string name, plogger logger, component_type type, wire power, fuel_pipe pipe,
                   size_t launch_pads)
    : icomponent(mass, std::move(name), std::move(logger), type), power_(std::move(power)),
      fuel_(std::move(pipe)), launch_pads_{launch_pads} {
  //cargo_ = std::make_unique<base_cargo>(0.1, this->name() + ": cargo", slogger(), component_type::cargo, 10.0);

  add_gui_entry<gui::text_entry>(this->name());
  add_gui_entry<gui::numeric_entry>(this->name());
  add_gui_entry<gui::launcher_entry>(this->name(), gui_calbacks());
  init_actions();
}

void launcher::action() {
  std::ranges::for_each(loaded_, [this](auto &m) { main_actions_[m.next_action](m); });
}

void launcher::log_action() const {}

void launcher::draw() {
  auto text_gui = entry<gui::text_entry>();
  auto num_gui = entry<gui::numeric_entry>();

  num_gui->log("Power consumption", power_consumption_);
  num_gui->log("Fuel flow", fuel_consumption_);

  gui::launcher_log_data log_data;
  log_data.total = ((cargo_) ? cargo_->count(component_type::hull) : 0);
  for (auto i = 0ul; auto &pl : loaded_) {
    log_data.loaded.emplace_back(gui::launcher_log_data::missile_info{
        .fuel = (pl.missile_ptr) ? pl.missile_ptr->fule() : 0,
        .charge = (pl.missile_ptr) ? pl.missile_ptr->charge() : 0,
        .warhead = pl.is_armed,
        .name = (pl.missile_ptr) ? pl.missile_ptr->name() : std::string{},
        .callback_index = i++,
        .lock_target = pl.is_locked,
        .ready2fire = pl.is_ready2fire,
        .load_progress = pl.load_progress,
    });
  }
  entry<gui::launcher_entry>()->log(log_data);
}

void launcher::connect_cargo(std::shared_ptr<base_cargo> cargo) { cargo_ = std::move(cargo); }
void launcher::connect_command_unit(std::shared_ptr<command_unit> &unit) {
  cunit_ = unit;
}

auto launcher::evaluate_approaching_time(auto const& target) {
  auto target_pos = target.last_position();

  return target_pos.r() / NAR_M::missile_parameters::peak_velocity;
}

void launcher::init_actions() {
  main_actions_[missile_action::load] = [this](auto &m){
    auto req = (1.0 - m.load_progress) * LauncherOperationConstant::charge4load() *
               LauncherOperationConstant::income_current_reduction();
    auto get = power_.pull(req);
    m.load_progress += get / req * LauncherOperationConstant::income_current_reduction();
    if (m.load_progress >= 1) {
      auto extracted = Utils::dynamic_unique_cast<missile>(cargo_->extract_payload(component_type::hull));
      if (extracted) { m.missile_ptr = std::move(extracted); }
      m.next_action = missile_action::idle;
    }
  };

  main_actions_[missile_action::charge] = [this](auto &m){
    if (m.missile_ptr->charge(power_)) m.next_action = missile_action::refuel;
  };

  main_actions_[missile_action::refuel] = [this](auto &m) {
    if (m.missile_ptr->refule(fuel_)) {
      auto wh = Utils::dynamic_unique_cast<warhead>(cargo_->extract_payload(component_type::warhead));
      if (wh) {
        wh->set_carrier_object(m.missile_ptr.get());
        m.missile_ptr->setup_warhead(std::move(wh));
      }
      m.next_action = missile_action::idle;
      m.is_armed = true;
    }
  };

  main_actions_[missile_action::lock] = [this](auto &m){
    if (cunit_->target_selected()) {

      m.lock_info.ignite_time = NAR_M::missile_parameters::ignite_time;
      m.lock_info.explode_time = evaluate_approaching_time(cunit_->target());
      m.lock_info.target_position = cunit_->target().last_position();
      auto vvec = cunit_->target().last_position().in_cartesian();
      vvec.norm();
      m.lock_info.target_position = vvec * 0.2;

      m.is_locked = true;
      m.is_ready2fire = true;
    }
    m.next_action = missile_action::idle;
  };

  main_actions_[missile_action::unlock] = [](auto &m) {
    m.is_locked = false;
    m.is_ready2fire = false;
  };

  main_actions_[missile_action::idle] = [](auto &m) {};

  main_actions_[missile_action::fire] = [](missile_info &m) {
    if (m.is_ready2fire){
      auto *nar_ptr = dynamic_cast<NAR_M*>(m.missile_ptr.get());
      nar_ptr->set_flight_parameter(m.lock_info.ignite_time, m.lock_info.explode_time);
      nar_ptr->arm();


    }
  };
}

launcher::~launcher() = default;

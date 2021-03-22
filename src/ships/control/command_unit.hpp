//
// Created by yalavrinenko on 24.04.2020.
//

#ifndef DSCS_COMMAND_UNIT_HPP
#define DSCS_COMMAND_UNIT_HPP
#include <src/ships/component.hpp>
#include <src/ships/detection/radar.hpp>
#include <src/ships/detection/object_tracker.hpp>
#include <utils/radar_entry.hpp>
#include <utility>
#include <stack>

struct CommandUnitConstanst{
  static constexpr auto CHARGE_FOR_LOCK = 1.0;
  static constexpr auto CHARGE_FOR_TRACK = 10.0;
};

class command_unit : public icomponent {
public:
  command_unit(wire power, double mass, std::string name, plogger logger, component_type type)
      : icomponent(mass, std::move(name), std::move(logger), type), power_(std::move(power)) {
    gui.numeric_log_ = add_gui_entry<gui::numeric_entry>(this->name());
  }

  void action() override;

  void log_action() const override {}

  void draw() override;

  void register_long_range_radar(auto radar) {
    long_range_radar_ = std::move(radar);
    gui.long_range_radar_log_ = add_gui_entry<gui::radar_entry>(
        this->name(), long_range_radar_->range() * PhysUnit::EM_SPEED(), long_range_radar_->range());

    gui.long_range_radar_log_->set_callback([this](double r, double phi){
      lock_stack_.push(polar_vector{r, phi});
    });
  }

protected:
  std::shared_ptr<long_range_radar<radio_unit, radio_transmitter_option>> long_range_radar_ = nullptr;
  object_tracker tracker_;

  struct {
    std::shared_ptr<gui::numeric_entry> numeric_log_ = nullptr;
    std::shared_ptr<gui::radar_entry> long_range_radar_log_ = nullptr;
  } gui;

  std::stack<polar_vector> lock_stack_;

  timestamp inner_ts_;
  wire power_;
};
#endif//DSCS_COMMAND_UNIT_HPP

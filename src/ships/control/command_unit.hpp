//
// Created by yalavrinenko on 24.04.2020.
//

#ifndef DSCS_COMMAND_UNIT_HPP
#define DSCS_COMMAND_UNIT_HPP
#include <src/ships/component.hpp>
#include <src/ships/detection/radar.hpp>
#include <utility>

class command_unit : public icomponent {
public:
  command_unit(double mass, std::string name, plogger logger, component_type type)
      : icomponent(mass, std::move(name), std::move(logger), type) {
    gui.numeric_log_ = add_gui_entry<gui::numeric_entry>(this->name());
  }

  void action() override {}

  void log_action() const override {}

  void draw() override {
    std::remove_cv_t<std::remove_reference_t<decltype(long_range_radar_->detected_points())>> data{};
    if (long_range_radar_) {
      data = long_range_radar_->detected_points();
      gui.long_range_radar_log_->change_range(long_range_radar_->range() * PhysUnit::EM_SPEED(), long_range_radar_->range());
    }

    auto total_target_ = 0;
    for (auto &[k, v] : data) {
      for (auto &point : v) {
        gui.long_range_radar_log_->log({point.position.x, point.position.y, std::to_string(point.range_circle)});
        ++total_target_;
      }
    }

    gui.numeric_log_->log("Target detected count:", total_target_);
    gui.numeric_log_->log("Target tracked count:", 0);
  }

  void register_long_range_radar(auto radar) {
    long_range_radar_ = std::move(radar);
    gui.long_range_radar_log_ = add_gui_entry<gui::radar_entry>(
        this->name(), long_range_radar_->range() * PhysUnit::EM_SPEED(), long_range_radar_->range());
  }

protected:
  std::shared_ptr<long_range_radar<radio_unit, radio_transmitter_option>> long_range_radar_ = nullptr;

  struct {
    std::shared_ptr<gui::numeric_entry> numeric_log_ = nullptr;
    std::shared_ptr<gui::radar_entry> long_range_radar_log_ = nullptr;
  } gui;
};
#endif//DSCS_COMMAND_UNIT_HPP

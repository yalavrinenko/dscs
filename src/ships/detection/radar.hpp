//
// Created by yalavrinenko on 22.10.2019.
//

#ifndef DSCS_RADAR_HPP
#define DSCS_RADAR_HPP

#include "communications.hpp"
#include <unordered_map>

struct radar_data{
  size_t range_circle{};
  vector_2d position{};
};

template <class __transmission_device, class __transmission_option>
class long_range_radar : public icomponent{
public:
  long_range_radar(double mass, size_t range,
                   __transmission_option option, em_field &field,
                   std::string name, plogger logger): icomponent(mass, std::move(name), std::move(logger)),
                   device_{1.0, std::move(option), field},
                   range_{range}{
    add_gui_entry<gui::numeric_entry>(this->name());
    add_gui_entry<gui::moving_plot_entry>(this->name(), 100);
    add_gui_entry<gui::radar_entry>(this->name(), range_ * PhysUnit::EM_SPEED(), range_);

    num_log_ = entry<gui::numeric_entry>(0);
    plot_log_ = entry<gui::moving_plot_entry>(1);
    radar_log_ = entry<gui::radar_entry>(2);
  }

  void draw() override {
    auto nlog  = entry<gui::numeric_entry>();
    num_log_->log("Working frequency", device_.linked_frequency());
    num_log_->log("Target detected count:", data_.size());
    num_log_->log("Target tracked count:", 0);
    plot_log_->log("Power consumption", device_.power_consumption());

    for (auto &[k, v]: data_){
      for (auto &point: v)
        radar_log_->log({point.position.x, point.position.y, std::to_string(point.range_circle)});
    }
  }

  void action() override{
    device_.action();

    if (is_active()) {
      if (timer_ == 0) {
        device_.send_message("", range_);
      } else {
        data_[timer_].clear();
      }

      auto &received_data = device_.received_messages();
      std::for_each(received_data.begin(), received_data.end(),
                    [this](field_package const &p) {
                      if (!p.is_origin())
                        data_[timer_].emplace_back(radar_data{timer_, p.source_location()});
                    });

      timer_ = (timer_ + 1) % (2 * range_);
      device_.flush();
    }
  }

  void log_action() const override{
    logger()->log("Power consumption:", device_.power_consumption());
  }

  double mass() const override { return icomponent::mass() + device_.mass(); }

  size_t& range() { return range_; }

  bool is_active() const { return is_active_; }

  void activate() const { return is_active_ = true; }
  void deactivate() const { return is_active_ = false; }

protected:
  std::unordered_map<size_t, std::vector<radar_data>> data_;

  std::shared_ptr<gui::moving_plot_entry> plot_log_;
  std::shared_ptr<gui::numeric_entry> num_log_;
  std::shared_ptr<gui::radar_entry> radar_log_;

  __transmission_device device_;
  size_t range_;

  size_t timer_{0};

  bool is_active_ {true};
};

#endif // DSCS_RADAR_HPP

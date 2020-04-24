//
// Created by yalavrinenko on 22.10.2019.
//

#ifndef DSCS_RADAR_HPP
#define DSCS_RADAR_HPP

#include "communications.hpp"
#include <unordered_map>
#include <utils/gui_input.hpp>

struct radar_data{
  size_t range_circle{};
  vector_2d position{};
};

template <class _transmission_device, class _transmission_option>
class long_range_radar : public icomponent{
public:
  long_range_radar(double mass, size_t range,
                   _transmission_option option, em_field &field,
                   std::string name, plogger logger): icomponent(mass, std::move(name), std::move(logger)),
                   device_{1.0, std::move(option), field},
                   range_{range}{
    add_gui_entry<gui::numeric_entry>(this->name());
    add_gui_entry<gui::moving_plot_entry>(this->name(), 100);

    num_log_ = entry<gui::numeric_entry>(0);
    plot_log_ = entry<gui::moving_plot_entry>(1);

    register_control_actions();
  }

  void draw() override {
    auto nlog  = entry<gui::numeric_entry>();
    //(*num_log_)::(gui::text_entry).log
    num_log_->log("Working frequency", device_.linked_frequency());
    plot_log_->log("Power consumption", device_.power_consumption());
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

  void activate() { is_active_ = true; }
  void deactivate() { is_active_ = false; data_.clear(); }

  void change_range(size_t range) { range_ = range; timer_ = 0; data_.clear(); }

  auto const& detected_points() const{
    return data_;
  }

protected:
  void register_control_actions();

  std::unordered_map<size_t, std::vector<radar_data>> data_;

  std::shared_ptr<gui::moving_plot_entry> plot_log_;
  std::shared_ptr<gui::numeric_entry> num_log_;

  _transmission_device device_;
  size_t range_;

  size_t timer_{0};

  bool is_active_ {true};
};
template<class _transmission_device, class _transmission_option>
void long_range_radar<_transmission_device, _transmission_option>::register_control_actions() {
  auto controls = this->add_gui_entry<gui::gui_controls>(this->name());

  auto activate = [this](auto &obj) { this->activate(); };
  auto shutdown = [this](auto &obj) { this->deactivate(); };

  auto button_group = std::make_unique<gui::button_group<gui::button_control, 2>>(
      std::initializer_list < gui::button_control>{
          {"Activate", activate},
          {"Deactivate", shutdown}});
  controls->add_control(std::move(button_group));
}

#endif // DSCS_RADAR_HPP

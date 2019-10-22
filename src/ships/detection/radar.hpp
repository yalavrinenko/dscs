//
// Created by yalavrinenko on 22.10.2019.
//

#ifndef DSCS_RADAR_HPP
#define DSCS_RADAR_HPP

#include "communications.hpp"

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
  }

  void action() override{
    device_.action();

    if (is_active()) {
      if (timer_ == 0) {
        device_.send_message("", range_);
      }
      if (timer_ == 1){
        data_.clear();
      }

      auto &received_data = device_.received_messages();
      std::for_each(received_data.begin(), received_data.end(),
                    [this](field_package const &p) {
                      if (!p.is_origin())
                        data_.emplace_back(radar_data{timer_, p.source_location()});
                    });

      timer_ = (timer_ + 1) % (2 * range_);
      device_.flush();
    }
  }

  void log_action() const override{
    logger()->log("Power consumption:", device_.power_consumption());
    logger()->log("Targets:");
    logger()->up_level();
    for (auto &o : data_){
      logger()->log(o.range_circle, o.position.x, o.position.y);
    }
    logger()->down_level();
  }

  double mass() const override { return icomponent::mass() + device_.mass(); }

  size_t& range() { return range_; }

  bool is_active() const { return is_active_; }

  void activate() const { return is_active_ = true; }
  void deactivate() const { return is_active_ = false; }

protected:
  std::vector<radar_data> data_;

  __transmission_device device_;
  size_t range_;

  size_t timer_{0};

  bool is_active_ {true};
};

#endif // DSCS_RADAR_HPP

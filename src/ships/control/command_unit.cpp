//
// Created by yalavrinenko on 24.04.2020.
//

#include "command_unit.hpp"
void command_unit::draw() {
  std::remove_cv_t<std::remove_reference_t<decltype(long_range_radar_->detected_points())>> data{};
  if (long_range_radar_) {
    data = long_range_radar_->detected_points();
    gui.long_range_radar_log_->change_range(long_range_radar_->range() * PhysUnit::EM_SPEED(),
                                            long_range_radar_->range());
  }

  auto total_target_ = 0;
  for (auto &[k, v] : data) {
    for (auto &point : v) {
      gui.long_range_radar_log_->log({point.position.x, point.position.y});
      ++total_target_;
    }
  }

  for (auto &tp : tracker_.tracked()) {
    std::array<std::pair<double, double>, gui::radar_entry::tracked_point::TRAJECTORY_POINTS> prediction;
    auto tmp_p = tp.make_trajectory_prediction(prediction.size());
    std::ranges::copy(tmp_p, prediction.begin());

    gui::radar_entry::tracked_point p{.description = "Tracked object " + std::to_string(tp.uid()),
                                      .uid = tp.uid(),
                                      .r = tp.last_position().r(),
                                      .phi = tp.last_position().phi(),
                                      .velocity = tp.velocity().value_or(polar_vector{0.0, 0.0}),
                                      .acceleration = tp.acceleration().value_or(polar_vector{0.0, 0.0}),
                                      .lock_target = (current_lock_target_) && tp.uid() == *current_lock_target_,
                                      .trajectory = std::move(prediction)};

    gui.long_range_radar_log_->log(p);
  }

  gui.numeric_log_->log("Target detected count:", total_target_);
  gui.numeric_log_->log("Target tracked count:", tracker_.tracked().size());
}
void command_unit::action() {
  inner_ts_ = inner_ts_ + 1;

  if (long_range_radar_) {
    auto points = long_range_radar_->detected_points();
    std::vector<polar_vector> pv;
    auto insert_function = [&pv](auto const &kv) {
      for (auto const &o : kv.second) pv.push_back(o.position);
    };
    std::ranges::for_each(points, insert_function);

    tracker_.update_map(std::move(pv), inner_ts_);
  }

  while (!lock_stack_.empty()) {
    auto p = lock_stack_.top();
    lock_stack_.pop();
    auto get = power_.pull(CommandUnitConstanst::CHARGE_FOR_LOCK);
    if (std::abs(get - CommandUnitConstanst::CHARGE_FOR_LOCK) <=
        std::abs(get + CommandUnitConstanst::CHARGE_FOR_LOCK) * std::numeric_limits<double>::epsilon())
      tracker_.lock(p, inner_ts_);
  }
}

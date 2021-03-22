//
// Created by yalavrinenko on 20.03.2021.
//

#include "object_tracker.hpp"
#include <algorithm>
#include <vector>

std::vector<polar_vector> tracking_object::make_trajectory_prediction(size_t points) const {
  double timedelta = timestamp::delta();

  auto generator = [this, timedelta](int i) {
    auto pv = (!positions_.empty()) ? positions_.front().v.in_cartesian() : vector_2d{0.0, 0.0};
    if (velocity_) pv += *velocity_ * (i * timedelta);
    if (acceleration_) pv += *acceleration_ * (0.5 * timedelta * timedelta);

    return pv.in_polar();
  };

  auto prediction = std::views::iota(0ul, points) | std::views::transform(generator) | std::views::common;

  return {prediction.begin(), prediction.end()};
}

void tracking_object::add_position_point(const polar_vector &p, timestamp const &ts) {
  if (positions_.empty() || !(positions_.back().v.r() == p.r() && positions_.back().v.phi() == p.phi()))
  {
    updated_at_ = ts;
    while (positions_.size() >= 3) { positions_.pop_front(); }

    positions_.emplace_back(tracked_entry{ts, p});
    if (positions_.size() >= 2) update_velocity();

    if (positions_.size() == 3) update_acceleration();
  }
}

void tracking_object::update_velocity() {
  auto v1 = velocity(positions_[0], positions_[1]);
  if (positions_.size() == 3) {
    auto v2 = velocity(positions_[1], positions_[2]);
    v1 = (v1 + v2) / 2;
  }
  velocity_ = v1;
}

void tracking_object::update_acceleration() {
  auto v1 = velocity(positions_[0], positions_[1]);
  auto v2 = velocity(positions_[1], positions_[2]);
  acceleration_ = ((v2 - v1) / (positions_[2].ts.now() - positions_[1].ts.now()));
}

polar_vector tracking_object::velocity(const tracking_object::tracked_entry &e1,
                                       const tracking_object::tracked_entry &e2) {
  auto diff = e2.v.in_cartesian() - e1.v.in_cartesian();
  return (diff / (e2.ts - e1.ts).now());
}

auto object_tracker::find_nearest(const polar_vector &p) {
  auto it = std::ranges::min_element(space_map_, [&p](auto const &lhs, auto const &rhs) {
    return (lhs.in_cartesian() - p.in_cartesian()).len() < (rhs.in_cartesian() - p.in_cartesian()).len();
  });
  return *it;
}

bool object_tracker::lock(const polar_vector &p, const timestamp &ts) {
  auto nearest = find_nearest(p);
  if (nearest.distance(p)) {//} <= object_tracker::RESOLUTION_EPS) {
    tracked_.emplace_back(next_uid_++, nearest, ts);
    return true;
  } else
    return false;
}
void object_tracker::update_tracked(timestamp const& ts) {
  auto update_func = [this, &ts](auto &tobject){
    auto nearest = find_nearest(tobject.last_position());
    tobject.add_position_point(nearest, ts);
  };

  std::ranges::for_each(tracked_, update_func);
}
void object_tracker::unlock(size_t id) {
  auto last = std::ranges::remove(tracked_, id, &tracking_object::uid);
  tracked_.erase(last.begin(), tracked_.end());
}

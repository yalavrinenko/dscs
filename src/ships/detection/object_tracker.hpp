//
// Created by yalavrinenko on 20.03.2021.
//

#ifndef DSCS_OBJECT_TRACKER_HPP
#define DSCS_OBJECT_TRACKER_HPP

#include <optional>
#include <common.hpp>
#include <deque>
#include <list>
#include <vector>
#include <algorithm>

class tracking_object{
public:
  explicit tracking_object(size_t id, polar_vector const& p, timestamp const& ts): id_{id}{
    add_position_point(p, ts);
  }

  void add_position_point(polar_vector const& p, timestamp const& ts);

  [[nodiscard]] auto const& last_position() const { return positions_.back().v; }
  [[nodiscard]] auto const& velocity() const { return velocity_; }
  [[nodiscard]] auto const& acceleration() const { return acceleration_; }

  [[nodiscard]] std::vector<polar_vector> make_trajectory_prediction(size_t points) const;

  [[nodiscard]] auto const& updated_at() const { return updated_at_; }

  [[nodiscard]] auto const& uid() const { return id_; }
protected:
  struct tracked_entry{
    timestamp ts;
    polar_vector v;
  };

  void update_velocity();
  void update_acceleration();

  [[nodiscard]] static polar_vector velocity(tracked_entry const& e1, tracked_entry const& e2) ;

private:
  size_t id_;


  timestamp updated_at_;

  std::deque<tracked_entry> positions_;
  std::optional<polar_vector> velocity_;
  std::optional<polar_vector> acceleration_;
};

class object_tracker {
public:

  void update_map(auto &&v, auto const& ts) {
    space_map_ = std::forward<decltype(v)>(v);
    update_tracked(ts);
  }

  bool lock(polar_vector const& p, timestamp const& ts);

  void unlock(size_t id);

  [[nodiscard]] auto const& tracked() const {
    return tracked_;
  }

  [[nodiscard]] auto const& target(size_t uid) const {
    auto it = std::ranges::find(tracked_, uid, &tracking_object::uid);
    if (it == tracked_.end())
      throw std::logic_error("No uid in tracked list");
    else
      return *it;
  }

private:
  static constexpr auto RESOLUTION_EPS = 1e-4;

  auto find_nearest(polar_vector const& p);

  void update_tracked(timestamp const& ts);

  std::vector<polar_vector> space_map_;

  std::list<tracking_object> tracked_;
  size_t next_uid_ = 0;
};


#endif//DSCS_OBJECT_TRACKER_HPP

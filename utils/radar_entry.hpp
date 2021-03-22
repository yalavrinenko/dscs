//
// Created by yalavrinenko on 22.03.2021.
//

#ifndef DSCS_RADAR_ENTRY_HPP
#define DSCS_RADAR_ENTRY_HPP
#include "gui_entries.hpp"

namespace gui {
  class radar_entry : public ilogger_entry {
  public:
    using radar_selection_callback = std::function<void(double r, double phi)>;
    using radar_unary_callback = std::function<void(size_t)>;
    struct radar_point {
      double r;
      double phi;
    };

    struct tracked_point{
      std::string description;
      size_t uid;
      double r, phi;
      std::pair<double, double> velocity;
      std::pair<double, double> acceleration;

      static auto constexpr TRAJECTORY_POINTS = 1000;
      std::array<std::pair<double, double>, TRAJECTORY_POINTS> trajectory;
    };

    using unary_function = std::function<std::pair<double, double>(int)>;

    static inline double pi() { return std::atan(1.0) * 4.0; }

    explicit radar_entry(std::shared_ptr<class logger_window> factory, std::string name, double max_r, size_t segments);

    void log(radar_point point) { data_.tmp().emplace_back(point); }
    void log(tracked_point const& point) { tracked_.tmp().emplace_back(point); }

    void change_range(double max_r, size_t segments) {
      max_r_ = max_r;
      segments_ = segments;
    }

    void flush() override { data_.swap(); tracked_.swap(); }

    void set_callbacks(radar_selection_callback on_select, radar_unary_callback on_unlock,
                       radar_unary_callback on_target) {
      on_select_ = on_select;
      on_unlock_ = on_unlock;
      on_target_ = on_target;
    }

  protected:
    void draw_impl() override;

    void process_mouse_click(ImVec2 const& center, double max_r, std::pair<double, double> const& scale);

    cloned_data<radar_point> data_;
    cloned_data<tracked_point> tracked_;
    double max_r_;
    size_t segments_{1};

    radar_selection_callback on_select_ = nullptr;
    radar_unary_callback on_unlock_ = nullptr;
    radar_unary_callback on_target_ = nullptr;
  };
}

#endif//DSCS_RADAR_ENTRY_HPP

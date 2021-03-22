//
// Created by yalavrinenko on 22.03.2021.
//

#include "radar_entry.hpp"
#include <src/common.hpp>
#include <numeric>
#include <numbers>

gui::radar_entry::radar_entry(std::shared_ptr<class logger_window> factory,
                              std::string name, double max_r, size_t segments):
    ilogger_entry(std::move(factory), std::move(name)),
    max_r_{max_r}, segments_{segments}{
}

void gui::radar_entry::draw_impl() {
  ImGui::Columns(2, nullptr, true);
  ImGui::Separator();
  ImGui::SetColumnWidth(0, 420);

  ImDrawList *dlist = ImGui::GetWindowDrawList();
  ImColor color(255, 0, 255);
  const ImVec2 raw_cursor_position = ImGui::GetCursorScreenPos();
  auto const &p = raw_cursor_position;

  float R = 200;
  float dR = R / segments_;
  ImVec2 center = {p.x + R, p.y + R};
//
//  dlist->AddLine({center.x - R, center.y}, {center.x + R, center.y}, color, 1);
//  dlist->AddLine({center.x, center.y - R}, {center.x, center.y + R}, color, 1);

  dlist->AddCircleFilled(center, 1, ImColor(255, 255, 255));
  for (auto i = 1; i <= segments_; ++i) {
    dlist->AddCircle(center, dR * i, color, 100, 1.0f);
  }

  auto convert = [this](double &r, double &phi){
    if (r >= max_r_)
      r = std::abs(r) / r * max_r_;
    auto x = r * std::cos(phi);
    auto y = r * std::sin(phi);
    r = -x; phi = y;
  };

  auto scale_point = [&convert, this, R](ImVec2 const& v){
    double x = v.x, y = v.y;
    convert(x, y);
    auto sx = static_cast<float>(x / max_r_) * R;
    auto sy = static_cast<float>(y / max_r_) * R;
    return ImVec2{sx, sy};
  };

  for (auto &point : data_.main()){
    auto mp = scale_point(ImVec2{static_cast<float>(point.r), static_cast<float>(point.phi)});

    ImVec2 target{center.x + mp.x, center.y + mp.y};
    dlist->AddCircleFilled(target, 4, ImColor(255, 255, 255));
  }

  for (auto &point : tracked_.main()){
    auto mp = scale_point(ImVec2{static_cast<float>(point.r), static_cast<float>(point.phi)});

    ImVec2 target{center.x + mp.x, center.y + mp.y};
    dlist->AddRect({target.x-5, target.y-5}, {target.x+5, target.y+5}, ImColor{0, 255,0}, 2);

    for (std::weakly_incrementable auto i : std::views::iota(1ul, point.trajectory.size())) {
      auto p1 = scale_point(ImVec2{static_cast<float>(point.trajectory[i-1].first), static_cast<float>(point.trajectory[i - 1].second)});
      auto p2 = scale_point(ImVec2{static_cast<float>(point.trajectory[i].first), static_cast<float>(point.trajectory[i].second)});

      auto scale_p1 = ImVec2{center.x + p1.x, center.y + p1.y};
      auto scale_p2 = ImVec2{center.x + p2.x, center.y + p2.y};

      dlist->AddLine(scale_p1, scale_p2, ImColor(255,0,0,255), 2.0f);
    }
  }

  process_mouse_click(center, R, {max_r_ / R, max_r_  / R});
  ImGui::Dummy({400, 400});

  ImGui::NextColumn();

  auto column_separator = [](){
    ImDrawList*   draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw_list->AddLine(ImVec2(p.x - 9999, p.y), ImVec2(p.x + 9999, p.y),  ImGui::GetColorU32(ImGuiCol_Border));
  };

  auto dec = [](auto v) {
    return v * 180.0 / std::numbers::pi_v<double>;
  };

  for (auto &point : tracked_.main()){
    column_separator();
    ImGui::Text("Target uid: %s\n"
                "Distance: %0.6lf\n"
                "Direction: %0.6lf\n"
                "Velocity (X, Y): \n\t\t(%0.3lf, %0.3lf)\n"
                "Acceleration (X, Y): \n\t\t(%0.3lf, %0.3lf)\n", point.description.c_str(), point.r, dec(point.phi),
                point.velocity.first, point.velocity.second,
                point.acceleration.first, point.acceleration.second);
    auto uid = point.uid;
    if (ImGui::Button(("Unlock##" + std::to_string(uid)).c_str()) && on_unlock_){
      on_unlock_(uid);
    }
    ImGui::SameLine();
    if (ImGui::Button(("As target##" + std::to_string(uid)).c_str()) && on_target_){
      on_target_(uid);
    }
    column_separator();
  }
  ImGui::Columns(1);
  ImGui::Separator();
}

void gui::radar_entry::process_mouse_click(const ImVec2 &center, double max_r, std::pair<double, double> const& scale) {
  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)){
    auto pos = ImGui::GetMousePos();
    auto cx = center.x - pos.x;
    auto cy = center.y - pos.y;
    polar_vector v{cx * scale.first, -cy * scale.second};
    v = v.in_polar();
    if (std::abs(cx) <= max_r && std::abs(cy) <= max_r && on_select_) {
      on_select_(v.r(), v.phi());
    }
  }
}

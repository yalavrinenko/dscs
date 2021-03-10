//
// Created by yalavrinenko on 17.02.2021.
//

#include "launcher_entry.hpp"

#include <utility>
void gui::launcher_entry::flush() {
  std::lock_guard<std::mutex> lg(io_mutex);
  data_.swap();
}
void gui::launcher_entry::draw_impl() {
  std::lock_guard<std::mutex> lg(io_mutex);
  [[maybe_unused]] auto &info = data_.main()[0];

  ImDrawList *dlist = ImGui::GetWindowDrawList();

  ImGui::Separator();
  {
    for (auto const &m : info.loaded) {
      ImGui::Separator();
      ImGui::Text("%s", ("Missile:" + m.name).c_str());

      ImGui::Columns(3, nullptr, true);
      ImGui::SetColumnWidth(2, 50);
      ImGui::Text("Fuel: %lf\t", m.fuel);
      ImGui::ProgressBar(m.fuel, {0.0f, 1.0f}, std::to_string(m.fuel).c_str());

      ImGui::NextColumn();
      ImGui::Text("Charge: %lf\t", m.charge);
      ImGui::ProgressBar(m.charge, {0.0f, 1.0f}, std::to_string(m.charge).c_str());

      ImGui::NextColumn();
      ImColor color = (m.warhead) ? ImColor{0, 255, 0} : ImColor{255, 0, 0};

      const ImVec2 cp = ImGui::GetCursorScreenPos();
      float R = 8;
      ImVec2 center = {cp.x + R, cp.y + R + 3};
      dlist->AddCircleFilled(center, 8, color, 100);

      ImGui::Separator();
      ImGui::Columns(1);
    }
  }

  ImGui::Separator();
}

gui::launcher_entry::launcher_entry(std::shared_ptr<logger_window> factory, std::string name)
    : ilogger_entry(std::move(factory), std::move(name)) {}
void gui::launcher_entry::log(const gui::launcher_log_data& log) {
  data_.tmp().emplace_back(log);
}

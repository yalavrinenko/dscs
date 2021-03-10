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

  ImGui::Text("Total %zu.\tLoaded: %zu", info.total, info.loaded.size());
  auto getter = [](void *ptr, int i) -> float { return 1.0; };

  {
    auto color = ImColor(0, 0, 255);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4) color);

    ImGui::PlotHistogram("", getter, nullptr, info.total - info.loaded.size(), 0, nullptr, 0, 1, ImVec2(200, 60));

    ImGui::PopStyleColor(1);
  }

  {
    ImGui::SameLine();
    auto color = ImColor(255, 0, 0);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4) color);

    ImGui::PlotHistogram("", getter, nullptr, info.loaded.size(), 0, nullptr, 0, 1, ImVec2(70, 60));

    ImGui::PopStyleColor(1);
  }

  if (ImGui::Button("Load", {200, 0})) { callbacks_.on_load(0); }

  ImGui::Separator();
  {
    for (auto const &m : info.loaded) {
      if (m.load_progress >= 1.0) {
        ImGui::Separator();
        ImGui::Text("%s", ("Missile:" + m.name).c_str());

        ImGui::Columns(4, nullptr, true);
        ImGui::SetColumnWidth(2, 50);
        ImGui::Text("Fuel: %0.2lf\t", m.fuel);
        ImGui::ProgressBar(m.fuel, {0.0f, 1.0f}, std::to_string(m.fuel).c_str());

        ImGui::NextColumn();
        ImGui::Text("Charge: %0.2lf\t", m.charge);
        ImGui::ProgressBar(m.charge, {0.0f, 1.0f}, std::to_string(m.charge).c_str());

        ImGui::NextColumn();

        auto circle_draw = [dlist](auto color, float shift) {
          const ImVec2 cp = ImGui::GetCursorScreenPos();
          float R = 8;
          ImVec2 center = {cp.x + R + shift, cp.y + R + 3};
          dlist->AddCircleFilled(center, 8, color, 100);
        };

        circle_draw((m.warhead) ? ImColor{0, 255, 0} : ImColor{255, 0, 0}, 0);
        circle_draw((m.lock_target) ? ImColor{0, 255, 0} : ImColor{255, 0, 0}, 20);

        ImGui::NextColumn();
        if (!m.warhead) {
          if (ImGui::Button("ARM")) callbacks_.on_arm(m.callback_index);
        } else {
          if (ImGui::Button("DISARM")) callbacks_.on_disarm(m.callback_index);
        }
        ImGui::SameLine();
        if (ImGui::Button("LOCK")) callbacks_.on_lock(m.callback_index);
        if (m.ready2fire) {
          ImGui::SameLine();
          if (ImGui::Button("LAUNCH")) callbacks_.on_launch(m.callback_index);
        }

        ImGui::Separator();
        ImGui::Columns(1);
      } else {
        ImGui::Separator();
        ImGui::Text("Load from cargo: %0.2lf\t", m.load_progress);
        ImGui::ProgressBar(m.load_progress, {0.0f, 1.0f}, std::to_string(m.load_progress).c_str());
        ImGui::Separator();
      }
    }
  }
  ImGui::Separator();
}

void gui::launcher_entry::log(const gui::launcher_log_data &log) { data_.tmp().emplace_back(log); }
gui::launcher_entry::launcher_entry(std::shared_ptr<logger_window> factory, std::string name,
                                    gui::launcher_entry::callback_set callbacks)
    : ilogger_entry(std::move(factory), std::move(name)), callbacks_(std::move(callbacks)) {}

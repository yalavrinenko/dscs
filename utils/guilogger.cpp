//
// Created by yalavrinenko on 19.03.2020.
//

#include "guilogger.hpp"
#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

void gui::logger_factory::init_window() {
  window_.setVerticalSyncEnabled(true);
  ctx_ = ImGui::CreateContext();
  ImGui::SFML::Init(window_);
  window_.resetGLStates();
}

gui::logger_factory::~logger_factory() {
  ImGui::DestroyContext(ctx_);
  ImGui::SFML::Shutdown();
  wthread_.get();
}

void gui::logger_factory::draw() {
  sf::Clock deltaClock;
  window_.setFramerateLimit(60);
  while (is_active()) {
    events();

    ImGui::SFML::Update(window_, deltaClock.restart());

    for (auto &entry : entries_)
      if (entry)
        entry->draw();

    window_.clear(); // fill background with color
    ImGui::SFML::Render(window_);
    window_.display();
  }
}
void gui::logger_factory::events() {
  sf::Event event{};
  while (window_.pollEvent(event)) {
    ImGui::SFML::ProcessEvent(event);

    if (event.type == sf::Event::Closed) {
      window_.close();
      closed_ = true;
    }
  }
}
gui::ilogger_entry::ilogger_entry(std::shared_ptr<class logger_factory> factory, std::string name):
    linked_factory_{std::move(factory)}, name_{std::move(name)}{}
void gui::ilogger_entry::draw() {
  ImGui::Begin(name_.c_str());
  draw_impl();
  ImGui::End();
}

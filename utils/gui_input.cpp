//
// Created by yalavrinenko on 23.04.2020.
//
#include "gui_input.hpp"
gui::gui_controls::gui_controls(std::shared_ptr<struct logger_window> factory, std::string name) : ilogger_entry(
    std::move(factory), std::move(name)) {
}

void gui::gui_controls::flush() { }

void gui::gui_controls::draw_impl() {
  for (auto const &control_ptr : controls_){
    control_ptr->draw();
  }
}

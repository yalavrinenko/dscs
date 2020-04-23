//
// Created by yalavrinenko on 23.04.2020.
//

#ifndef DSCS_GUI_INPUT_HPP
#define DSCS_GUI_INPUT_HPP
#include "guilogger.hpp"
#include <memory>
#include <vector>
namespace gui {

  class icontrol {
  public:
    virtual void draw() = 0;
  };

  using control_callback = std::function<void(icontrol &)>;

  class gui_controls : public ilogger_entry {
  public:
    explicit gui_controls(std::shared_ptr<struct logger_window> factory, std::string name);

    template<typename CT, typename... T>
    void add_control(T &&... args) {
      controls_.emplace_back(std::make_unique<CT>(args...));
    }

    template<typename control_type>
    void add_control(std::unique_ptr<control_type> &&controls){
      controls_.emplace_back(std::move(controls));
    }

    void flush() override;

    void draw_impl() override;

  protected:
    std::vector<std::unique_ptr<icontrol>> controls_;
  };

  template<typename callback_type>
  class button_control : public icontrol {
  public:
    using callback_t = callback_type;

    button_control() = default;

    button_control(std::string text, callback_t callback) : button_text_{std::move(text)},
                                                            callback_{std::move(callback)} {}

    void draw() override {
      if (!button_text_.empty() && ImGui::Button(button_text_.c_str())) {
        callback_(*this);
      }
    }

  protected:
    callback_type callback_;
    std::string button_text_;
  };

  template<typename callback_type, std::size_t N>
  class button_group : public icontrol {
  public:
    using button_t = button_control<callback_type>;

    button_group(std::initializer_list<button_t> &&list) {
      auto i = 0;
      for (auto &&e : list){
          buttons_[i++] = std::move(e);
      }
    }
    void draw() override {
      ImGui::Columns(N, nullptr, false);
      for (auto &button : buttons_) {
        button.draw();
        ImGui::NextColumn();
      }
      ImGui::Columns(1);
    }

  protected:
    std::array<button_control<callback_type>, N> buttons_;
  };
}// namespace gui

#endif//DSCS_GUI_INPUT_HPP

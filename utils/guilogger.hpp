//
// Created by yalavrinenko on 19.03.2020.
//

#ifndef DSCS_GUILOGGER_HPP
#define DSCS_GUILOGGER_HPP

#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <future>
#include <imgui.h>
#include <mutex>
#include <vector>

namespace gui {
class ilogger_entry{
public:
  explicit ilogger_entry(std::shared_ptr<class logger_factory> factory, std::string name);
  virtual void draw();
  virtual void flush() = 0;

  std::shared_ptr<class logger_factory>& factory() { return linked_factory_; }
protected:
  virtual void draw_impl() = 0;
  std::shared_ptr<class logger_factory> linked_factory_;
  std::string name_;
  std::mutex io_mutex;
};

class logger_factory : public std::enable_shared_from_this<logger_factory> {
public:
  static std::shared_ptr<logger_factory> create(std::string name="") {
    static auto factory = std::shared_ptr<logger_factory>(new logger_factory(std::move(name)));
    return factory;
  }

  void draw();

  bool is_active() const { return window_.isOpen() && !closed_; }

  template <typename LoggerType, typename ... TInitArgs>
  std::shared_ptr<LoggerType> create_logger(TInitArgs&& ... args){
    auto ptr = std::make_shared<LoggerType>(shared_from_this(), std::forward<TInitArgs>(args)...);
    entries_.emplace_back(ptr);
    return ptr;
  }

  void flush(){
    for (auto &e : entries_)
      e->flush();
  }

  ~logger_factory();
protected:
  explicit logger_factory(std::string name)
      : factory_name_{std::move(name)},
        window_(sf::VideoMode(1280, 1000), factory_name_) {
    init_window();
    wthread_ = std::async(std::launch::async, [this](){ this-> draw(); });
  }

  void init_window();

  void events();

private:
  std::string factory_name_;
  sf::RenderWindow window_;
  ImGuiContext* ctx_{nullptr};
  std::vector<std::shared_ptr<ilogger_entry>> entries_;

  std::future<void> wthread_;

  bool closed_ = false;
};

template <typename DataType, typename container=std::vector<DataType>>
class cloned_data{
public:
  void swap() {
    std::swap(main_, tmp_);
    tmp_.clear();
  }
  container& main() { return main_; }
  container& tmp() { return tmp_;}
private:
  container main_, tmp_;
};
} // namespace gui
#endif // DSCS_GUILOGGER_HPP

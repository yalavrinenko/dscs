//
// Created by yalavrinenko on 17.02.2021.
//

#ifndef DSCS_LAUNCHER_ENTRY_HPP
#define DSCS_LAUNCHER_ENTRY_HPP

#include <utils/guilogger.hpp>
namespace gui {
  struct launcher_log_data{
    struct missile_info{
      double fuel{0};
      double charge{0};
      bool warhead{false};
      std::string name{};
      size_t callback_index{};

      bool lock_target;
      bool ready2fire;
      double load_progress;
    };
    size_t total;
    std::vector<missile_info> loaded;
  };

  class launcher_entry: public gui::ilogger_entry {
  public:
    using on_action_cb = std::function<void(size_t sender)>;
    struct callback_set{
      on_action_cb on_arm, on_lock, on_launch, on_disarm;
      on_action_cb on_load;
    };
    launcher_entry(std::shared_ptr<logger_window> factory, std::string name,
                   callback_set callbacks);
    void log(const launcher_log_data& log);
    void flush() override;
  protected:
    void draw_impl() override;

    cloned_data<launcher_log_data> data_;

    callback_set callbacks_;
  };
}


#endif//DSCS_LAUNCHER_ENTRY_HPP

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
    };
    std::vector<missile_info> loaded;
  };

  class launcher_entry: public gui::ilogger_entry {
  public:
    explicit launcher_entry(std::shared_ptr<logger_window> factory, std::string name);
    void log(const launcher_log_data& log);
    void flush() override;
  protected:
    void draw_impl() override;

    cloned_data<launcher_log_data> data_;
  };
}


#endif//DSCS_LAUNCHER_ENTRY_HPP

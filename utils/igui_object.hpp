//
// Created by yalavrinenko on 20.03.2020.
//

#ifndef DSCS_IGUI_OBJECT_HPP
#define DSCS_IGUI_OBJECT_HPP
#include "guilogger.hpp"
namespace gui{
class igui_object{
public:
  template <typename entry_type>
  int add_gui_entry(entry_type e){
    entries_.push_back(std::move(e));
    return entries_.size() - 1;
  }

  template <typename entry_type, typename ... Args>
  int add_gui_entry(Args ... args){
    entries_.push_back(gui::logger_factory::create()->create_logger<entry_type>(std::forward<Args>(args)...));
    return entries_.size() - 1;
  }

  template <typename entry_type>
  std::shared_ptr<entry_type> entry(int id) {
    return std::dynamic_pointer_cast<entry_type>(entries_[id]);
  }

  template <typename entry_type>
  std::shared_ptr<entry_type> entry() {
    for (auto &ptr : entries_){
      auto cptr = std::dynamic_pointer_cast<entry_type>(ptr);
      if (cptr != nullptr)
        return cptr;
    }
    return nullptr;
  }

  virtual void draw() = 0;
protected:
  std::vector<std::shared_ptr<gui::ilogger_entry>> entries_;
};
}
#endif // DSCS_IGUI_OBJECT_HPP

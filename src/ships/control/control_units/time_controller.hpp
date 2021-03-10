//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_TIME_CONTROLLER_HPP
#define DSCS_TIME_CONTROLLER_HPP

#include "../icontrol.hpp"
#include <ranges>
#include <algorithm>

template<typename arg_t>
using visitor_func_t = std::function<void(icontrol::component_group<arg_t> &)>;
using time_action_function = std::variant<visitor_func_t<pengine>, visitor_func_t<preactor>, visitor_func_t<pwarhead>>;

struct queue_cmp {
  bool operator()(std::pair<timestamp, time_action_function> const &lhs,
                  std::pair<timestamp, time_action_function> &rhs) const {
    return rhs.first < lhs.first;
  }
};

template<typename main_t, typename linked_t>
struct augmented_visitor{
  explicit augmented_visitor(linked_t const& v): linked_visitor_{v} {}
  void operator()(auto&& value){
    if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::decay_t<main_t>>){
      linked_visitor_(value);
    }
  }
  linked_t const& linked_visitor_;
};

template<typename T>
augmented_visitor(visitor_func_t<T>) -> augmented_visitor<icontrol::component_group<T> const&, visitor_func_t<T>>;

class timed_control: public icontrol{
public:
  void add_action(timestamp ts, time_action_function action);

protected:
  std::vector<control_action> control_impl(timestamp const &ts, control_interface &ship) override;

  void perform_action(time_action_function const& act){
    auto apply_visitor = [this](auto&& func){
      auto vf = augmented_visitor(func);
      std::ranges::for_each(controllers(), [&vf](auto &e){
        std::visit(vf, e);
      });
    };
    std::visit(apply_visitor, act);
  }

private:
  std::priority_queue<std::pair<timestamp, time_action_function>, std::vector<std::pair<timestamp, time_action_function>>,
                      queue_cmp> queue_;
  timestamp begin_{};
};

#endif // DSCS_TIME_CONTROLLER_HPP

//
// Created by yalavrinenko on 30.08.2019.
//

#include "actions.hpp"
bool action_queue::invoke_next(timestamp const &ts) {
  if (!queue_.empty() && queue_.top().step() == ts) {
    auto top = queue_.top(); queue_.pop();
    top.run(ts);
    return true;
  } else {
    return false;
  }
}

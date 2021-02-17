//
// Created by yalavrinenko on 06.11.2019.
//

#include "remote_control.hpp"
std::vector<control_action>
radio_control_unit::control(timestamp const &ts, control_interface &ship) {
  if (is_first){
    is_first = false;
    t = ts;
  }

  if (ts == t + 20){
 //   auto rotate = [](pengine &engine){ engine->align(45.0); };
//    engines.apply(rotate);
  }

  if (ts == t + 50){
//    auto ignite = [](pengine &engine) { engine->ignite(); };
//    engines.apply(ignite);
  }
  return {};
}

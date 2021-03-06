//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_DRIFTING_HULK_HPP
#define DSCS_DRIFTING_HULK_HPP
#include "void_object.hpp"

using namespace std::string_literals;

static int halk_count_ = 0;

class drifting_hulk: public void_object {
public:
  explicit drifting_hulk(double mass): void_object(mass, "Space_hulk_"s + std::to_string(++halk_count_),
                                                   {nullptr, nullptr}){
  }

  void update(timestamp const &time) override {}

  vector_2d force(timestamp const &time) override { return vector_2d(); }

  void action() override {}

  void log_action() const override {}
  void draw() override {}
  std::vector<control_action>
  extract_control_actions(timestamp const &ts) override { return {}; }
};

#endif // DSCS_DRIFTING_HULK_HPP

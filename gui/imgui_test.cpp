//
// Created by yalavrinenko on 19.03.2020.
//

#include "../utils/gui_entries.hpp"
#include "../utils/guilogger.hpp"
#include <X11/Xlib.h>
#include <future>
#include <random>
int main(){
  XInitThreads();

  auto lf = gui::logger_factory::create("Factory!");

  //auto plot = lf->create_logger<gui::moving_plot_entry>("Plot entry", 100);
  auto pplot = lf->create_logger<gui::polar_entry>("Plot entry", 1);
  auto pplot1 = lf->create_logger<gui::polar_entry>("Plot entry", 1);

  std::mt19937_64 engine{std::random_device{}()};
  std::normal_distribution<double> nd{0.5, 0.5};
  std::uniform_real_distribution<double> ud{0, 2.0 * gui::polar_entry::pi()};
  while (lf->is_active()){
    auto v = [&nd, &ud, &engine](int i){
      if (i == -1) return std::pair<double, double>{1.0, 1.0};
      return std::pair<double, double>{nd(engine), ud(engine)};
    };

    pplot->log("Direction:", v);
    lf->flush();
    //plot->log("nd:", nd(engine));
    //plot->log("ud:", ud(engine));
  }

  return 0;
}
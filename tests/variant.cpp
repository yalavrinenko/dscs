//
// Created by yalavrinenko on 10.03.2021.
//
#include <variant>
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

template<typename ... T>
struct wrapper: public T...{
  using T::operator()...;

  template<typename O>
  void operator()(O) { std::cout << "Augmentation" << std::endl; }
};
template<typename ...T>
wrapper(T...) -> wrapper<T...>;

template<typename T>
using variant_visitor_t = std::function<void(T)>;

int main(int argc, char** argv){
  using v_t = std::variant<double, int, char, std::string>;
  std::vector<v_t> v{3.14, 23, 'a', 56, 2.33, 't', std::string{"Hello"}};

  wrapper w {
      [](int v) { std::cout << "Int " << v << std::endl; },
      [](double v) { std::cout << "Double " << v << std::endl; },
      [](char v) { std::cout << "Char " << v << std::endl; },
      //[](std::string const& v) { std::cout << "String " << v << std::endl; }
  };

  std::ranges::for_each(v, [&w](auto &e){
    std::visit(w, e);
  });

//  using visitor_func_t = std::variant<variant_visitor_t<int>, variant_visitor_t<double>, variant_visitor_t<char>, variant_visitor_t<std::string const&>>;
//  std::vector<visitor_func_t> vf {
//      variant_visitor_t<int>([](int v) { std::cout << "Int " << v << std::endl; }),
//      variant_visitor_t<double>([](double v) { std::cout << "Double " << v << std::endl; }),
//      variant_visitor_t<char>([](char v) { std::cout << "Char " << v << std::endl; }),
//      [](std::string const& v) { std::cout << "String " << v << std::endl; }
//  };
//
//  wrapper vf_w {
//      [](variant_visitor_t<int> const &v) { std::cout << "int()" << std::endl; },
//      [](variant_visitor_t<double> const &v) { std::cout << "double()" << std::endl; },
//      [](variant_visitor_t<char> const &v) { std::cout << "char()" << std::endl; },
//      [](variant_visitor_t<std::string const&> const &v) { std::cout << "std::string const&()" << std::endl; },
//  };
//
//  auto lookup_func = [&vf_w, &v](auto &e) {
//    std::ranges::for_each(v, [&e](auto &i) { std::visit(e, i); });
//  };
//
//  std::ranges::for_each(vf, lookup_func);

  return 0;
}
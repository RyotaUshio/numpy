#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>


namespace numpy {
  template <typename T> class ndarray;
  namespace utils {

  template <typename T>
  T product(const std::vector<T>& vec, std::size_t from=0, std::size_t to=0) {
    return std::reduce(vec.begin()+from, vec.end()-to, T(1), [](T a, T b){return a * b;});
  }

  // for debug
  template <typename T>
  void print_vector(std::vector<T> vec) {
    for(const auto e : vec)
      std::cout << e << ", ";
    std::cout << std::endl;
}

  void line(int width=80) {
    std::cout << std::string(width, '-') << std::endl;
  }
  
  template <typename T>
  void test(ndarray<T>& a) {
    line();
    std::cout << "__repr__() = ";
    std::cout << a.__repr__() << std::endl;
    line();
    std::cout << "begin().info()" << std::endl;
    a.begin().info();
    line();
    std::cout << "end().info()" << std::endl;
    a.end().info();
    line();
    std::cout << "range-based for loop" << std::endl;
    for (const auto e : a)
      std::cout << e << ", ";
    std::cout << std::endl;
    line();
  }

}
}

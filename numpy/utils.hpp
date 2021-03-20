#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <string>
#include <sstream>
#include <numpy/typename.hpp>


namespace numpy {
  template <typename T> class ndarray;
  namespace utils {
    
    template <typename T>
    T product(const std::vector<T>& vec, std::size_t from=0, std::size_t to=0) {
      return std::reduce(vec.begin()+from, vec.end()-to, T(1), [](T a, T b){return a * b;});
    }

    template <typename T>
    T dot(const std::vector<T>& lhs, const std::vector<T>& rhs, T init=0) {
      return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), init);
    }
    
    void line(int width=80) {
      std::cout << std::string(width, '-') << std::endl;
    }
  
    template <typename T>
    void test(ndarray<T>& a) {
      std::cout << "dtype : " << python::str(typeid(T)) << std::endl;
      std::cout << "__repr__() : " << std::endl;;
      std::cout << a.__repr__() << std::endl;
      line();
      std::cout << "range-based for loop" << std::endl;
      for (const auto e : a)
        std::cout << e << ", ";
      std::cout << std::endl;
      line();
    }

  }
}

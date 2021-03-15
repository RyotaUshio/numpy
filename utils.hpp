#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <string>
#include <sstream>


namespace numpy {
  template <typename T> class ndarray;
  namespace utils {
    
    template <typename T>
    T product(const std::vector<T>& vec, std::size_t from=0, std::size_t to=0) {
      return std::reduce(vec.begin()+from, vec.end()-to, T(1), [](T a, T b){return a * b;});
    }
    
    // for debug
    template <typename T>
    std::string vector_to_str(const std::vector<T>& vec) {
      std::stringstream ss;
      bool first = true;
      ss << "(";
      for(const auto e : vec) {
	if (first) {
	  first = false;
	} else {
	  ss << ", ";
	}
	ss << e;
      }
      ss << ")";
      return ss.str();
    }
    
    template <typename T>
    void print_vector(std::vector<T> vec) {
      std::cout << vector_to_str(vec) << std::endl;
    }  

    void line(int width=80) {
      std::cout << std::string(width, '-') << std::endl;
    }
  
    template <typename T>
    void test(ndarray<T>& a) {
      std::cout << "dtype : " << a.dtype.name() << std::endl;
      std::cout << "__repr__() = ";
      std::cout << a.__repr__() << std::endl;
      // line();
      // std::cout << "begin().info()" << std::endl;
      // a.begin().info();
      // line();
      // std::cout << "end().info()" << std::endl;
      // a.end().info();
      // line();
      // std::cout << "range-based for loop" << std::endl;
      // for (const auto e : a)
      //   std::cout << e << ", ";
      // std::cout << std::endl;
      line();
    }

  }
}

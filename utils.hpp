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

    template <typename T>
    T dot(const std::vector<T>& lhs, const std::vector<T>& rhs, T init=0) {
      return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), init);
    }
    
    // for debug
    template <typename T>
    std::string str(const std::vector<T>& vec) {
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
    std::string vector_to_str(const std::vector<T>& vec) {
      return str(vec);
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

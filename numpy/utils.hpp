#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <string>
#include <sstream>
#include <numpy/typename.hpp>

namespace python {
  std::string str(const std::type_info& dtype);
}    

namespace numpy {
  template <typename T> class ndarray;
  namespace utils {
    
    template <typename T>
    T product(const std::vector<T>& vec, std::size_t from=0, std::size_t to=0) {
      return std::reduce(vec.begin()+from, vec.end()-to, T(1), [](T a, T b){return a * b;});
    }

    template <template <class, class...> class Container1,
	      template <class, class...> class Container2,
	      class T>
    T dot(const Container1<T>& lhs, const Container2<T>& rhs, T init=0) {
      return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), init);
    }

    template <typename T>
    std::string vector_to_string(const std::vector<T>& vec, std::string prefix="", std::string suffix="") {
      std::stringstream ss;
      bool first = true;
      ss << prefix;
      for(const auto e : vec) {
	if (not first) 
	  ss << ", ";
	ss << e;
	first = false;
      }
      ss << suffix;
      return ss.str();
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

    template <class Dtype>
    ndarray<Dtype> bring_axis_to_head(const ndarray<Dtype>& array, axis_type axis) {
      auto axes = axes_type(array.ndim());
      axes[0] = axis;
      axis_type ax = 0;
      std::generate(axes.begin() + 1, axes.end(),
		    [&ax, axis](){
		      if (ax == axis) ax++;
		      return ax++;
		    });
      return array.transpose(axes);
    }    
  }
  
}

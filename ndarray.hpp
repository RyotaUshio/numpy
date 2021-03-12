#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <stdexcept>

namespace numpy {
  typedef unsigned int uint;

  template <typename T>
  class ndarray {

    // attributes
    std::vector<T> _data;
  public:
    std::vector<uint> shape;
    uint size;
    uint ndim;

    // constructors
    ndarray(std::vector<T> src, std::vector<uint> shape_)
      : _data(src), shape(shape_) {
      size = _shape_to_size();
      ndim = shape.size();

      if (size != src.size())
	throw std::invalid_argument("Imcompatible shape");      
    }
    
    ndarray(T* first, T* last, std::vector<uint> shape_)
      : ndarray<T>(std::vector<T>(first, last), shape_) {}
    
    std::vector<T> data() const {
      return _data;
    }

    // operators
    // ndarray<T>& operator+=(ndarray<T> rhs) {
    //   if (not std::equal(shape, rhs.shape))
    // 	throw std::invalid_argument("Cannot add arrays with different shape");
      
    //   // iteratorが必要
    //   return *this;
    // }
    
    // other methods
  private:
    uint _shape_to_size() const {
      uint product = 1;
      for(const auto e : shape)
	product *= e;
      return product;
    }
  };
}

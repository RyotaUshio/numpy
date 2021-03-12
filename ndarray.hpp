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
    std::vector<T> _data;
  public:
    std::vector<uint> shape;
    uint size;
    uint ndim;

    ndarray(std::initializer_list<T> src, std::initializer_list<uint> shape_)
      : _data(src.begin(), src.end()), shape(shape_) {
      size = _shape_to_size();
      ndim = shape.size();

      if (size != src.size())
	throw std::invalid_argument("Imcompatible shape");
    }
    
    std::vector<T> data() const {
      return _data;
    }

  private:
    uint _shape_to_size() {
      uint product = 1;
      for(const auto e : shape)
	product *= e;
      return product;
    }
  };
}

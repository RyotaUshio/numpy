#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <cstdio>
#include "utils.hpp"

namespace numpy {
  template <typename T> class ndarray;
  
  template <typename T>
  struct nditer {
    const ndarray<T>& array;
    std::size_t index; // index w.r.t. the axis
    int axis;
    int max_index;
    
    nditer(const ndarray<T>& arr, int ax=-1, std::size_t offset=0)
      : array(arr), axis(ax) {
      if (axis > 0) {
	max_index = array.shape[axis];
      }
    }

    ndarray<T>& operator*() {
      
    }

    nd_iter<T>& operator+=(const std::size_t rhs) {
    }

    nd_iter<T> operator+(const std::size_t rhs) const {
      nd_iter<T> tmp(*this);
      tmp += rhs;
      return tmp;
    }
    
    nd_iter<T>& operator++() { // pre-increment
      *this += 1;
      return *this;
    }
    
    nd_iter<T> operator++(int) { // post-increment: takes a dummy parameter of int
      nd_iter<T> tmp(*this);
      operator++();
      return tmp;
    }

    long operator-(const nd_iter<T>& rhs) const {
    }
    
    bool operator!=(const nd_iter<T>& rhs) const {
      return (index != rhs.index) or (axis != rhs.axis) or (&array != &(rhs.array));
    }

    bool operator==(const nd_iter<T>& rhs) const {
      return (not (*this) != rhs);
    }
  };
}

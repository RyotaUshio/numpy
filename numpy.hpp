#pragma once
#include "ndarray.hpp"


namespace numpy {
  template <class T>
  ndarray<T> reshape(const ndarray<T>& a, const shape_type& newshape) {
    return a.reshape(newshape);
  }
}

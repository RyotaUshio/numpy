#pragma once
#include "ndarray.hpp"


namespace numpy {
  template <class T>
  ndarray<T> reshape(const ndarray<T>& a, const shape_type& newshape) {
    return a.reshape(newshape);
  }

  template <class T>
  ndarray<T> transpose(const ndarray<T>& a, const std::vector<dim_type>& axes) {
    return a.transpose(axes);
  }

  template <class T>
  ndarray<T> transpose(const ndarray<T>& a) {
    return a.transpose();
  }

}

#pragma once

#include <algorithm>

namespace numpy {

  template <class Dtype> class ndarray;


  class ufunc : public python::object {
  /**
   * https://numpy.org/doc/stable/reference/ufuncs.html 
   * https://numpy.org/doc/stable/reference/generated/numpy.ufunc.html
   */
  };

  class ufunc_unary : public ufunc {
    
  public:
    template <typename Dtype>
    ndarray<Dtype> operator() (ndarray<Dtype>& x) {
      
    }

    template <typename Dtype>
    ndarray<Dtype>& operator() (ndarray<Dtype>& x, ndarray<Dtype>& out) {
      
    }
    
  };

  
  class ufunc_binary : public ufunc {
    
  public:
    template <typename Dtype1, typename Dtype2>
    auto operator() (ndarray<Dtype1>& x1, ndarray<Dtype2>& x2) -> decltype(x1) {
      
    }

    // ...
    template <typename Dtype>
    auto operator() (ndarray<Dtype>& x1, ndarray<Dtype>& x2, ndarray<Dtype>& out) -> decltype(x1) {
      
    }
    
  };
  
}

#include <numpy/ufunc_instances.hpp>

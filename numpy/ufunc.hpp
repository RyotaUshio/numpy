#pragma once

#include <algorithm>

namespace numpy {

  template <class Dtype> class ndarray;


  class ufunc // : public python::object
  {
  /**
   * https://numpy.org/doc/stable/reference/ufuncs.html 
   * https://numpy.org/doc/stable/reference/generated/numpy.ufunc.html
   **/
  public:
    /**
     * Attributes
     * ----------
     * identity
     *   The identity value.
     * nargs
     *   The number of arguments.
     * nin
     *   The number of inputs.
     * nout
     *   The number of outputs.
     * ntypes
     *   The number of types.
     * signature
     *   Definition of the core elements a generalized ufunc operates on.
     * types
     *   Returns a list with types grouped input->output.
     **/
    std::size_t nargs;
    std::size_t nin;
    std::size_t nout;

    /**
     * Methods
     * -------
     * __call__(*args, **kwargs)
     *   Call self as a function.
     * accumulate(array[, axis, dtype, out])
     *   Accumulate the result of applying the operator to all elements.
     * at(a, indices[, b])
     *   Performs unbuffered in place operation on operand ‘a’ for elements specified by ‘indices’.
     * outer(A, B, /, **kwargs)
     *   Apply the ufunc op to all pairs (a, b) with a in A and b in B.
     * reduce(array[, axis, dtype, out, keepdims, …])
     *   Reduces array’s dimension by one, by applying ufunc along one axis.
     * reduceat(array, indices[, axis, dtype, out])
     *   Performs a (local) reduce with specified slices over a single axis.
    **/
  };

  class ufunc_unary : public ufunc {

    // the default out=None for uninitialized outputs to be allocated by the ufunc
  public:
    template <typename Dtype>
    ndarray<Dtype> operator() (ndarray<Dtype>& x) {
      
    }

    template <typename Dtype>
    ndarray<Dtype>& operator() (ndarray<Dtype>& x, ndarray<Dtype>& out) {
      // broadcast(x);
      // std::transform(x.begin(), x.end(), rhs.begin(), begin(), op);
      // 戻す
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

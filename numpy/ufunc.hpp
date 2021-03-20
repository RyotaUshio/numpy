#pragma once

#include <functional>
#include <algorithm> // transform
#include <utility> // move
#include <type_traits> // is_same

namespace numpy {

  template <class Dtype> class ndarray;


  class ufunc {
  /**
   * https://numpy.org/doc/stable/reference/ufuncs.html 
   * https://numpy.org/doc/stable/reference/generated/numpy.ufunc.html
   **/
    
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
  public:
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

  template <template <class> class UnaryOperation>
  struct ufunc_unary {

    // template <class Type>
    // auto operator()(const ndarray<Type>& x)
    //   -> ndarray<decltype(UnaryOperation<Type>()(x))> {
      
    //   ndarray<decltype(BinaryOperation<Type1, Type2>()(x1, x2))> out;
    //   return operator()(x1, x2, out);
    // }

    // template <class Type, class OutType>
    // auto operator()(const ndarray<Type>& x, const ndarray<OutType>& out)
    //   -> ndarray<decltype(BinaryOperation<Type>()(x))> {
      
    //   auto x1_copy = x1.view;
    //   auto x2_copy = x2.view;
    //   broadcast(x1, x2);
    //   static_assert(std::is_same_v<OutType, decltype(BinaryOperation<Type1, Type2>()(x1, x2))>, "output type invalid");
    //   std::transform(x1.begin(), x2.end(), x2.begin(), out.begin(), BinaryOperation<Type1, Type2>());
    //   x1.view = std::move(x1_copy);
    //   x2.view = std::move(x2_copy);
    //   return out;
    // }

  };

  template <template <class, class> class BinaryOperation>
  struct ufunc_binary {

    template <class Type1, class Type2>
    auto operator()(ndarray<Type1>& x1, ndarray<Type2>& x2)
      -> ndarray<decltype(BinaryOperation<Type1, Type2>()(x1, x2))> {
      
      ndarray<decltype(BinaryOperation<Type1, Type2>()(x1, x2))> out;
      return operator()(x1, x2, out);
    }

    template <class Type1, class Type2, class OutType>
    auto operator()(ndarray<Type1>& x1, ndarray<Type2>& x2, ndarray<OutType>& out)
      -> ndarray<decltype(BinaryOperation<Type1, Type2>()(x1, x2))> {
  
      auto x1_copy = x1.view;
      auto x2_copy = x2.view;
      broadcast(x1, x2);
      static_assert(std::is_same_v<OutType, decltype(BinaryOperation<Type1, Type2>()(x1, x2))>, "output type invalid");
      std::transform(x1.begin(), x2.end(), x2.begin(), out.begin(), BinaryOperation<Type1, Type2>());
      x1.view = std::move(x1_copy);
      x2.view = std::move(x2_copy);
      return out;
    }
    
  };
}

#include <numpy/ufunc_instances.hpp>

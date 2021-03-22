#pragma once

#include <functional>
#include <algorithm> // transform
#include <utility> // move
#include <type_traits> // is_same
#include <numpy/shares_memory.hpp>

namespace numpy {

  template <class Dtype> class ndarray;
  template <class Dtype> ndarray<Dtype> empty(const shape_type& shape);
  

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
  struct ufunc_unary: public ufunc {

    constexpr ufunc_unary() = default;
    
    template <class Type>
    auto operator()(ndarray<Type>& x) const
      -> ndarray<decltype(UnaryOperation<Type>()(Type()))> {

      auto out = empty<decltype(UnaryOperation<Type>()(Type()))>(x.shape());
      return operator()(x, out);
    }
    
    template <class Type, class OutputType>
    auto operator()(ndarray<Type>& x, ndarray<OutputType>& out) const
      -> ndarray<decltype(UnaryOperation<Type>()(Type()))>& {

      static_assert(std::is_same_v<OutputType, decltype(UnaryOperation<Type>()(Type()))>, "output operand of invalid type");
      auto x_copy = x.view;
      array_view::broadcast(out.view, x.view);
      std::transform(x.begin(), x.end(), out.begin(), UnaryOperation<Type>());
      x.view = std::move(x_copy);
      return out;
    }
    
  };
  

  template <template <class, class> class BinaryOperation>
  struct ufunc_binary: public ufunc {

    constexpr ufunc_binary() = default;

    template <class Type1, class Type2>
    auto operator()(ndarray<Type1>& x1, ndarray<Type2>& x2) const
      -> ndarray<decltype(BinaryOperation<Type1, Type2>()(Type1(), Type2()))> {

      auto outshape = array_view::get_broadcasted_shape(x1.view, x2.view);
      auto out = empty<decltype(BinaryOperation<Type1, Type2>()(Type1(), Type2()))>(outshape);
      return operator()(x1, x2, out);
    }
    
    template <class Type1, class Type2, class OutputType>
    auto operator()(ndarray<Type1>& x1, ndarray<Type2>& x2, ndarray<OutputType>& out) const
      -> ndarray<decltype(BinaryOperation<Type1, Type2>()(Type1(), Type2()))>& {

      static_assert(std::is_same_v<OutputType, decltype(BinaryOperation<Type1, Type2>()(Type1(), Type2()))>, "output operand of invalid type");

      
      auto x1_copy = x1.view;
      auto x2_copy = x2.view;
      array_view::broadcast(out.view, x1.view, x2.view);
      std::transform(x1.begin(), x1.end(), x2.begin(), out.begin(), BinaryOperation<Type1, Type2>());
      x1.view = std::move(x1_copy);
      x2.view = std::move(x2_copy);
      return out;      
    }
    
  };
}

#include <numpy/ufunc_instances.hpp>

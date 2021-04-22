#pragma once
#include <numpy/ndarray.hpp>
#include <numpy/array_math.hpp>
#include <numpy/io.hpp>
#include <numpy/algorithm.hpp>
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>
#include <type_traits>

using numpy::True;
using numpy::False;

namespace numpy {

  template <class dtype=void, class array_like>
  auto array(const array_like& object)
    -> typename std::remove_reference<decltype(object.begin(), object.end(),
					       std::declval<ndarray<typename std::conditional<std::is_same<dtype, void>::value, decltype(*(object.begin())), dtype>::type>>())>::type {
    /**
     * The original parameters: 
     *     object, Dtype=None, *, copy=True, order='K', subok=False, ndmin=0, like=None
     * `copy` will be supported before long. `order` and `subok` will not for now.
     */
    using Type = typename std::conditional<
      std::is_same<dtype, void>::value,
      typename std::remove_reference<decltype(*object.begin())>::type,
      dtype>::type;
    std::vector<Type> tmp;
    std::copy(object.begin(), object.end(), std::back_inserter(tmp));
    ndarray<Type> out(tmp, std::vector<shape_elem_type>(1, tmp.size()));
    return out;
  }

  template <class Dtype>
  auto array(const Dtype scolar)
    -> typename std::enable_if<std::is_arithmetic<Dtype>::value, ndarray<Dtype>>::type {
    return ndarray<Dtype>(scolar);
  }
  
  template <class dtype=void, class array_like>
  auto asarray(const array_like& a)
    -> ndarray<
    typename std::conditional<
    std::is_same<dtype, void>::value, typename std::remove_reference<
					decltype(*a.begin())>::type, dtype>::type>
  {  
    using Dtype = typename std::conditional<
      std::is_same<dtype, void>::value, typename std::remove_reference<decltype(*a.begin())>::type, dtype>::type;
    if constexpr(std::is_same<array_like, ndarray<Dtype>>::value) {
      return a;
    } else {
      return array<Dtype>(a);
    }
  }

  template <class Dtype>
  ndarray<Dtype> full(const shape_type& shape, Dtype fill_value) {
    return ndarray<Dtype>(std::vector<Dtype>(utils::product(shape), fill_value), shape);
  }

  template <class Dtype=float64>
  ndarray<Dtype> zeros(const shape_type& shape) {
    return full(shape, Dtype(0));
  }

  template <class Dtype=float64>
  ndarray<Dtype> ones(const shape_type& shape) {
    return full(shape, Dtype(1));
  }

  template <class Dtype=float64>
  ndarray<Dtype> empty(const shape_type& shape) {
    return ndarray<Dtype>(std::vector<Dtype>(utils::product(shape)), shape);
  }

  template <class Dtype, class Arg>
  ndarray<Dtype> reshape(const ndarray<Dtype>& a, const Arg& arg) {
    return a.reshape(arg);
  }

  template <class Dtype>
  ndarray<Dtype> transpose(const ndarray<Dtype>& a, const axes_type& axes) {
    return a.transpose(axes);
  }

  template <class Dtype>
  ndarray<Dtype> transpose(const ndarray<Dtype>& a) {
    return a.transpose();
  }

  template <class Dtype>
  ndarray<Dtype> arange(Dtype start, Dtype stop, Dtype step=Dtype(1)) {
    shape_elem_type size = std::ceil((stop - start) / step);
    auto result = empty<Dtype>({size});
    Dtype n = start;
    std::generate(result.begin(), result.end(),
		  [&n, step](){auto tmp = n; n += step; return tmp;});
    return result;
  }

  template <class Dtype>
  ndarray<Dtype> arange(Dtype stop) {
    return arange<Dtype>(0, stop, 1);
  }

  template <class Dtype=void>
  auto linspace(double start, double stop, intp num=50, bool endpoint=true)
    -> ndarray<typename std::conditional<std::is_same<Dtype, void>::value, float64, Dtype>::type> {
    using OutputType = typename std::conditional<std::is_same<Dtype, void>::value, float64, Dtype>::type;
    if (endpoint) {
      auto result = empty<OutputType>({num});
      intp i = 0;
      std::generate(result.begin(), result.end(),
		    [&i, start, stop, num](){return start + i++ * (stop - start) / (num - 1);});
      return result;
    }
    return linspace(start, stop - (stop - start) / num, num, true);
  }

  template <class Dtype>
  ndarray<Dtype> _at_least_2d_impl(const ndarray<Dtype>& ary) {
    return (ary.ndim() < 2) ? ary.reshape(1, ary.size()) : ary;
  }
    
  template <class... Dtype>
  std::tuple<ndarray<Dtype>...> at_least_2d(const ndarray<Dtype>&... arys) {
    return {_at_least_2d_impl(arys)...};
  }
  

  /* constants */
  constexpr double pi = M_PI;
  const double e = std::exp(1.0);

  /* explicit instantiation */
  // template class ndarray<byte>;
  // template class ndarray<ubyte>;
  // template class ndarray<short_>;
  // template class ndarray<ushort>;
  // template class ndarray<intc>;
  // template class ndarray<uintc>;
  // template class ndarray<int_>;
  // template class ndarray<uint>;
  // template class ndarray<longlong>;
  // template class ndarray<ulonglong>;
  // template class ndarray<single>;
  // template class ndarray<double_>;
  // template class ndarray<longdouble>;
  // template class ndarray<csingle>;
  // template class ndarray<cdouble>;
  // template class ndarray<clongdouble>;

  /* specailization for bool_ */
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator+(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator-(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator*(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator/(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator+=(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator-=(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator*=(const ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator/=(const ndarray<bool_>& rhs) = delete;

}

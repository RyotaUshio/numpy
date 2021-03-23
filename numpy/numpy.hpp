#pragma once
#include <numpy/ndarray.hpp>
#include <cmath>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace numpy {

  template <class dtype=void, class array_like>
  auto array(const array_like& object)
    -> decltype(object.begin(), object.end(),
		ndarray<typename std::conditional<std::is_same<dtype, void>::value, decltype(*(object.begin())), dtype>::type>()) {
    /**
     * The original parameters: 
     *     object, Dtype=None, *, copy=True, order='K', subok=False, ndmin=0, like=None
     * `copy` will be supported before long. `order` and `subok` will not for now.
     */
    using Type = typename std::conditional<std::is_same<dtype, void>::value, decltype(*(object.begin())), dtype>::type;
    std::vector<Type> tmp;
    std::copy(object.begin(), object.end(), std::back_inserter(tmp));
    ndarray<Type> out(tmp, std::vector<shape_elem_type>(1, tmp.size()));
    return out;
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

  //numpy.linspace(start, stop, num=50, endpoint=True, retstep=False, dtype=None, axis=0)
  template <class Dtype>
  ndarray<Dtype> linspace(Dtype start, Dtype stop, intp num=50, bool endpoint=true) {
    if (endpoint) {
      auto result = empty<Dtype>({num});
      intp i = 0;
      std::generate(result.begin(), result.end(),
		    [&i, start, stop, num](){return i++ * (stop - start) / static_cast<Dtype>(num - 1);});
      return result;
    }
    return linspace(start, stop, num + 1, true)[":-1"];
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
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator+(ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator-(ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator*(ndarray<bool_>& rhs) = delete;
  // template<> inline ndarray<bool_>& ndarray<bool_>::operator/(ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator+=(ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator-=(ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator*=(ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator/=(ndarray<bool_>& rhs) = delete;

}

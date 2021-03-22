#pragma once
#include <numpy/ndarray.hpp>
#include <cmath>
#include <vector>

namespace numpy {

  template <class array_like>
  auto array(const array_like& object)
    -> decltype(object.begin(), object.end(), ndarray<decltype(*(object.begin()))>()) {
    /**
     * The original parameters: 
     *     object, Dtype=None, *, copy=True, order='K', subok=False, ndmin=0, like=None
     * `copy` will be supported before long. `order` and `subok` will not for now.
     */
    using Dtype = decltype(*(object.begin()));
    std::vector<Dtype> tmp;
    std::copy(object.begin(), object.end(), std::back_inserter(tmp));
    ndarray<Dtype> out(tmp, std::vector<shape_elem_type>(1, tmp.size()));
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

  template <class Dtype>
  ndarray<Dtype> reshape(const ndarray<Dtype>& a, const shape_type& newshape) {
    return a.reshape(newshape);
  }

  template <class Dtype>
  ndarray<Dtype> transpose(const ndarray<Dtype>& a, const axes_type& axes) {
    return a.transpose(axes);
  }

  template <class Dtype>
  ndarray<Dtype> transpose(const ndarray<Dtype>& a) {
    return a.transpose();
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

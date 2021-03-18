#pragma once
#include <numpy/ndarray.hpp>
#include <cmath>


namespace numpy {

  // template <class dtype, class array_like>
  // ndarray<dtype> array(const array_like& object, dim_type ndmin=0, const array_like& like=None) {
  //   /**
  //    * The original parameters: 
  //    *     object, dtype=None, *, copy=True, order='K', subok=False, ndmin=0, like=None
  //    * `copy` will be supported before long. `order` and `subok` will not for now.
  //    */
  //   np.ndarray<dtype> out(object, 
  //   return out;
  // }

  template <class dtype>
  ndarray<dtype> full(const shape_type& shape, dtype fill_value) {
    return ndarray<dtype>(std::vector<dtype>(utils::product(shape), fill_value), shape);
  }

  template <class dtype=float64>
  ndarray<dtype> zeros(const shape_type& shape) {
    return full(shape, dtype(0));
  }

  template <class dtype=float64>
  ndarray<dtype> ones(const shape_type& shape) {
    return full(shape, dtype(1));
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
  template class ndarray<byte>;
  template class ndarray<ubyte>;
  template class ndarray<short_>;
  template class ndarray<ushort>;
  template class ndarray<intc>;
  template class ndarray<uintc>;
  template class ndarray<int_>;
  template class ndarray<uint>;
  template class ndarray<longlong>;
  template class ndarray<ulonglong>;
  template class ndarray<single>;
  template class ndarray<double_>;
  template class ndarray<longdouble>;
  template class ndarray<csingle>;
  template class ndarray<cdouble>;
  template class ndarray<clongdouble>;

  /* specailization for bool_ */
  template<> inline ndarray<bool_>& ndarray<bool_>::operator+=(const ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator-=(const ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator*=(const ndarray<bool_>& rhs) = delete;
  template<> inline ndarray<bool_>& ndarray<bool_>::operator/=(const ndarray<bool_>& rhs) = delete;

}

#pragma once

namespace np = numpy;

namespace scipy {

  namespace linalg {

    using python::slice;
    using python::str;
    
    template <class Dtype>
    using matrix = np::ndarray<Dtype>;
    template <class Dtype>
    using vector = np::ndarray<Dtype>;
    template <class Type>
    using Float = typename std::conditional<std::is_floating_point<Type>::value, Type, double>::type;

    constexpr np::float_ eps_default = 5.0e-15;

    template <class ArrayLike>
    auto norm(const ArrayLike& x, int ord=2) -> np::float64 {
      if (ord == 1)
	return np::sum(np::absolute(x));
      if (ord == 2)
	return std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), np::float_(0)));
      else
	return std::pow(np::sum(np::power(np::absolute(x), ord)), 1.0/ord);
    }
    
  }

}

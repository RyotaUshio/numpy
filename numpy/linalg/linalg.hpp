#pragma once

namespace numpy {

  namespace linalg {

    // template <class Dtype1, class Dtype2>
    // auto solve(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b) -> ndarray<float64> {
    //   // ...
    // }

    template <class ArrayLike>
    auto norm(const ArrayLike& x, int ord=2) -> float64 {
      if (ord == 1)
	return sum(absolute(x));
      if (ord == 2)
	return std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), 0));
    }
    
  } // namespace linalg
  
}

#pragma once

#include <scipy/linalg/solve.hpp>

namespace scipy {

  namespace linalg {

    // template <class Dtype1, class Dtype2>
    // auto solve(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b) -> ndarray<float64> {
    //   // ...
    // }

    template <class Dtype>
    std::tuple<matrix<Dtype>, vector<int>> lu_factor(const matrix<Dtype>& a, bool overwrite_a=false) {
      auto lu = LU_decomposition(a, overwrite_a);
      return {lu.LU, lu.p};
    }

    template <class Dtype>
    matrix<Dtype> lu_solve(const std::tuple<matrix<Dtype>, vector<int>>& lu_and_piv, const matrix<Dtype>& b, bool overwrite_b=False) {
      auto [lu, piv] = lu_and_piv;
      auto LU = LU_decomposition(lu, piv);
      return LU.solve(b, overwrite_b);
    }

    template <class Dtype>
    matrix<Dtype> cg(const matrix<Dtype>& a, const vector<Dtype>& b, np::float_ tol=eps_default) {
      auto CG = ConjugateGradient(a, b, tol);
      return CG.solve();
    }

    template <class ArrayLike>
    auto norm(const ArrayLike& x, int ord) -> np::float64 {
      if (ord == 1)
	return np::sum(np::absolute(x));
      if (ord == 2)
	return std::sqrt(std::inner_product(x.begin(), x.end(), x.begin(), np::float_(0)));
      else
	throw std::runtime_error("NotImplementedError");
    }

    template <class Dtype>
    auto inv(const matrix<Dtype>& a, bool overwrite_a=false) {
      assert(a.shape(0) == a.shape(1));
      auto I = np::identity(a.shape(0));
      auto LU = LU_decomposition(a, overwrite_a);
      return LU.solve(I);
    }
    
  } // namespace linalg
  
}

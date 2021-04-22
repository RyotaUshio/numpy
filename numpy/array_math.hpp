#pragma once

#include <numeric>
#include <algorithm>
#include <vector>
#include <tuple>

namespace numpy {

  template <class Dtype> ndarray<Dtype> empty(const shape_type& shape);
  template <class Dtype> ndarray<Dtype> zeros(const shape_type& shape);
  template <class... Dtype> std::tuple<ndarray<Dtype>...> at_least_2d(const ndarray<Dtype>&... arys);
  
  template <class Dtype>
  auto sum(const ndarray<Dtype>& a) -> Dtype {
    return std::accumulate(a.begin(), a.end(), Dtype(0));
  }

  template <class Dtype>
  auto sum(const ndarray<Dtype>& a, axis_type axis) -> ndarray<Dtype> {
    auto shape = a.shape();
    shape.erase(shape.begin() + axis);
    auto out = zeros<Dtype>(shape);
    auto tmp = utils::bring_axis_to_head(a, axis);
    for (int i = 0; i<tmp.shape(0); i++)
      out += tmp[i];
    return out;
  }

  template <class Dtype, class... Args>
  auto mean(const ndarray<Dtype>& a, Args... args) -> decltype(sum(a, args...)) {
    auto sum_ = sum(a, args...);
    return sum_ /= a.size();
  }

  template <class Dtype1, class Dtype2>
  auto dot(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b) 
    -> decltype(Dtype1() * Dtype2())
  {
    if (a.ndim() == b.ndim() == 1) {
      if (a.size() != b.size())
	throw std::invalid_argument("ValueError: shapes "
				    + python::str(a.shape()) + " and " + python::str(b.shape())
				    + " not aligned: "
				    + python::str(a.shape(0)) + " (dim 0) != "
				    + python::str(b.shape(0)) + " (dim 0)");
      
      // Broadcasting, constructing & destructing temporary objects are so heavy
      // that (1) is much slower than (2)
      
      // (1)
      // return sum(a * b);
      
      // (2)
      using OutputType = decltype(Dtype1() * Dtype2());
      return std::inner_product(a.begin(), a.end(), b.begin(), OutputType(0));
    }
    throw std::logic_error("NotImplementedError: dot product of 2+d arrays");
  }
  
  template <class Dtype1, class Dtype2>
  auto matmul(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b)
    -> ndarray<decltype(Dtype1() * Dtype2())> {
    
    if (a.ndim() * b.ndim() == 0) {
      throw std::invalid_argument("ValueError: matmul: Scolar is not allowed.");
    }
    
    using OutputType = decltype(Dtype1() * Dtype2());
    auto [a_2d] = at_least_2d(a);
    auto out = empty<OutputType>({a_2d.shape(a_2d.ndim() - 2), b.shape(b.ndim() - 1)});
    return matmul(a, b, out);
  }
  
  template <class Dtype1, class Dtype2, class OutputType>
  ndarray<OutputType>& matmul(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b, ndarray<OutputType>& out) {
    if (a.ndim() * b.ndim() == 0) {
      throw std::invalid_argument("ValueError: matmul: Scolar is not allowed.");
    }
			     
    if (a.ndim() == 1) {
      auto n = a.size();
      out = matmul(a.reshape(1, n), b).reshape(n);
      return out;
    }

    if (b.ndim() == 1) {
      auto n = b.size();
      out = matmul(a, b.reshape(n, 1)).reshape(n);
      return out;
    }    
    
    if (a.ndim() == 2 and b.ndim() == 2) {
      if (a.shape(1) != b.shape(0))
	throw std::invalid_argument("ValueError: matmul: Input operand 1 has a mismatch in its core dimension 0, with gufunc signature (n?,k),(k,m?)->(n?,m?) (size " + python::str(a.shape(1)) + " is different from " + python::str(b.shape(0)) + ")");
      
      auto b_T = b.T();      
      auto n = b_T.shape(0);
      int idx = -1;
      std::generate(out.begin(), out.end(),
      		    [&](){idx++; return dot(a(idx / n), b_T(idx % n));});
      return out;
      
      // auto m = a.shape(0);
      // auto n = b.shape(1);
      // for(int i=0; i<m; i++)
      // 	for(int j=0; j<n; j++)
      // 	  out[{i, j}] = dot(a(i), b_T(j));
      // return out;
    }
    
    // matmul of stack of matrices has not been implemented yet
    throw std::logic_error("NotImplementedError: application of matmul to stack of matrices");
    // https://numpy.org/doc/stable/reference/generated/numpy.matmul.html
    // - "If either argument is N-D, N > 2, it is treated as a stack of matrices residing in the last two indexes and broadcast accordingly."
  }
  
  template <class Dtype1, class Dtype2>
  auto _matmul_recursive(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b)
    -> ndarray<decltype(Dtype1() * Dtype2())> {
    auto l = a.shape(0);
    auto m = a.shape(1);
    auto n = b.shape(1);

    if (l > 1 or m > 1 or n > 1) {

      auto out = zeros<decltype(Dtype1() * Dtype2())>({l, n});
    
      auto a11 = a(python::slice(l/2), python::slice(m/2));
      auto a12 = a(python::slice(l/2), python::slice(m/2, m));
      auto a21 = a(python::slice(l/2, l), python::slice(m/2));
      auto a22 = a(python::slice(l/2, l), python::slice(m/2, m));

      auto b11 = b(python::slice(m/2), python::slice(n/2));
      auto b12 = b(python::slice(m/2), python::slice(n/2, n));
      auto b21 = b(python::slice(m/2, m), python::slice(n/2));
      auto b22 = b(python::slice(m/2, m), python::slice(n/2, n));

      if (a11.size() * b11.size())
	out(python::slice(l/2), python::slice(n/2)) += _matmul_recursive(a11, b11);
      if (a12.size() * b21.size())
	out(python::slice(l/2), python::slice(n/2)) += _matmul_recursive(a12, b21);
      if (a11.size() * b12.size())
	out(python::slice(l/2), python::slice(n/2, n)) += _matmul_recursive(a11, b12);
      if (a12.size() * b22.size())
	out(python::slice(l/2), python::slice(n/2, n)) += _matmul_recursive(a12, b22);
      if (a21.size() * b11.size())
	out(python::slice(l/2, l), python::slice(n/2)) += _matmul_recursive(a21, b11);
      if (a22.size() * b21.size())
	out(python::slice(l/2, l), python::slice(n/2)) += _matmul_recursive(a22, b21);
      if (a21.size() * b12.size())
	out(python::slice(l/2, l), python::slice(n/2, n)) += _matmul_recursive(a21, b12);
      if (a22.size() * b22.size())
	out(python::slice(l/2, l), python::slice(n/2, n)) += _matmul_recursive(a22, b22);

      return out;
      
    } else if (a.size() == 1 and b.size() == 1) {
      return a * b;
    }
    throw std::runtime_error("something's gone wrong");
  }

  template <class Dtype1, class Dtype2>
  auto _matmul_strassen(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b)
    -> ndarray<decltype(Dtype1() * Dtype2())> {
    auto l = a.shape(0);
    auto m = a.shape(1);
    auto n = b.shape(1);

    if (l > 1 or m > 1 or n > 1) {

      auto out = zeros<decltype(Dtype1() * Dtype2())>({l, n});
    
      auto a11 = a(python::slice(l/2), python::slice(m/2));
      auto a12 = a(python::slice(l/2), python::slice(m/2, m));
      auto a21 = a(python::slice(l/2, l), python::slice(m/2));
      auto a22 = a(python::slice(l/2, l), python::slice(m/2, m));

      auto b11 = b(python::slice(m/2), python::slice(n/2));
      auto b12 = b(python::slice(m/2), python::slice(n/2, n));
      auto b21 = b(python::slice(m/2, m), python::slice(n/2));
      auto b22 = b(python::slice(m/2, m), python::slice(n/2, n));

      auto s1 = b12 - b22;
      auto s2 = a11 + a12;
      auto s3 = a21 + a22;
      auto s4 = b21 - b11;
      auto s5 = a11 + a22;
      auto s6 = b11 + b22;
      auto s7 = a12 - a22;
      auto s8 = b21 + b22;
      auto s9 = a11 - a21;
      auto s10 = b11 + b12;

      auto p1 = _matmul_strassen(a11, s1);
      auto p2 = _matmul_strassen(s2, b22);
      auto p3 = _matmul_strassen(s3, b11);
      auto p4 = _matmul_strassen(a22, s4);
      auto p5 = _matmul_strassen(s5, s6);
      auto p6 = _matmul_strassen(s7, s8);
      auto p7 = _matmul_strassen(s9, s10);

      python::print_sep("\n", p1, p2, p3, p4, p5, p6, p7);

      out(python::slice(l/2), python::slice(n/2)) = p5 + p4 - p2 + p6;
      out(python::slice(l/2), python::slice(n/2)) = p1 + p2;
      out(python::slice(l/2), python::slice(n/2, n)) = p3 + p4;
      out(python::slice(l/2), python::slice(n/2, n)) = p5 + p1 - p3 - p7;

      return out;
      
    } else if (a.size() == 1 or b.size() == 1) {
      return a * b;
    }
    throw std::runtime_error("something's gone wrong");
  }
  
}

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

    /* direct solvers */
    
    template <class Dtype>
    void _forward_elimination_step(matrix<Dtype>& a, vector<int>& p) {
      // pivot selection
      auto abs_first_col = np::fabs(a(":", 0));
      auto itr_pivot = std::max_element(abs_first_col.begin(), abs_first_col.end());
      auto idx_pivot = itr_pivot.get_index();

      // swap
      if (idx_pivot) {
	auto tmp = a(idx_pivot).copy();
	a(idx_pivot) = a(0);
	a(0) = tmp;

	auto tmp_p = p[{idx_pivot}];
	p(idx_pivot) = p(0);
	p(0) = tmp_p;
      }
      
      // sweap
      auto sl = slice("1:");
      a(sl, 0) *= 1.0 / *a.begin();
      a(sl, sl) -= a(0, sl) * a(sl, slice(0, 1));
    }

    template <class Dtype>
    vector<int> _forward_elimination(matrix<Dtype>& Ab) {
      // Let us consider the following equation:
      //   Ax = b.
      // This function conducts forward elimination in-place.
      //
      // parameter:
      // Ab : coefficient matrix A or augmented coefficient matrix (A|b)
      
      // !add exception handling here!
      static_assert(std::is_floating_point_v<Dtype>);

      int i;
      auto n = Ab.shape(0);
      auto p = np::array(python::range(n)); // permutation

      for (i=0; i<n-1; i++) {
	auto sl = slice(str(i) + ":");
	auto lower_right_block = Ab(sl, sl);
	auto p_sliced = p(sl);
	_forward_elimination_step(lower_right_block, p_sliced);
      }

      return p;
    }

    template <class Dtype>
    void _backward_substitution_upper(const matrix<Dtype>& A, matrix<Dtype>& b, bool diag_is_1=false) {
      // conducts backward substitution in-place.
      //
      // parameters:
      // A : UPPER triangular coefficient matrix
      // b : constant vector

      auto n = A.shape(0);
      for (int i=n-1; i>=0; i--) {
	auto sl = slice(str(i+1) + ":");
	if (i < n-1) {
	  auto sum = np::matmul(A(i, sl).copy(), b(sl).copy());
	  b(i) -= sum;
	}
	if (not diag_is_1)
	  b(i) /= A(i, i);
      }
    }

    template <class Dtype>
    void _backward_substitution_lower(const matrix<Dtype>& A, matrix<Dtype>& b, bool diag_is_1=false) {
      // conducts backward substitution in-place.
      //
      // parameters:
      // A : LOWER triangular coefficient matrix
      // b : constant vector

      auto n = A.shape(0);
      for (int i=0; i<n; i++) {
	auto sl = slice(":" + str(i));
	if (i > 0) {
	  auto sum = np::matmul(A(i, sl).copy(), b(sl).copy());
	  b(i) -= sum;
	}
	if (not diag_is_1)
	  b(i) /= A(i, i);
      }      
    }

    template <class Dtype>
    struct LU_decomposition {

      matrix<Dtype> LU;
      vector<int> p;

      LU_decomposition(const matrix<Dtype>& LU_, const vector<int>& p_)
	: LU(LU_), p(p_) {}
      
      LU_decomposition(const matrix<Dtype>& a, bool overwrite_a=false) {
	if (overwrite_a)
	  LU = a;
	else
	  LU = a.copy();
	assert(LU.shape(0) == LU.shape(1));
	p = _forward_elimination(LU);
      }

      matrix<Dtype> L() const {
	auto ret = np::zeros(LU.shape());
      	int i, j;
      	int n = LU.shape(0);
	for (i=0; i<n; i++)
	  for (j=0; j<n; j++) {
	    if (i > j)
	      ret(i, j) = LU(i, j);
	    if (i == j)
	      ret(i, j) = 1.0;
	  }
	return ret;
      }

      matrix<Dtype> U() const {
	auto ret = np::zeros(LU.shape());
      	int i, j;
      	int n = LU.shape(0);
	for (i=0; i<n; i++)
	  for (j=0; j<n; j++)
	    if (i <= j)
	      ret(i, j) = LU(i, j);
	return ret;
      }

      matrix<Dtype> permute(const matrix<Dtype>& b, bool overwrite_b=false) const {
	auto tmp = b.copy();
	matrix<Dtype> rhs;
	if (overwrite_b)
	  rhs = b;
	else
	  rhs = np::empty(b.shape());

	auto itr = p.begin();
	auto end = p.end();
	int i = 0;
	for(; itr!=end; itr++, i++)
	  rhs(i) = tmp(*itr);

	return rhs;
      }

      matrix<Dtype> solve(const matrix<Dtype>& b, bool overwrite_b=false) const {	
	auto rhs = permute(b, overwrite_b);
	
	_backward_substitution_lower(LU, rhs, true);
	_backward_substitution_upper(LU, rhs, false);

	return rhs;
      }
    };

    template <class Dtype>
    struct Gaussian_elimination {

      matrix<Dtype> LU;
      matrix<Dtype> x;
      
      Gaussian_elimination(const matrix<Dtype>& ab, bool overwrite_ab=false) {
	if (overwrite_ab)
	  LU = ab;
	else
	  LU = ab.copy();
	assert(LU.shape(0) < LU.shape(1));

	_forward_elimination(LU);
	
	auto n = LU.shape(0);
	
      	auto slice_all = slice(":");
      	auto slice_left = slice(n);
      	auto slice_right = slice(str(n)+":");

      	auto a = LU(slice_all, slice_left);
      	auto b = LU(slice_all, slice_right);

      	_backward_substitution_upper(a, b, false);

	x = b;
      }

      Gaussian_elimination(const matrix<Dtype>& a, const matrix<Dtype>& b) {
	auto ab = np::empty({a.shape(0),
			     a.shape(1) + (b.ndim() == 1 ? 1 : b.shape(1))});

       	auto n = a.shape(1);
	auto slice_all = slice(":");
	auto slice_left = slice(n);
	auto slice_right = slice(str(n)+":");

	ab(slice_all, slice_left) = a;
	auto [b_2d] = at_least_2d(b);
	ab(slice_all, slice_right) = b_2d;
	
	*this = Gaussian_elimination(ab, true);
      }

      matrix<Dtype> solve() const {
	auto sol = x.copy();
	return sol.shape(1) == 1 ? sol.reshape(-1) : sol;
      }
    };


    // iterative solvers
    
    template <class ArrayLike>
    auto norm(const ArrayLike& x, int ord=2) -> np::float64;

    constexpr np::float_ eps_default = 1.0e-15;

    template <class Dtype>
    struct _iterative_solver {

      matrix<Dtype> a;
      vector<Dtype> b;
      np::float_ eps;
      np::float_ norm_b;
      int n;
            
      _iterative_solver(const matrix<Dtype>& a_, const vector<Dtype>& b_, np::float_ eps_=eps_default)
	: a(a_), b(b_), eps(eps_), norm_b(norm(b)), n(a.shape(1)) {}

      bool _converge(const vector<Dtype>& x, bool relative=true) const {
	auto residual = np::matmul(a, x) - b;
	auto error = norm(residual);
	if (relative) {
	  error /= norm_b;
	}
	return error < eps;
      }

      virtual void _solve_impl(vector<Dtype>& x) = 0;

      vector<Dtype> solve(const python::NoneType None=python::None) {
	auto x = np::zeros({this->n});
	_solve_impl(x);
	return x;
      }
      
      vector<Dtype> solve(const vector<Dtype>& x0) {
	auto x = x0.copy();
	_solve_impl(x);
	return x;
      }
    };

    template <class Dtype, bool _use_x2>
    struct _stationary_iterative_solver: public _iterative_solver<Dtype> {

      using _iterative_solver<Dtype>::_iterative_solver;

      template <class ArrayIterator>
      auto _get_new_x_i(ArrayIterator itr_a, ArrayIterator itr_b,
    			ArrayIterator itr_x_old, ArrayIterator itr_x_new,
    			int n, int i) -> typename ArrayIterator::value_type {
    	auto init = typename ArrayIterator::value_type(0);
    	auto sum = std::inner_product(itr_a, itr_a + i, itr_x_old, init);
    	sum += std::inner_product(itr_a + i+1, itr_a + n, itr_x_old + i+1, init);
    	return (*itr_b - sum) / *(itr_a + i);
      }

      using ArrayIterator = typename np::ndarray<Dtype>::iterator;
      virtual void _update_x_i(ArrayIterator itr_a, ArrayIterator itr_b,
    		       ArrayIterator itr_x, ArrayIterator itr_x2,
    			       int n, int i) = 0;

      void _solve_impl(vector<Dtype>& x) {
    	// preparation
	auto x2 = x; // buffer
    	auto n = this->a.shape(0);
    	auto itr_x = x.begin();
    	auto itr_x2 = itr_x;

    	if constexpr (_use_x2) {
    	    x2 = np::empty(x.shape());
    	    itr_x2 = x2.begin();
    	  }

    	// loop
    	while (not this->_converge(x)) {
	  print(x);
    	  auto itr_a = this->a.begin();
    	  auto itr_b = this->b.begin();
      
    	  for (int i=0; i<n; i++) {
    	    _update_x_i(itr_a, itr_b, itr_x, itr_x2, n, i);
    	    itr_a += n;
    	    ++itr_b;
    	  }

    	  if constexpr (_use_x2) { // Jacobi
    	      // swap
    	      auto tmp = itr_x;
    	      itr_x = itr_x2;
    	      itr_x2 = tmp;
    	    }  
    	}
      }
    };

    template <class Dtype>
    struct Jacobi: public _stationary_iterative_solver<Dtype, true> {

      Jacobi(const matrix<Dtype>& a_, const vector<Dtype>& b_, np::float_ eps_=eps_default)
      	: _stationary_iterative_solver<Dtype, true>(a_, b_, eps_) {}

      using ArrayIterator = typename np::ndarray<Dtype>::iterator;
      void _update_x_i(ArrayIterator itr_a, ArrayIterator itr_b,
    		       ArrayIterator itr_x, ArrayIterator itr_x2,
    		       int n, int i) override {
    	*(itr_x2 + i) = this->_get_new_x_i(itr_a, itr_b, itr_x, itr_x2, n, i);
      }
    };

    template <class Dtype>
    struct Gauss_Seidel: public _stationary_iterative_solver<Dtype, false> {

      Gauss_Seidel(const matrix<Dtype>& a_, const vector<Dtype>& b_, np::float_ eps_=eps_default)
    	: _stationary_iterative_solver<Dtype, false>(a_, b_, eps_) {}
      
      using ArrayIterator = typename np::ndarray<Dtype>::iterator;
      void _update_x_i(ArrayIterator itr_a, ArrayIterator itr_b,
    		       ArrayIterator itr_x, ArrayIterator itr_x2,
    		       int n, int i) override {
    	*(itr_x + i) = this->_get_new_x_i(itr_a, itr_b, itr_x, itr_x, n, i);
      }
    };

    template <class Dtype>
    struct SOR: public _stationary_iterative_solver<Dtype, false> {

      np::float_ omega;
      
      SOR(const matrix<Dtype>& a_, const vector<Dtype>& b_, np::float_ omega_, np::float_ eps_=eps_default)
    	: _stationary_iterative_solver<Dtype, false>(a_, b_, eps_), omega(omega_) {}
      
      using ArrayIterator = typename np::ndarray<Dtype>::iterator;
      void _update_x_i(ArrayIterator itr_a, ArrayIterator itr_b,
    		       ArrayIterator itr_x, ArrayIterator itr_x2,
    		       int n, int i) override {
    	auto y = this->_get_new_x_i(itr_a, itr_b, itr_x, itr_x, n, i);
    	*(itr_x + i) = *(itr_x + i) * (1 - omega) + y * omega;
      }
    };


    template <class Dtype>
    struct ConjugateGradient: public _iterative_solver<Dtype> {

      vector<Dtype> r; // residual = steepest decent direction
      vector<Dtype> p; // decent direction = A-orthogonalization of r
      np::float_ alpha; // step size
      
      ConjugateGradient(const matrix<Dtype>& a_, const vector<Dtype>& b_, np::float_ eps_=eps_default)
	: _iterative_solver<Dtype>(a_, b_, eps_) {}

      void _solve_impl(vector<Dtype>& x) override {
	// init
	auto n = this->a.shape(1);
	r = this->b - np::matmul(this->a, x);
	p = r;
	auto Ap = np::empty({n});
	np::float_ r_dot_p, p_dot_Ap;
	np::float_ beta;

	while (not this->_converge(x)) {
	  Ap = np::matmul(this->a, p);
	  r_dot_p = np::dot(r, p);
	  p_dot_Ap = np::dot(p, Ap);

	  alpha = r_dot_p / p_dot_Ap;
	  x += alpha * p;
	  r -= alpha * Ap;
      	  
	  r_dot_p = np::dot(r, p);

	  beta = r_dot_p / p_dot_Ap;
	  p = r - beta * p;
	}
      }
    };
  }

  
}

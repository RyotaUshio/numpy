#pragma once

namespace scipy {
  
  namespace linalg {

    namespace _eigen {
      
      // power method
      template <class Dtype>
      struct PowerMethod {
	
	const matrix<Dtype> a;
	vector<Dtype> eigvec;
	Dtype eigval;
	np::float_ eps;

	bool _converge(const vector<Dtype>& eigvec_new) const {
	  auto err = norm(eigvec_new - eigval*eigvec);
	  return err < eps; 
	}

	virtual vector<Dtype> _update_eigvec() const {
	  return np::matmul(a, eigvec);
	}

	virtual void set_eig() {  
	  while (true) {
	    auto eigvec_new = this->_update_eigvec();
	    if (_converge(eigvec_new))
	      break;
	    eigval = np::dot(eigvec, eigvec_new); // Rayleigh quotient
	    eigvec = eigvec_new;
	    eigvec /= norm(eigvec);
	  }
	}

	PowerMethod(const matrix<Dtype>& a_, const vector<Dtype>& eigvec0, np::float_ eps_=eps_default)
	  : a(a_), eigvec(eigvec0), eps(eps_) {}	
	
	PowerMethod(const matrix<Dtype>& a_, np::float_ eps_=eps_default)
	  : PowerMethod(a_, np::ones<Dtype>({a_.shape(1)}), eps_) {}
      
      };
      
      // inverse iteration
      template <class Dtype>
      struct InverseIteration: PowerMethod<Dtype> {
	
	_solve::LU_decomposition<Dtype> lu;
	Dtype shift;
	
	vector<Dtype> _update_eigvec() const override {
	  return lu.solve(this->eigvec); 
	}
	
	InverseIteration(const matrix<Dtype>& a_, const vector<Dtype>& eigvec0, Dtype shift_=0.0, np::float_ eps_=eps_default) : PowerMethod<Dtype>(a_ - shift_*np::identity(a_.shape(0)), eigvec0, eps_), lu(this->a), shift(shift_) {}
	  
	InverseIteration(const matrix<Dtype>& a_, Dtype shift_=0.0, np::float_ eps_=eps_default)
	  : InverseIteration(a_, np::ones<Dtype>({a_.shape(1)}), shift_, eps_) {}

	void set_eig() override {
	  PowerMethod<Dtype>::set_eig();
	  // At this point, this->eigval is an eigenvalue of inv(A - shift*I)
	  // So it must be corrected
	  this->eigval = 1.0 / this->eigval + shift;
	}
      };


      // QR method

      template <class Dtype>
      std::tuple<matrix<Dtype>, matrix<Dtype>> GramSchmidt(const matrix<Dtype>& a) {
	auto n = a.shape(1);
	
	// transpose for concise notation
	auto Q = a.T().copy();
	auto R = np::zeros({n, n});
	
	for (int j=0; j<n; j++) {
	  auto Q_j = Q(j);
	  auto R_jj = norm(Q_j);
	  
	  R(j, j) = R_jj;
	  Q_j /= R_jj;
	  
	  for (int k=j+1; k<n; k++) {
	    auto Q_k = Q(k);
	    auto R_kj = np::dot(Q_k, Q_j);
	    
	    R(k, j) = R_kj;
	    Q_k -= R_kj * Q_j;
	  }
	}
	
	return {Q.T(), R.T()};
      }

      
      template <class Dtype>
      struct QR {
	
      	const matrix<Dtype> a;
      	matrix<Dtype> eigvecs;
	vector<Dtype> eigvals;
      	np::float_ eps_eigvals;
	np::float_ eps_eigvecs;
	
      	QR(const matrix<Dtype>& a_, np::float_ eps_eigvals_=eps_default, np::float_ eps_eigvecs_=eps_default)
      	  : a(a_), eigvecs(np::zeros(a.shape())), eigvals(np::zeros({a.shape(0)})), eps_eigvals(eps_eigvals_), eps_eigvecs(eps_eigvecs_) {}

      	bool _converge(const vector<Dtype>& eigvals_new) const {
      	  auto err = norm(eigvals_new - eigvals, 1);
      	  return err < eps_eigvals; 
      	}

	bool _converge_min(const matrix<Dtype>& A) const {
	  if (A.shape(0) > 1) {
	    auto err = std::fabs(*(A.end()-2));
	    return err < eps_eigvals;
	  }
	  return true;
      	}

	void set_eigvals_with_deflation() {
	  auto A = a.copy();
	  
	  while (A.size())
	    while (true) {
	      auto [Q, R] = GramSchmidt(A);
	      np::matmul(R, Q, A);
	      auto min_eigval = A(-1, -1);
	      if (_converge_min(A)) {
		eigvals(A.shape(0)-1) = min_eigval;
		A = A(":-1", ":-1");
		break;
	      }
	    }
      	}

      	void set_eigvals_without_deflation() {
	  auto A = a.copy();
      	  while (true) {
      	    auto [Q, R] = GramSchmidt(A);
      	    np::matmul(R, Q, A);
	    
	    auto eigvals_new = np::diag(A);
	    if (_converge(eigvals_new))
	      break;
	    eigvals = eigvals_new;
      	  }
      	}

	void set_eigvals(bool deflate=true) {
	  if (deflate)
	    set_eigvals_with_deflation();
	  else
	    set_eigvals_without_deflation();
	}

	void set_eigvecs() {
	  int i = 0;
	  for (const auto& eigval : eigvals) {
	    InverseIteration<Dtype> inv_iter(a, eigval*1.01, eps_eigvecs);
	    inv_iter.set_eig();
	    eigvecs(":", i) = inv_iter.eigvec;
	    ++i;
	  }
	}

	void set_eig(bool deflate=true) {
	  set_eigvals(deflate);
	  set_eigvecs();
	}
      
      };

      
      
    }
    
  }
  
}

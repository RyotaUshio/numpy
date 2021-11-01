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

	virtual void eig() {  
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

	void eig() override {
	  PowerMethod<Dtype>::eig();
	  // At this point, this->eigval is an eigenvalue of inv(A - shift*I)
	  // So it must be corrected
	  this->eigval = 1.0 / this->eigval + shift;
	}
      };
      
    }
    
  }
  
}

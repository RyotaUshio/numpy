#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <utility>
#include <numpy/python.hpp>
#include <numpy/dtype.hpp>
#include <numpy/memory.hpp>
#include <numpy/viewinfo.hpp>
#include <numpy/broadcast.hpp>
#include <numpy/iterator.hpp>
#include <numpy/utils.hpp>
#include <numpy/typename.hpp>
#include <numpy/ufunc.hpp>

namespace numpy {
  
  template <typename Dtype>
  class ndarray {
    
  public:
    using iterator = array_iter<Dtype>;
    using viewinfo = array_view;
    
    template <typename AnotherDtype> friend class ndarray;
    friend array_iter<Dtype>;
    template <template <class> class UnaryOperation> friend struct ufunc_unary;
    template <template <class, class> class BinaryOperation> friend struct ufunc_binary;
    template <typename Dtype1, typename Dtype2> friend void broadcast(ndarray<Dtype1>& lhs, ndarray<Dtype2>& rhs);

  private:
    std::shared_ptr<shared_memory<Dtype>> memory_ptr;
    ndarray::viewinfo view;
    ndarray::viewinfo view_transpose;
    
  public:
    const ndarray<Dtype>& base;
    
    // accessors
    inline const shape_type& shape() const noexcept {
      return view.shape;
    }
    
    inline const shape_elem_type& shape(axis_type axis) const noexcept(false) {
      if (axis > ndim())
	throw std::out_of_range("IndexError: shape index out of range");
      return view.shape[axis];
    }
    
    inline const size_type& size() const noexcept {
      return view.size;
    }
    
    inline const dim_type& ndim() const noexcept {
      return view.ndim;
    }
    
    inline const std::type_info& dtype() const{
      return typeid(Dtype);
    }

    inline ndarray<Dtype> T() const {
      return ndarray<Dtype>(memory_ptr, view_transpose, base);
    }
    
    /* Constructors */
    ndarray(): base(*this) {}
    
  private:
    ndarray(const std::shared_ptr<shared_memory<Dtype>>& ptr, const viewinfo& view_, const ndarray<Dtype>& base_)
      : memory_ptr(ptr), view(view_), view_transpose(view.transpose()), base(base_) {}

    ndarray(shared_memory<Dtype> *ptr, const viewinfo& view_, const ndarray<Dtype>& base_)
      : ndarray<Dtype>(std::shared_ptr<shared_memory<Dtype>>(ptr), view_, base_) {}

    ndarray(const std::vector<Dtype>& data, const viewinfo& view_)
      : ndarray<Dtype>(new shared_memory<Dtype>(data), view_, *this) {}
    
  public:
    ndarray(const std::vector<Dtype>& data, const shape_type& shape_)
      : ndarray<Dtype>(data, viewinfo(shape_)) {}
    
    ndarray(Dtype* first, Dtype* last, const shape_type& shape_)
      : ndarray<Dtype>(std::vector<Dtype>(first, last), shape_) {}

    friend void swap(ndarray<Dtype>& first, ndarray<Dtype>& second) {
      using std::swap;
      // std::cout << "ndarray::swap is called" << std::endl;
      first.memory_ptr.swap(second.memory_ptr);
      swap(first.view, second.view);
      swap(first.view_transpose, second.view_transpose);
    }

    /**
     * Copy constructor & copy assignment operator
     * -------------------------------------------
     * The copied object should refer to the same memory address, but newly generate its view object.
     */
    
    ndarray(const ndarray<Dtype>& src)
      : ndarray<Dtype>(src.memory_ptr, src.view, src.base) {
      // std::cout << "ndarray::(copy constructor) is called" << std::endl;
    }

    /**
     * The elegant implementation of the copy assignment operator with the COPY-AND-SWAP IDIOM
     * cf) https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
     */
    
    // ndarray<Dtype>& operator=(ndarray<Dtype> rhs) {
    //   std::cout << "ndarray::(copy assignment operator) is called" << std::endl;
    //   swap(*this, rhs);
    //   return *this;
    // }

    /**
     * When a move assignment operator is also defined, however, IT WILL CAUSE A COMPLIE ERROR WHICH LOOKS LIKE BELOW:
     *
     * g++ -std=c++17 -O3 -Wall -I .  -o test/test_copy_move test/test_copy_move.cpp
     * test/test_copy_move.cpp:38:5: error: use of overloaded operator '=' is ambiguous (with operand types 'numpy::ndarray<double>' and 'ndarray<double>')
     *   c = b.transpose();
     *   ~ ^ ~~~~~~~~~~~~~
     * ./numpy/ndarray.hpp:104:21: note: candidate function
     *     ndarray<Dtype>& operator=(ndarray<Dtype> rhs) {
     *                     ^
     * ./numpy/ndarray.hpp:117:21: note: candidate function
     *     ndarray<Dtype>& operator=(ndarray<Dtype>&& rhs) noexcept {
     *                     ^
     * 1 error generated.
     * make: *** [test/test_copy_move] Error 1
     */

    /**
     * So, I'm obliged to throw away the graceful fasion above, and instead, choose the usual way of implmentation using a `const T&` parameter.
     */
        
    ndarray<Dtype>& operator=(const ndarray<Dtype>& rhs) {
      // std::cout << "ndarray::(copy assignment operator) is called" << std::endl;
      ndarray<Dtype> tmp(rhs);
      swap(*this, tmp);
      return *this;
    }
 
    /* Move constructor & move assignment operator */
    
    ndarray(ndarray<Dtype>&& src) noexcept
      : ndarray<Dtype>() {
      // std::cout << "ndarray::(move constructor) is called" << std::endl;
      swap(*this, src);
    }
      
    ndarray<Dtype>& operator=(ndarray<Dtype>&& rhs) noexcept {
      // std::cout << "ndarray::(move assignment operator) is called" << std::endl;
      swap(*this, rhs);
      return *this;
    }
    
    ndarray<Dtype> reshape(const shape_type& newshape) const {
      return ndarray<Dtype>(memory_ptr, viewinfo(newshape), base);
    }

    ndarray<Dtype> transpose(const std::vector<dim_type>& axes) const {
      return ndarray<Dtype>(memory_ptr, view.transpose(axes), base);
    }
    
    ndarray<Dtype> transpose() const {
      return T();
    }
    
    /* operators */

    // https://numpy.org/doc/stable/reference/ufuncs.html
    // Some of these ufuncs are called automatically on arrays when the relevant infix notation is used (e.g., add(a, b) is called internally when a + b is written and a or b is an ndarray).

    template <class Dtype2>
    auto operator+(ndarray<Dtype2>& rhs) {
      return add(*this, rhs);
    }
    
    template <class Dtype2>
    auto operator-(ndarray<Dtype2>& rhs) {
      return subtract(*this, rhs);
    }

    template <class Dtype2>
    auto operator*(ndarray<Dtype2>& rhs) {
      return multiply(*this, rhs);
    }

    template <class Dtype2>
    auto operator/(ndarray<Dtype2>& rhs) {
      return divide(*this, rhs);
    }

    ndarray<Dtype>& operator+=(ndarray<Dtype>& rhs) {
      return add(*this, rhs, *this);
    }

    ndarray<Dtype>& operator-=(ndarray<Dtype>& rhs) {
      return subtract(*this, rhs, *this);
    }

    ndarray<Dtype>& operator*=(ndarray<Dtype>& rhs) {
      return multiply(*this, rhs, *this);
    }

    ndarray<Dtype>& operator/=(ndarray<Dtype>& rhs) {
      return divide(*this, rhs, *this);
    }
    
    array_iter<Dtype> begin() const {
      return array_iter<Dtype>(*this);
    };
    
    array_iter<Dtype> end() const {
      return begin() + view.size;
    }

    // indexing
    template <class... Indexer>
    ndarray<Dtype> operator()(Indexer... indices) const {
      viewinfo newview = view.indexer(indices...);
      return ndarray<Dtype>(memory_ptr, newview, base);
    }    

    template <class AnotherDtype>
    ndarray<AnotherDtype> astype() const {
      // なんでだめ?
      // return ndarray<AnotherDtype>(std::vector<AnotherDtype>(begin(), end()), viewinfo<AnotherDtype>(view));
      std::vector<AnotherDtype> newdata(view.size);
      std::transform(begin(), end(), newdata.begin(), [](Dtype e){return static_cast<AnotherDtype>(e);});
      return ndarray<AnotherDtype>(newdata, viewinfo(view));
    }

    ndarray<Dtype> copy() const {
      // ndarray<Dtype> tmp;
      // std::copy(begin(), end(), std::back_inserter(tmp))
      std::vector<Dtype> tmp(size());
      std::copy(begin(), end(), tmp.begin());
      ndarray<Dtype> copied(tmp, shape());
      return copied;
    }

    const viewinfo& get_view() const {
      return view;
    }

    std::string memory_info() const {
      std::stringstream ss;
      ss << "<shared_memory at ";
      ss << memory_ptr;
      ss << ">";
      return ss.str();
    }

    // methods relevant to Python's built-in functions
    std::string __repr__() const // override
    {
      return "array(" + _str_rec(',') + ")";
    }

    std::string __str__() const // override
    {
      return _str_rec('\0');
    }
    
    std::string _str_rec(const char delimiter) const {
      if (view.ndim <= 0) 
	return python::str(memory_ptr->data[view.offset]);
      std::stringstream ss;
      ss << "[";
      long len = static_cast<long>(view.shape[0]);
      for (long i=0; i<len; i++) {
	if (i >= 1)
	  ss << "\t";
	ss << (*this)(i)._str_rec(delimiter);
	if (i < len - 1) {
	  ss << delimiter;
	  if (view.ndim >= 2)
	    ss << "\n";
	}
      }
      ss << "]";
      return ss.str();
    }

    shape_elem_type __len__() const {
      return shape(0);
    }
    
  };
  
}

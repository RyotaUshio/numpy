#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <utility>
#include <functional>
#include <numpy/python.hpp>
#include <numpy/dtype.hpp>
#include <numpy/memory.hpp>
#include <numpy/metadata.hpp>
#include <numpy/iterator.hpp>
#include <numpy/utils.hpp>
#include <numpy/typename.hpp>

namespace numpy {

  template <typename Dtype> class array_transpose;
  
  template <typename Dtype>
  class ndarray : public python::object {
  public:
    using iterator = array_iter<Dtype>;
    using view = array_view;
    
    template <typename AnotherDtype> friend class ndarray;
    friend array_iter<Dtype>;
    friend array_transpose<Dtype>;

  private:
    std::shared_ptr<shared_memory<Dtype>> memory_ptr;
    ndarray::view meta;
    
  public:
    // accessors
    inline const shape_type& shape() const noexcept {
      return meta.shape;
    }
    
    inline const shape_elem_type& shape(axis_type axis) const noexcept(false) {
      if (axis > ndim())
	throw std::out_of_range("IndexError: shape index out of range");
      return meta.shape[axis];
    }
    
    inline const size_type& size() const noexcept {
      return meta.size;
    }
    
    inline const dim_type& ndim() const noexcept {
      return meta.ndim;
    }
    
    inline const std::type_info& dtype() const{
      return typeid(Dtype);
    }

    array_transpose<Dtype> T;

    // constructors
  private:
    ndarray() = default;
    
    ndarray(const std::shared_ptr<shared_memory<Dtype>>& ptr, const array_view& meta_)
      : memory_ptr(ptr), meta(meta_), T(*this) {}

    ndarray(shared_memory<Dtype> *ptr, const array_view& meta_)
      : ndarray<Dtype>(std::shared_ptr<shared_memory<Dtype>>(ptr), meta_) {}

    ndarray(const std::vector<Dtype>& data, const array_view& meta_)
      : ndarray<Dtype>(new shared_memory<Dtype>(data), meta_) {}
    
  public:
    ndarray(const std::vector<Dtype>& data, const shape_type& shape_)
      : ndarray<Dtype>(data, array_view(shape_)) {}
    
    ndarray(Dtype* first, Dtype* last, const shape_type& shape_)
      : ndarray<Dtype>(std::vector<Dtype>(first, last), shape_) {}

    friend void swap(ndarray<Dtype>& first, ndarray<Dtype>& second) {
      using std::swap;
      swap(first.memory_ptr, second.memory_ptr);
      swap(first.meta, second.meta);
      swap(first.T, second.T);
    }

    // Copy constructor & copy assignment operator
    // The copied object should refer to the same memory address, but newly generate its view object.
    ndarray(const ndarray<Dtype>& src)
      : ndarray<Dtype>(src.memory_ptr, src.meta) {}

    // ndarray<Dtype>& operator=(ndarray<Dtype> rhs)
    //   : ndarray<Dtype>() {
    //   swap(*this, rhs);
    //   return *this;
    // }

    // // Move constructor & move assignment operator
    // ndarray(ndarray<Dtype>&& src) {
    //   swap(*this, src);
    // }
      
    // ndarray<Dtype>& operator=(ndarray<Dtype>&& rhs) {
    //   swap(*this, rhs);
    //   return *this;
    // }
    
    ndarray<Dtype> reshape(const shape_type& newshape) const {
      return ndarray<Dtype>(memory_ptr, array_view(newshape));
    }

    ndarray<Dtype> transpose(const std::vector<dim_type>& axes) const {
      return ndarray<Dtype>(memory_ptr, meta.transpose(axes));
    }
    
    ndarray<Dtype> transpose() const {
      return ndarray<Dtype>(memory_ptr, meta.transpose());
    }
    
    // operators
    template <class Operation>
    ndarray<Dtype>& comp_assign(const ndarray<Dtype>& rhs, Operation op) {
      // broadcast(rhs); ...
      std::transform(begin(), end(), rhs.begin(), begin(), op);
      return *this;
    }

    ndarray<Dtype>& operator+=(const ndarray<Dtype>& rhs) {
      return comp_assign(rhs, std::plus<Dtype>());
    }

    ndarray<Dtype>& operator-=(const ndarray<Dtype>& rhs) {
      return comp_assign(rhs, std::minus<Dtype>());
    }

    ndarray<Dtype>& operator*=(const ndarray<Dtype>& rhs) {
      return comp_assign(rhs, std::multiplies<Dtype>());
    }

    ndarray<Dtype>& operator/=(const ndarray<Dtype>& rhs) {
      return comp_assign(rhs, std::divides<Dtype>());
    }
    
    array_iter<Dtype> begin() const {
      return array_iter<Dtype>(*this);
    };
    
    array_iter<Dtype> end() const {
      return begin() + meta.size;
    }

    // indexing
    template <class... Indexer>
    ndarray<Dtype> operator()(Indexer... indices) const {
      array_view newmeta = meta.indexer(indices...);
      return ndarray<Dtype>(memory_ptr, newmeta);
    }    

    // other methods
    std::string __repr__() const override {
      return "array(" + _str_rec(',') + ")";
    }

    std::string __str__() const override {
      return _str_rec('\0');
    }
    
    std::string _str_rec(const char delimiter) const {
      if (meta.ndim <= 0) 
	return std::to_string(memory_ptr->data[meta.offset]);
      std::stringstream ss;
      ss << "[";
      long len = static_cast<long>(meta.shape[0]);
      for (long i=0; i<len; i++) {
	if (i >= 1)
	  ss << "\t";
	ss << (*this)(i)._str_rec(delimiter);
	if (i < len - 1) {
	  ss << delimiter;
	  if (meta.ndim >= 2)
	    ss << "\n";
	}
      }
      ss << "]";
      return ss.str();
    }

    template <class AnotherDtype>
    ndarray<AnotherDtype> astype() {
      // なんでだめ?
      // return ndarray<AnotherDtype>(std::vector<AnotherDtype>(begin(), end()), array_view<AnotherDtype>(meta));
      std::vector<AnotherDtype> newdata(meta.size);
      std::transform(begin(), end(), newdata.begin(), [](Dtype e){return static_cast<AnotherDtype>(e);});
      return ndarray<AnotherDtype>(newdata, array_view(meta));
    }

    // ndarray<Dtype> copy() {
    //   // return astype<Dtype>();
    // } 
  };

  
  template <class Dtype>
  class array_transpose {
    friend ndarray<Dtype>;
    
    const std::shared_ptr<shared_memory<Dtype>>& memory_ptr;
    const array_view meta;
    
    array_transpose(const ndarray<Dtype>& origin)
      : memory_ptr(origin.memory_ptr), meta(origin.meta.transpose()) {}

  public:
    array_view& get_meta() {
      return meta;
    }

    operator ndarray<Dtype>() const {
      return ndarray<Dtype>(memory_ptr, meta);
    }
  };
  
}

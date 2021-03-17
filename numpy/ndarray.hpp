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

  template <typename Type> class array_transpose;
  
  template <typename Type>
  class ndarray : public python::object {
    template <typename Dtype> friend class ndarray;
    friend array_iter<Type>;
    friend array_transpose<Type>;

  private:
    std::shared_ptr<shared_memory<Type>> memory_ptr;
    array_metadata<Type> meta;
    
  public:
    using iterator = array_iter<Type>;
    
    const shape_type& shape;
    const size_type& size;
    const dim_type& ndim;
    const std::type_info& dtype;

    array_transpose<Type> T;
  

    // constructors
  private:
    ndarray(const std::shared_ptr<shared_memory<Type>>& ptr, const array_metadata<Type>& meta_)
      : memory_ptr(ptr), meta(meta_), shape(meta.shape), size(meta.size), ndim(meta.ndim), dtype(meta.dtype), T(*this) {}

    ndarray(shared_memory<Type> *ptr, const array_metadata<Type>& meta_)
      : ndarray<Type>(std::shared_ptr<shared_memory<Type>>(ptr), meta_) {}

    ndarray(const std::vector<Type>& data, const array_metadata<Type>& meta_)
      : ndarray<Type>(new shared_memory<Type>(data), meta_) {}
    
  public:
    ndarray(const std::vector<Type>& data, const shape_type& shape_)
      : ndarray<Type>(data, array_metadata<Type>(shape_)) {}
    
    ndarray(Type* first, Type* last, const shape_type& shape_)
      : ndarray<Type>(std::vector<Type>(first, last), shape_) {}

    ndarray(const ndarray<Type>& src)
      : ndarray<Type>(src.memory_ptr, src.meta) {}

    ndarray<Type>& operator=(const ndarray<Type>& rhs) {
      return ndarray<Type>(rhs);
    }
    
    ndarray<Type> reshape(const shape_type& newshape) const {
      return ndarray<Type>(memory_ptr, array_metadata<Type>(newshape));
    }

    ndarray<Type> transpose(const std::vector<dim_type>& axes) const {
      return ndarray<Type>(memory_ptr, meta.transpose(axes));
    }
    
    ndarray<Type> transpose() const {
      return ndarray<Type>(memory_ptr, meta.transpose());
    }
    
    // operators
    template <class Operation>
    ndarray<Type>& comp_assign(const ndarray<Type>& rhs, Operation op) {
      // broadcast(rhs); ...
      std::transform(begin(), end(), rhs.begin(), begin(), op);
      return *this;
    }

    ndarray<Type>& operator+=(const ndarray<Type>& rhs) {
      return comp_assign(rhs, std::plus<Type>());
    }

    ndarray<Type>& operator-=(const ndarray<Type>& rhs) {
      return comp_assign(rhs, std::minus<Type>());
    }

    ndarray<Type>& operator*=(const ndarray<Type>& rhs) {
      return comp_assign(rhs, std::multiplies<Type>());
    }

    ndarray<Type>& operator/=(const ndarray<Type>& rhs) {
      return comp_assign(rhs, std::divides<Type>());
    }
    
    array_iter<Type> begin() const {
      return array_iter<Type>(*this);
    };
    
    array_iter<Type> end() const {
      return begin() + size;
    }

    // indexing
    template <class... Indexer>
    ndarray<Type> operator()(Indexer... indices) const {
      array_metadata<Type> newmeta = meta.indexer(indices...);
      return ndarray<Type>(memory_ptr, newmeta);
    }    

    // other methods
    std::string __repr__() const override {
      return "array(" + _str_rec(',') + ")";
    }

    std::string __str__() const override {
      return _str_rec('\0');
    }
    
    std::string _str_rec(const char delimiter) const {
      if (ndim <= 0) 
	return std::to_string(memory_ptr->data[meta.offset]);
      std::stringstream ss;
      ss << "[";
      long len = static_cast<long>(shape[0]);
      for (long i=0; i<len; i++) {
	if (i >= 1)
	  ss << "\t";
	ss << (*this)(i)._str_rec(delimiter);
	if (i < len - 1) {
	  ss << delimiter;
	  if (ndim >= 2)
	    ss << "\n";
	}
      }
      ss << "]";
      return ss.str();
    }

    template <class Dtype>
    ndarray<Dtype> astype() {
      // なんでだめ?
      // return ndarray<Dtype>(std::vector<Dtype>(begin(), end()), array_metadata<Dtype>(meta));
      std::vector<Dtype> newdata(size);
      std::transform(begin(), end(), newdata.begin(), [](Type e){return static_cast<Dtype>(e);});
      return ndarray<Dtype>(newdata, array_metadata<Dtype>(meta));
    }

    ndarray<Type> copy() {
      // return astype<Type>();
    }
    
  };

  
  template <class Type>
  class array_transpose {
    friend ndarray<Type>;
    
    const std::shared_ptr<shared_memory<Type>>& memory_ptr;
    const array_metadata<Type> meta;
    
    array_transpose(const ndarray<Type>& origin)
      : memory_ptr(origin.memory_ptr), meta(origin.meta.transpose()) {}

  public:
    array_metadata<Type>& get_meta() {
      return meta;
    }

    operator ndarray<Type>() const {
      return ndarray<Type>(memory_ptr, meta);
    }
  };
  
}

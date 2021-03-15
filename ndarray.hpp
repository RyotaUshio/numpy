#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include "memory.hpp"
#include "metadata.hpp"
// #include "iterator.hpp"
#include "utils.hpp"

namespace numpy {
  
  template <typename Type>
  class ndarray {
    // friend elementwise_iterator<Type>;

  public:
    // attributes
    std::shared_ptr<shared_memory<Type> > memory_ptr;
    array_metadata<Type> meta;
    
  public:
    const shape_type& shape;
    const size_type& size;
    const dim_type& ndim;
    const std::type_info& dtype;

    // const ndarray<Type>& T; // keep a reference to the transposed array (循環しちゃう??)
    
    // typedef elementwise_iterator<Type> iterator;

    // constructors

  private:
    ndarray(std::shared_ptr<shared_memory<Type> > ptr, array_metadata<Type> meta_)
      : memory_ptr(ptr), meta(meta_), shape(meta.shape), size(meta.size), ndim(meta.ndim), dtype(meta.dtype) {}

    ndarray(shared_memory<Type> *ptr, array_metadata<Type> meta_)
      : ndarray<Type>(std::shared_ptr<shared_memory<Type> >(ptr), meta_) {}

  public:
    ndarray(const std::vector<Type>& data, const shape_type& shape_)
      : ndarray<Type>(new shared_memory<Type>(data), array_metadata<Type>(shape_)) {}
    
    ndarray(Type* first, Type* last, shape_type shape_)
      : ndarray<Type>(std::vector<Type>(first, last), shape_) {}

    ndarray(const ndarray<Type>& src)
      : ndarray(src.memory_ptr, src.meta) {}

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
    // ndarray<Type>& operator+=(ndarray<Type> rhs) {
    //   if (not std::equal(shape, rhs.shape))
    // 	throw std::invalid_argument("Cannot add arrays with different shape");
      
    //   // iteratorが必要
    //   return *this;
    // }

    // elementwise_iterator<Type> begin() const {
    //   return elementwise_iterator<Type>(const_cast< ndarray<Type> * >(this));
    // };
    
    // elementwise_iterator<Type> end() const {
    //   return begin() + (size - 1) + 1;
    // };

    // indexing
    template <class... Indexer>
    ndarray<Type> operator()(Indexer... indices) const {
      array_metadata<Type> newmeta = meta.indexer(indices...);
      return ndarray<Type>(memory_ptr, newmeta);
    }    

    // other methods
    std::string __repr__() const {
      return "array(" + _str_rec(',') + ")";
    }

    std::string __str__() const {
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

    template <class dtype>
    ndarray<dtype> astype() {
      // iteratorほしい
      // return ndarray<dtype>(&(memory_ptr->astype<dtype>()), array_metadata<dtype>(shape_));
    }

    ndarray<Type> copy() {
      // return astype<Type>();
    }


    // for debug
    void printmeta() {
      meta.print();
    }
  };
}

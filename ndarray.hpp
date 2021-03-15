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
  
  template <typename T>
  class ndarray {
    // friend nd_iter<T>;

  public:
    // attributes
    std::shared_ptr<shared_memory<T> > memory_ptr;
    array_metadata<T> meta;
    
  public:
    const shape_type& shape;
    const size_type& size;
    const dim_type& ndim;
    const std::type_info& dtype;
    
    // typedef nd_iter<T> iterator;

    // constructors

  private:
    ndarray(std::shared_ptr<shared_memory<T> > ptr, array_metadata<T> meta_)
      : memory_ptr(ptr), meta(meta_), shape(meta.shape), size(meta.size), ndim(meta.ndim), dtype(meta.dtype) {}

    ndarray(shared_memory<T> *ptr, array_metadata<T> meta_)
      : ndarray<T>(std::shared_ptr<shared_memory<T> >(ptr), meta_) {}

  public:
    ndarray(const std::vector<T>& data, const shape_type& shape_)
      : ndarray<T>(new shared_memory<T>(data), array_metadata<T>(shape_)) {}
    
    ndarray(T* first, T* last, shape_type shape_)
      : ndarray<T>(std::vector<T>(first, last), shape_) {}

    ndarray(const ndarray<T>& src)
      : ndarray(src.memory_ptr, src.meta) {}

    ndarray<T> reshape(const shape_type& newshape) const {
      return ndarray<T>(memory_ptr, array_metadata<T>(newshape));
    }
    
    // operators
    // ndarray<T>& operator+=(ndarray<T> rhs) {
    //   if (not std::equal(shape, rhs.shape))
    // 	throw std::invalid_argument("Cannot add arrays with different shape");
      
    //   // iteratorが必要
    //   return *this;
    // }

    // nd_iter<T> begin() const {
    //   return nd_iter<T>(const_cast< ndarray<T> * >(this));
    // };
    
    // nd_iter<T> end() const {
    //   return begin() + (size - 1) + 1;
    // };

    // indexing
    template <class... Indexer>
    ndarray<T> operator()(Indexer... indices) const {
      array_metadata<T> newmeta = meta.indexer(indices...);
      return ndarray<T>(memory_ptr, newmeta);
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
	ss << (*this)(i)._str_rec(delimiter);
	if (i < len - 1)
	  ss << delimiter << "  ";
      }
      ss << "]";
      return ss.str();
    }

    template <class dtype>
    ndarray<dtype> astype() {
      // iteratorほしい
      // return ndarray<dtype>(&(memory_ptr->astype<dtype>()), array_metadata<dtype>(shape_));
    }

    ndarray<T> copy() {
      // return astype<T>();
    }


    // for debug
    void printmeta() {
      meta.print();
    }
  };
}

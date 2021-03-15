#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <cstdio>
#include "utils.hpp"
#include "metadata.hpp"

namespace numpy {
  template <typename T> class ndarray;
  
  template <typename T>
  class elementwise_iterator {
    using vec_itr = std::vector<T>::iterator;
    
    const array_metadata<T>& meta;
    vec_itr dataptr;

    elementwise_iterator(const array_metadata<T>& meta_, vec_itr dataptr_, int index_)
      : meta(meta_), dataptr(dataptr_), index(index_) {}

    elementwise_iterator(const ndarray<T>& array, int index_, std::size_t offset=0)
      : elementwise_iterator(array.meta, array.memory_ptr->data.begin() + offset, index_) {}
    
    T& operator*() {
      return *dataptr;
    }

    T operator*() const {
      return *dataptr;
    }

    elementwise_iterator<T>& operator+=(const std::size_t rhs) {
      
    }

    elementwise_iterator<T> operator+(const std::size_t rhs) const {
      elementwise_iterator<T> tmp(*this);
      tmp += rhs;
      return tmp;
    }
    
    elementwise_iterator<T>& operator++() { // pre-increment
      *this += 1;
      return *this;
    }
    
    elementwise_iterator<T> operator++(int) { // post-increment: takes a dummy parameter of int
      elementwise_iterator<T> tmp(*this);
      operator++();
      return tmp;
    }

    int operator-(const elementwise_iterator<T>& rhs) const {
      
    }
    
    bool operator!=(const elementwise_iterator<T>& rhs) const {
      
    }

  };
}

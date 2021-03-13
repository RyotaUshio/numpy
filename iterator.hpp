#pragma once
#include <iostream>
#include <tuple>
#include <cstdio>
#include "utils.hpp"

namespace numpy {
  template <typename T> class ndarray;

  
  template <typename T>
  struct base_iter {
    std::size_t index;
    ndarray<T>* array;
    int axis;
    std::size_t stride;
    
    base_iter(ndarray<T>* arr, int ax=-1, std::size_t offset=0)
      : array(arr), axis(ax) {
      index = arr->offset + offset;
      stride = arr->unit_stride;
      if (ax >= 0)
	stride *= utils::product(arr->shape, ax+1);
    }

    base_iter(const base_iter<T>& src) // copy constructor
      : base_iter(src.array, src.axis) {
      index = src.index;
    }

    void info() {
      std::cout << "array->shape = "; utils::print_vector(array->shape);
      std::cout << "array->size = " << array->size << std::endl;
      std::cout << "array->ndim = " << array->ndim << std::endl;
      std::cout << "array->dtype.name() = " << array->dtype.name() << std::endl;
      std::cout << "array->offset = " << array->offset << std::endl;
      std::cout << "array->unit_stride = " << array->unit_stride << std::endl;
      std::cout << "array->jump = " << std::boolalpha << array->jump << std::endl;
      if (array->jump) {
	std::cout << "array->jump_period = " << std::boolalpha << array->jump_period << std::endl;
	std::cout << "array->unit_jump = " << std::boolalpha << array->unit_jump << std::endl;
      }
      std::cout << "index = " << index << std::endl;
      std::cout << "axis = " << axis << std::endl;
      std::cout << "stride = " << stride << std::endl;
    }

    const T* raw_ptr() const {
      return &(array->memory_ptr->data[index]);
    }

    T& operator*() {
      return array->memory_ptr->data[index];
    }

    base_iter<T>& operator+=(const std::size_t rhs) {
      std::size_t incre = stride * rhs;

      // jump
      if (array->jump) {
	std::size_t tmp1, tmp2, tmp;
	std::size_t unit_jump = array->unit_jump; // array->jump_period * stride;
	tmp1 = (index - array->offset);
	tmp2 = tmp1 % unit_jump;
	tmp = tmp2 + incre;

	// printf("index = %lu, array->offset = %lu\n", index, array->offset);
	// printf("incre = %lu, tmp1 = (index - array->offset) = %lu, unit_jump = (array->jump_period * stride) = %lu, tmp2 = tmp1 %% unit_jump = %lu, tmp = tmp2 + incre = %lu\n", incre, tmp1, unit_jump, tmp2, tmp);
	
	std::size_t q, r;
	q = tmp / array->jump_period * stride;
	if (q) {
	  index += q * unit_jump;
	  index -= array->jump_period;
	}
	// printf("q = %lu, r = %lu\n", q, r);
      }
      
      index += incre;
      
      return *this;
    }

    long operator-(const base_iter<T>& rhs) const {
      return this->raw_ptr() - rhs.raw_ptr();
    }

    base_iter<T> operator+(const std::size_t rhs) const {
      base_iter<T> tmp(*this);
      tmp += rhs;
      return tmp;
    }
    
    base_iter<T>& operator++() { // pre-increment
      *this += 1;
      return *this;
    }
    
    base_iter<T> operator++(int) { // post-increment: takes a dummy parameter of int
      base_iter<T> tmp(*this);
      operator++();
      return tmp;
    }
    
    bool operator!=(const base_iter<T>& rhs) const {
      return (index != rhs.index);
    }

    bool operator==(const base_iter<T>& rhs) const {
      return (not (*this) != rhs);
    }
  };
}

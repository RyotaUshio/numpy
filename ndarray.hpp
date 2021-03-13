#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <stdexcept>
#include <typeinfo>
#include <boost/shared_ptr.hpp>
#include "memory.hpp"
#include "iterator.hpp"
#include "utils.hpp"

namespace numpy {
  typedef unsigned int uint;
  template <typename T> class base_iter;


  
  template <typename T>
  class ndarray {
    friend base_iter<T>;

  public:
    // attributes
    boost::shared_ptr<shared_memory<T> > memory_ptr;
    std::size_t offset; // head of the array
    std::size_t unit_stride;
    // std::size_t jump_distance;
    bool jump;
    std::size_t jump_period;
    std::size_t unit_jump;
    
  public:
    std::vector<std::size_t> shape;
    std::size_t size;
    std::size_t ndim;
    const std::type_info& dtype;
    typedef base_iter<T> iterator;

    // constructors

  private:
    ndarray(boost::shared_ptr<shared_memory<T> > ptr,
	    std::vector<std::size_t> shape_,
	    std::size_t offset_,
	    std::size_t unit_stride_,
	    std::size_t jump_period_=-1,
	    std::size_t unit_jump_=0)
      : memory_ptr(ptr), shape(shape_), offset(offset_), unit_stride(unit_stride_), jump_period(jump_period_), unit_jump(unit_jump_), dtype(typeid(T)) {
      size = utils::product(shape);
      ndim = shape.size();
      jump = false;
      if (ndim >= 2)
	jump = (jump_period != (std::size_t)-1) ? true : false;
    }

    ndarray(shared_memory<T> *ptr,
	    std::vector<std::size_t> shape_,
	    std::size_t offset_,
	    std::size_t unit_stride_,
	    std::size_t jump_period_=-1)
      : ndarray<T>(boost::shared_ptr<shared_memory<T> >(ptr), shape_, offset_, unit_stride_, jump_period_) {}

  public:
    ndarray(std::vector<T> data, std::vector<std::size_t> shape_)
      : ndarray<T>(new shared_memory<T>(data), shape_, 0, 1) {}
    
    ndarray(T* first, T* last, std::vector<std::size_t> shape_)
      : ndarray<T>(std::vector<T>(first, last), shape_) {}

    ndarray(const ndarray<T>& src)
      : ndarray(src.memory_ptr, src.shape, src.offset, src.unit_stride, src.jump_period, src.unit_jump) {}

    void reshape(std::vector<std::size_t> shape_) {
      std::size_t size_ = utils::product(shape_);
      if (size_ != size) 
	throw std::invalid_argument("Imcompatible size");
      shape = shape_;
      size = size_;
      ndim = shape_.size();
    }

    // operators
    // ndarray<T>& operator+=(ndarray<T> rhs) {
    //   if (not std::equal(shape, rhs.shape))
    // 	throw std::invalid_argument("Cannot add arrays with different shape");
      
    //   // iteratorが必要
    //   return *this;
    // }

    base_iter<T> begin() const {
      return base_iter<T>(const_cast< ndarray<T> * >(this));
    };
    
    base_iter<T> end() const {
      return begin() + (size - 1) + 1;
    };

    std::vector<std::size_t> index_to_shape(const std::vector<long> index) const {
      if (index.size() != ndim)
	throw std::invalid_argument("index.size must be == ndim");
      
      int axis;
      std::vector<std::size_t> new_shape;

      for(axis=0; axis<ndim; axis++) {
	if (index[axis] < 0)
	  new_shape.push_back(shape[axis]);
      }

      return new_shape;
    }
    
    std::size_t index_to_offset(const std::vector<long> index) const {
      if (index.size() != ndim)
	throw std::invalid_argument("index.size must be == ndim");
      
      int axis;
      std::size_t unit_offset = 1;
      auto itr = begin();
      
      for(axis=ndim-1; axis>=0; axis--) {
	if (index[axis] > 0)
	  itr += index[axis] * unit_offset;
	unit_offset *= shape[axis];
      }

      return itr.raw_ptr() - &(memory_ptr->data[0]);
    }

    std::size_t index_to_unit_stride(const std::vector<long> index) const {
      if (index.size() != ndim)
	throw std::invalid_argument("index.size must be == ndim");
      
      int axis;
      std::size_t scale = 1; // new_unit_stride = unit_stride;
      
      for(axis=ndim-1; axis>=0; axis--) {
	if (index[axis] >= 0) // ':' is represented by a negative number
	  // new_unit_stride *= shape[axis];
	  scale *= shape[axis];
	else
	  break;
      }
      if (scale >= size) {
	if (scale > size) 
	  throw std::range_error("Stride calculation failed");
	scale = 0;
      }

      return unit_stride * scale;
    }

    std::size_t index_to_jump_period(const std::vector<long> index) const {
      int last2 = index[ndim-2];
      int last1 = index[ndim-1];
      if (last1 < 0 and last2 >= 0)
	return shape[ndim-1];
      if (last2 < 0 and last1 >= 0)
	return shape[ndim-2];
      return -1;
    }

    ndarray<T> operator()(std::vector<long> index) const {
      while (index.size() < ndim)
	index.push_back(-1);
      
      std::vector<std::size_t> shape_ = index_to_shape(index);
      std::size_t offset_ = index_to_offset(index);
      std::size_t unit_stride_ = index_to_unit_stride(index);
      std::size_t jump_period_ = index_to_jump_period(index);

      ndarray<T> chunk(memory_ptr, shape_, offset_, unit_stride_, jump_period_, shape[ndim-2] * shape[ndim-1]);
      
      return chunk;
    }

    // deprecated
    ndarray<T> operator[](const std::size_t index) const {
      if (ndim <= 0)
    	throw std::out_of_range("Cannot subscript a 0-D array");

      std::vector<std::size_t> shape_(shape.begin() + 1, shape.end());
      std::size_t offset_ = offset + size / shape[0] * index;
      ndarray<T> first_axis(memory_ptr, shape_, offset_, unit_stride);
      return first_axis;
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
	return std::to_string(memory_ptr->data[offset]);
      std::stringstream ss;
      ss << "[";
      long len = static_cast<long>(shape[0]);
      for (long i=0; i<len; i++) {
	ss << (*this)({i})._str_rec(delimiter);
	if (i < len - 1)
	  ss << delimiter << "  ";
      }
      ss << "]";
      return ss.str();
    }

    template <typename dtype>
    ndarray<dtype> astype() {
      // shared_memoryのastypeが必要
    }
   
  };
}

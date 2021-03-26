// An iterator corresponding the original NumPy's "PyArrayIterObject"
// https://numpy.org/doc/stable/reference/c-api/types-and-structures.html#c.PyArrayIterObject

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <numpy/dtype.hpp>
#include <numpy/utils.hpp>
#include <numpy/viewinfo.hpp>
#include <numpy/memory.hpp>

namespace numpy {
  
  template <typename Dtype> class ndarray;
  
  template <typename Dtype>
  class array_iter {
    
    using coord_type = std::vector<dim_type>;
    using ptr = typename std::vector<Dtype>::iterator;

    const ndarray<Dtype>* array;
    ptr dataptr;
    int index; // current 1-d index
    coord_type coord; // current N-d index

    array_iter(const ndarray<Dtype>* array_,
	       const ptr& dataptr_,
	       const coord_type& coord_)
      : array(array_), dataptr(dataptr_) {
      set_coord(coord_);
    }

    array_iter(const ndarray<Dtype>* array_,
	       const ptr& dataptr_,
	       const int index_)
      : array(array_), dataptr(dataptr_) {
      set_index(index_);
    }

  public:
    array_iter(const ndarray<Dtype>* array_)
      : array_iter<Dtype>(array_, array_->memory_ptr->begin() + array_->view.offset, 0) {}
     
    void coord_to_index() {
      int unit = 1;
      index = 0;
      for (int i=array->view.ndim - 1; i>=0; i--) {
	index += coord[i] * unit;
	unit *= array->view.shape[i];
      }
    }

    void index_to_coord() {
      coord.resize(array->view.ndim);

      if (array->view.ndim) {
	int q, r;
	q = index / array->view.size;
	r = index % array->view.size;
	int i;

	for (i=0; i<array->view.ndim; i++)
	  coord[i] = q * array->view.shape[i];

	for (i=array->view.ndim - 1; i>=0; i--) {
	  coord[i] += r % array->view.shape[i];
	  r /= array->view.shape[i];
	}
      }
    }

    void set_coord(const coord_type& newcoord) {
      coord = newcoord;
      coord_to_index();
    }

    void set_index(const int newindex) {
      index = newindex;
      index_to_coord();
    }

    ptr coord_to_ptr(const coord_type& coord_) {
      return array->memory_ptr->begin() + utils::dot(array->view.stride, coord_, array->view.offset);
    }

    void go_to(const coord_type& dest) {
      // Originally "PyArray_ITER_GOTO (it, dest)"
      set_coord(dest);
      dataptr = coord_to_ptr(coord);
    }

    void go_to(const int dest_index) {
      // Originally "PyArray_ITER_GOTO1D(it, index)"
      set_index(dest_index);
      dataptr = coord_to_ptr(coord);
    }

  public:

    inline const Dtype* raw_ptr() const noexcept {
      return &(*dataptr);
    }

    Dtype& operator*() {
      return *dataptr;
    }

    Dtype operator*() const {
      return *dataptr;
    }

    array_iter<Dtype>& operator+=(const int n) {
      go_to(index + n);
      return *this;
    }

    array_iter<Dtype> operator+(const int n) const {
      array_iter<Dtype> tmp(*this);
      return tmp += n;
    }
    
    array_iter<Dtype>& operator++() { // pre-increment
      *this += 1;
      return *this;
    }
    
    array_iter<Dtype> operator++(int) { // post-increment: takes a dummy parameter of int
      array_iter<Dtype> tmp(*this);
      operator++();
      return tmp;
    }

    array_iter<Dtype>& operator-=(const int n) {
      return *this += -n; 
    }

    array_iter<Dtype> operator-(const int n) const {
      return *this + (-n);
    }
    
    bool operator!=(const array_iter<Dtype>& rhs) const {
      return (array != rhs.array) or (index != rhs.index);
    }
    
    bool operator==(const array_iter<Dtype>& rhs) const {
      return not (dataptr != rhs.dataptr);
    }

    std::string __repr__() const // override
    {
      std::stringstream ss;
      ss << "array_iterator<" << python::str(typeid(Dtype)) << ">(";
      ss << "array->view=" << python::repr(array->view);
      ss << ", array->memory_ptr=" << array->memory_ptr;
      ss << ", dataptr=" << raw_ptr();
      ss << ", index=" << index;
      ss << ", coord=" << python::str(coord);
      ss << ")";
      return ss.str();
    }
  };
  
}

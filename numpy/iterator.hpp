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
  class array_iter // : public python::object
  {
    
    using coord_type = std::vector<dim_type>;
    using ptr = typename std::vector<Dtype>::iterator;

    const array_view& view;
    const std::shared_ptr<shared_memory<Dtype>>& memory_ptr;
    ptr dataptr;
    int index; // current 1-d index
    coord_type coord; // current N-d index

    array_iter(const array_view& view_,
	       const std::shared_ptr<shared_memory<Dtype>>& memory_ptr_,
	       const ptr& dataptr_,
	       const coord_type& coord_)
      : view(view_), memory_ptr(memory_ptr_), dataptr(dataptr_) {
      set_coord(coord_);
    }

    array_iter(const array_view& view_,
	       const std::shared_ptr<shared_memory<Dtype>>& memory_ptr_,
	       const ptr& dataptr_,
	       const int index_)
      : view(view_), memory_ptr(memory_ptr_), dataptr(dataptr_) {
      set_index(index_);
    }

  public:
    array_iter(const ndarray<Dtype>& array)
      : array_iter<Dtype>(array, array.memory_ptr->begin() + array.view.offset, 0) {}

  private:
    template <class... Args>
    array_iter(const ndarray<Dtype>& array, const Args&... rest)
      : array_iter<Dtype>(array.view, array.memory_ptr, rest...) {}
     
    void coord_to_index() {
      int unit = 1;
      index = 0;
      for (int i=view.ndim - 1; i>=0; i--) {
	index += coord[i] * unit;
	unit *= view.shape[i];
      }
    }

    void index_to_coord() {
      coord.resize(view.ndim);
      int q, r;
      q = index / view.size;
      r = index % view.size;
      int i;

      for (i=0; i<view.ndim; i++)
	coord[i] = q * view.shape[i];

      for (i=view.ndim - 1; i>=0; i--) {
	coord[i] += r % view.shape[i];
	r /= view.shape[i];
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
      return memory_ptr->begin() + utils::dot(view.stride, coord_, view.offset);
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
      return dataptr != rhs.dataptr;
    }
    
    bool operator==(const array_iter<Dtype>& rhs) const {
      return not (dataptr != rhs.dataptr);
    }

    std::string __repr__() const // override
    {
      std::stringstream ss;
      ss << "array_iterator<" << python::str(typeid(Dtype)) << ">(";
      ss << "view=" << python::repr(view);
      ss << ", memory_ptr=" << memory_ptr;
      ss << ", dataptr=" << raw_ptr();
      ss << ", index=" << index;
      ss << ", coord=" << python::str(coord);
      ss << ")";
      return ss.str();
    }
  };
  
}

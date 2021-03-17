// An iterator corresponding the original NumPy's "PyArrayIterObject"
// https://numpy.org/doc/stable/reference/c-api/types-and-structures.html#c.PyArrayIterObject

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <numpy/pyobject.hpp>
#include <numpy/dtype.hpp>
#include <numpy/utils.hpp>
#include <numpy/metadata.hpp>
#include <numpy/memory.hpp>

namespace numpy {
  template <typename T> class ndarray;
  
  template <typename T>
  class array_iter : public python::object {
    using coord_type = std::vector<dim_type>;
    using ptr = typename std::vector<T>::iterator;

    const array_metadata<T>& meta;
    const std::shared_ptr<shared_memory<T> >& memory_ptr;
    ptr dataptr;
    int index; // current 1-d index
    coord_type coord; // current N-d index

    array_iter(const array_metadata<T>& meta_,
	       const std::shared_ptr<shared_memory<T> >& memory_ptr_,
	       const ptr& dataptr_,
	       const coord_type& coord_)
      : meta(meta_), memory_ptr(memory_ptr_), dataptr(dataptr_) {
      set_coord(coord_);
    }

    array_iter(const array_metadata<T>& meta_,
	       const std::shared_ptr<shared_memory<T> >& memory_ptr_,
	       const ptr& dataptr_,
	       const int index_)
      : meta(meta_), memory_ptr(memory_ptr_), dataptr(dataptr_) {
      set_index(index_);
    }

  public:
    array_iter(const ndarray<T>& array)
      : array_iter<T>(array, array.memory_ptr->data.begin() + array.meta.offset, 0) {}

  private:
    template <class... Args>
    array_iter(const ndarray<T>& array, const Args&... rest)
      : array_iter<T>(array.meta, array.memory_ptr, rest...) {}
     
    void coord_to_index() {
      int unit = 1;
      index = 0;
      for (int i=meta.ndim - 1; i>=0; i--) {
	index += coord[i] * unit;
	unit *= meta.shape[i];
      }
    }

    void index_to_coord() {
      coord.resize(meta.ndim);
      int q, r;
      q = index / meta.size;
      r = index % meta.size;
      int i;

      for (i=0; i<meta.ndim; i++)
	coord[i] = q * meta.shape[i];
     
      for (i=meta.ndim - 1; i>=0; i--) {
	coord[i] += r % meta.shape[i];
	r -= coord[i];
	r /= meta.shape[i];
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
      return memory_ptr->data.begin() + utils::dot(meta.stride, coord_, meta.offset);
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

    T& operator*() {
      return *dataptr;
    }

    T operator*() const {
      return *dataptr;
    }

    array_iter<T>& operator+=(const int n) {
      go_to(index + n);
      return *this;
    }

    array_iter<T> operator+(const int n) const {
      array_iter<T> tmp(*this);
      return tmp += n;
    }
    
    array_iter<T>& operator++() { // pre-increment
      *this += 1;
      return *this;
    }
    
    array_iter<T> operator++(int) { // post-increment: takes a dummy parameter of int
      array_iter<T> tmp(*this);
      operator++();
      return tmp;
    }

    array_iter<T>& operator-=(const int n) {
      return *this += -n; 
    }

    array_iter<T> operator-(const int n) const {
      return *this + (-n);
    }
    
    bool operator!=(const array_iter<T>& rhs) const {
      return dataptr != rhs.dataptr;
    }
    
    bool operator==(const array_iter<T>& rhs) const {
      return not (dataptr != rhs.dataptr);
    }

    std::string __repr__() const override {
      std::stringstream ss;
      ss << "array_iterator<" << typeid(T).name() << ">(";
      ss << "meta=" << repr(meta);
      ss << ", memory_ptr=" << memory_ptr;
      ss << ", dataptr=" << &(*dataptr);
      ss << ", index=" << index;
      ss << ", coord=" << utils::str(coord);
      ss << ")";
      return ss.str();
    }
  };
}

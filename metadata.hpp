#pragma once
#include <typeinfo> // for dtype
#include <vector>
#include <cmath> // abs
#include <stdexcept> // invalid_argument
#include "utils.hpp"
#include "slice.hpp"


namespace numpy {

  template <class T> class ndarray;
  
  typedef std::vector<int> stride_type;
  typedef std::vector<std::size_t> shape_type;
  typedef std::size_t size_type;
  typedef unsigned int dim_type;
  typedef std::size_t offset_type;

  template <class T>
  class array_metadata {
    friend ndarray<T>;
    
    shape_type shape;
    dim_type ndim;
    size_type size;
    offset_type offset;
    stride_type stride;
    const std::type_info& dtype;

    array_metadata(shape_type shape_, stride_type stride_, offset_type offset_=0)
      : shape(shape_), offset(offset_), stride(stride_), dtype(typeid(T)) {
      size = utils::product(shape);
      ndim = shape.size();
      if (stride.size() != ndim)
	throw std::invalid_argument("Stride and shape must have the same length.");
    }

    array_metadata(shape_type shape_, offset_type offset_=0)
      : array_metadata(shape_, stride_type(shape_.size(), 1), offset_) {}
    
    array_metadata(const array_metadata& src)
      : shape(src.shape), ndim(src.ndim), size(src.size), offset(src.offset), stride(src.stride), dtype(src.dtype) {}

    void reshape(const shape_type& newshape) {
      size_type size_ = utils::product(newshape);
      if (size_ != size) 
	throw std::invalid_argument("Imcompatible size");
      shape = newshape;
      size = size_;
      ndim = shape.size();
    }

    void _indexer_inplace_impl(dim_type axis) {}
    // template <class... Tail> void _indexer_inplace_impl(dim_type axis, int head, Tail... tail);
    // template <class... Tail> void _indexer_inplace_impl(dim_type axis, slice head, Tail... tail);
    
    template <class... Tail>
    void _indexer_inplace_impl(dim_type axis, slice head, Tail... tail) {
      offset += head.start(shape[axis]) * stride[axis];
      shape[axis] = std::abs(head.size(shape[axis]));
      stride[axis] *= head.step();
      _indexer_inplace_impl(axis + 1, tail...);
    }

    template <class... Tail>
    void _indexer_inplace_impl(dim_type axis, int head, Tail... tail) {
      offset += head * stride[axis];
      shape.erase(shape.begin() + axis);
      stride.erase(stride.begin() + axis);
      _indexer_inplace_impl(axis + 1, tail...);
    }

    template <class... Index>
    void indexer_inplace(Index... indices) {
      _indexer_inplace_impl(0, indices...);
    }

    template <class... Index>
    array_metadata<T> indexer(Index... indices) const {
      array_metadata<T> newmeta(*this);
      newmeta.indexer_inplace(indices...);
      return newmeta;
    }    
  };
  
}

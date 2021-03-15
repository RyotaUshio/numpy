#pragma once
#include <typeinfo> // for dtype
#include <vector>
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
      : shape(shape_), stride(stride_), offset(offset_), dtype(typeid(T)) {
      adjust_to_shape();
      if (stride.size() != ndim)
	throw std::invalid_argument("Stride and shape must have the same length.");
    }

    array_metadata(shape_type shape_, offset_type offset_=0)
      : shape(shape_), offset(offset_), dtype(typeid(T)) {
      adjust_to_shape();
      stride = stride_type(shape.size());
      stride[ndim-1] = 1;
      for(int i=ndim-2; i>=0; i--){
	stride[i] = stride[i+1] * shape[i+1];
      }
    }
    
    array_metadata(const array_metadata& src)
      : shape(src.shape), ndim(src.ndim), size(src.size), offset(src.offset), stride(src.stride), dtype(src.dtype) {}

    void set_shape(const shape_type& newshape) {
      shape = newshape;
      adjust_to_shape();
    }

    void adjust_to_shape() {
      size = utils::product(shape);
      ndim = shape.size();
    }

    void _indexer_inplace_impl(dim_type axis) {}
    
    template <class... Tail>
    void _indexer_inplace_impl(dim_type axis, slice head, Tail... tail) {
      offset += head.abs_start(shape[axis]) * stride[axis];
      shape[axis] = head.size(shape[axis]);
      stride[axis] *= head.step;
      _indexer_inplace_impl(axis + 1, tail...);
    }

    template <class... Tail>
    void _indexer_inplace_impl(dim_type axis, int head, Tail... tail) {
      offset += head * stride[axis];
      shape.erase(shape.begin() + axis);
      stride.erase(stride.begin() + axis);
      _indexer_inplace_impl(axis, tail...);
    }

    template <class... Index>
    void indexer_inplace(Index... indices) {
      if (sizeof...(indices) > ndim) {
	throw std::invalid_argument("IndexError: too many indices for array: array is " + std::to_string(ndim) + "-dimensional, but " + std::to_string(sizeof...(indices)) + " were indexed");
      }      
      _indexer_inplace_impl(0, indices...);
      adjust_to_shape();
    }

    template <class... Index>
    array_metadata<T> indexer(Index... indices) const {
      array_metadata<T> newmeta(*this);
      newmeta.indexer_inplace(indices...);
      return newmeta;
    }

    // for debug
    void print() {
      std::cout << "shape: "; utils::print_vector(shape);
      std::cout << "size: " << size << std::endl;
      std::cout << "ndim: " << ndim << std::endl;
      std::cout << "offset: " << offset << std::endl;
      std::cout << "stride: "; utils::print_vector(stride);
    }
  };
  
}

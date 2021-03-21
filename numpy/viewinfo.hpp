#pragma once
#include <typeinfo> // for dtype
#include <vector>
#include <stdexcept> // invalid_argument
#include <algorithm> // find
#include <string>
#include <sstream>
#include <utility> // swap
#include <numpy/dtype.hpp>
#include <numpy/utils.hpp>
#include <numpy/python.hpp>


namespace numpy {
  
  class array_view {
    
    template <class Dtype> friend class ndarray;
    template <class Dtype> friend class array_transpose;
    template <class Dtype> friend class array_iter;
    template <template <class, class> class BinaryOperation> friend struct ufunc_binary;

    template <class ArrayViewHead, class... ArrayViewTail>
    friend void broadcast(ArrayViewHead out_view, ArrayViewTail&... views);
    friend const shape_elem_type& broadcast_axis(const shape_elem_type& a, const shape_elem_type& b) noexcept(false);    
    template <class ArrayViewHead, class... ArrayViewTail>
    friend void get_broadcasted_shape_impl(shape_type& newshape,
				     const ArrayViewHead& head, const ArrayViewTail&... tail);
    template <class... ArrayView> friend dim_type max_ndim(const ArrayView&... views);
    template <class... ArrayView> friend bool is_same_shape(const ArrayView&... views);
    template <class ArrayView> friend void adjust_views(dim_type ndim, ArrayView& view);
    
    shape_type shape;
    dim_type ndim;
    size_type size;
    offset_type offset;
    stride_type stride;

  public:
    array_view() = default;
    
    array_view(const shape_type& shape_, const stride_type& stride_, const offset_type& offset_=0)
      : shape(shape_), offset(offset_), stride(stride_) {
      adjust_to_shape();
      if (stride.size() != ndim)
	throw std::invalid_argument("Stride and shape must have the same length.");
    }

    array_view(const shape_type& shape_, const offset_type& offset_=0)
      : shape(shape_), offset(offset_) {
      adjust_to_shape();
      stride = stride_type(ndim);
      stride[ndim-1] = 1;
      for(int i=ndim-2; i>=0; i--){
	stride[i] = stride[i+1] * shape[i+1];
      }
    }

    array_view(const array_view& src)
      : shape(src.shape), ndim(src.ndim), size(src.size), offset(src.offset), stride(src.stride) {}

  private:
    friend void swap(array_view& a, array_view& b) {
      using std::swap;
      swap(a.shape, b.shape);
      swap(a.ndim, b.ndim);
      swap(a.size, b.size);
      swap(a.offset, b.offset);
      swap(a.stride, b.stride);
      // std::cout << "array_view::swap is called" << std::endl;
    }
    
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
    void _indexer_inplace_impl(dim_type axis, python::slice head, Tail... tail) {
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
    array_view indexer(Index... indices) const {
      array_view newview(*this);
      newview.indexer_inplace(indices...);
      return newview;
    }

    array_view transpose(const std::vector<dim_type>& axes) const {
      if (axes.size() != ndim)
	throw std::invalid_argument("ValueError: axes don't match array");
      shape_type newshape;
      stride_type newstride;
      for (const auto e : axes) {
	newshape.push_back(shape[e]);
	newstride.push_back(stride[e]);
      }
      return array_view(newshape, newstride);
    }

    array_view transpose() const {
      std::vector<dim_type> axes(ndim);
      for(int i=0; i<ndim; i++)
	axes[i] = ndim - 1 - i;
      return transpose(axes);
    }
    
    void broadcast_to(const shape_type& newshape) {
      for(axis_type ax=0; ax<shape.size(); ax++)
	if(shape[ax] != newshape[ax]) {
	  shape[ax] = newshape[ax];
	  stride[ax] = 0;
	}
      adjust_to_shape();
    }

  public:
    // for debug
    std::string __repr__() const // override
    {
      std::stringstream ss;
      ss << "array_view(";
      ss << "shape=" << python::str(shape);
      ss << ", size=" << size;
      ss << ", ndim=" << ndim;
      ss << ", offset=" << offset;
      ss << ", stride=" << python::str(stride);
      ss << ")";
      return ss.str();
    }
  };
  
}

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
    template <class Dtype> friend class array_iter;
    template <template <class> class UnaryOperation> friend struct ufunc_unary;
    template <template <class, class> class BinaryOperation> friend struct ufunc_binary;
    
    shape_type shape;
    dim_type ndim;
    size_type size;
    offset_type offset;
    stride_type stride;

    friend void swap(array_view& a, array_view& b) {
      using std::swap;
      swap(a.shape, b.shape);
      swap(a.ndim, b.ndim);
      swap(a.size, b.size);
      swap(a.offset, b.offset);
      swap(a.stride, b.stride);
    }

  public:
    array_view() = default;
    
    array_view(const shape_type& shape_, const stride_type& stride_, const offset_type& offset_=0)
      : shape(shape_), offset(offset_), stride(stride_) {
      adjust_to_shape();
      if (stride.size() != ndim)
	throw std::invalid_argument("Stride and shape must have the same length.");
    }

    inline void shape_to_stride() {
      stride = stride_type(ndim);
      if (ndim) {
	stride[ndim-1] = 1;
	for(int i=ndim-2; i>=0; i--){
	  stride[i] = stride[i+1] * shape[i+1];
	}
      }
    }

    array_view(const shape_type& shape_, const offset_type offset_=0)
      : shape(shape_), offset(offset_) {
      adjust_to_shape();
      shape_to_stride();
    }

    array_view(shape_type&& shape_, const offset_type offset_=0)
      : shape(shape_), offset(offset_) {
      adjust_to_shape();
      shape_to_stride();
    }

    array_view(const array_view& src)
      : shape(src.shape), ndim(src.ndim), size(src.size), offset(src.offset), stride(src.stride) {
    }

    array_view& operator=(const array_view& rhs) {
      array_view tmp(rhs);
      swap(*this, tmp);
      return *this;
    }
 
    /* Move constructor & move assignment operator */
    
    array_view(array_view&& src) noexcept
      : array_view() {
      swap(*this, src);
    }
      
    array_view& operator=(array_view&& rhs) noexcept {
      swap(*this, rhs);
      return *this;
    }

  private:    
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
      offset += python::slice::abs_index(shape[axis], head) * stride[axis];
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

    array_view transpose(const axes_type& axes) const {
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
      auto new_ndim = newshape.size();
      shape.insert(shape.begin(), new_ndim - ndim, 1);
      stride.insert(stride.begin(), new_ndim - ndim, 0);
      for(axis_type ax=0; ax<new_ndim; ax++)
	if(shape[ax] != newshape[ax]) {
	  shape[ax] = newshape[ax];
	  stride[ax] = 0;
	}
      adjust_to_shape();
    }

  public:
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
    
    
    /** static methods for broadcasting **/
    
    template <class... ArrayView>
    static inline bool is_same_shape(const ArrayView&... views) {
      return (views.ndim == ...);
    }
  
    template <class... ArrayView>
    static inline dim_type max_ndim(const ArrayView&... views) {
      auto view_with_max_ndim = std::max({views...}, [](const auto& a, const auto& b){return a.ndim < b.ndim;});
      return view_with_max_ndim.ndim;
    }

    template <class ArrayView>
    static inline void adjust_views(dim_type ndim, ArrayView& view) {
      view.shape.insert(view.shape.begin(), ndim - view.ndim, 1);
    }

    static const shape_elem_type& broadcast_axis(const shape_elem_type& a, const shape_elem_type& b) noexcept(false) {
      if (a == b)
	return a;
      if (a == 1)
	return b;
      if (b == 1)
	return a;
      throw std::invalid_argument("hoge");
    }

    template <class ArrayViewHead, class... ArrayViewTail>
    static inline void get_broadcasted_shape_impl(shape_type& newshape,
						  const ArrayViewHead& head, const ArrayViewTail&... tail) {
      try {
	std::transform(newshape.begin(), newshape.end(),
		       head.shape.begin(), newshape.begin(),
		       broadcast_axis);
      } catch (const std::invalid_argument& e) {
	throw std::invalid_argument("ValueError: operands could not be broadcast together");
      }
 
      if constexpr (sizeof...(tail) > 0) {
	  get_broadcasted_shape_impl(newshape, tail...);
	}
    }

    template <class... ArrayView>
    static shape_type get_broadcasted_shape(ArrayView... views) {
      auto ndim = max_ndim(views...);

      // swallow
      // https://qiita.com/_EnumHack/items/677363eec054d70b298d#swallow
      using swallow = std::initializer_list<int>;
      (void)swallow{ (adjust_views(ndim, views), 0)... };

      shape_type newshape(ndim, 1);
      get_broadcasted_shape_impl(newshape, views...);
      return newshape;
    }

    template <class ArrayViewHead, class... ArrayViewTail>
    static void broadcast(ArrayViewHead out_view, ArrayViewTail&... views) {
      if (is_same_shape(out_view, views...)) // no need to broadcast
	return;	
      auto newshape = get_broadcasted_shape(out_view, views...);
      if (newshape != out_view.shape) {
	throw std::invalid_argument("ValueError: non-broadcastable output operand with shape "
				    + python::str(out_view.shape)
				    + " doesn't match the broadcast shape "
				    + python::str(newshape));
      }
      using swallow = std::initializer_list<int>;
      (void)swallow{(views.broadcast_to(newshape), 0)...};
    }

  };
  
}


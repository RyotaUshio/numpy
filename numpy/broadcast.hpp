#pragma once

namespace numpy {

  
  template <class... ArrayView>
  inline bool is_same_shape(const ArrayView&... views) {
    return (views.ndim == ...);
  }
  
  template <class... ArrayView>
  inline dim_type max_ndim(const ArrayView&... views) {
    auto view_with_max_ndim = std::max({views...}, [](const auto& a, const auto& b){return a.ndim < b.ndim;});
    return view_with_max_ndim.ndim;
  }

  template <class ArrayView>
  inline void adjust_views(dim_type ndim, ArrayView& view) {
    view.shape.insert(view.shape.begin(), ndim - view.ndim, 1);
  }

  const shape_elem_type& broadcast_axis(const shape_elem_type& a, const shape_elem_type& b) noexcept(false) {
    if (a == b)
      return a;
    if (a == 1)
      return b;
    if (b == 1)
      return a;
    throw std::invalid_argument("hoge");
  }

  template <class ArrayViewHead, class... ArrayViewTail>
  inline void get_broadcasted_shape_impl(shape_type& newshape,
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
  shape_type get_broadcasted_shape(ArrayView... views) {
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
  void broadcast(ArrayViewHead out_view, ArrayViewTail&... views) {
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

  
  // namespace {
  
  // const shape_elem_type& broadcast_axis(const shape_elem_type& a, const shape_elem_type& b) noexcept(false) {
  //   if (a == b)
  //     return a;
  //   if (a == 1)
  //     return b;
  //   if (b == 1)
  //     return a;
  //   throw "cannot broadcast";
  // }

  // shape_type get_broadcasted_shape(array_view& a, array_view& b) {
  //   auto [small, big] = std::minmax(a, b, [](const auto& view1, const auto& view2) {return view1.ndim < view2.ndim;});
  //   const_cast<shape_type&>(small.shape).insert(small.shape.begin(), big.ndim - small.ndim, 1);
  //   shape_type newshape;
  //   try {
  //     std::transform(small.shape.begin(), small.shape.end(), big.shape.begin(), std::back_inserter(newshape), broadcast_axis);
  //   } catch (const char*) {
  //     throw std::invalid_argument("ValueError: operands could not be broadcast together with shapes "
  // 				  + python::str(a.shape) + " " + python::str(b.shape));
  //   }
  //   return newshape;
  //   // return std::move(newshape);
  //   // https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value
  //   // --- "When returning a named local variable or a temporary expression directly, you should avoid the explicit std::move. The compiler must (and will in the future) move automatically in those cases, and adding std::move might affect other optimizations."
  // }
  
  // template <typename Dtype1, typename Dtype2>
  // void broadcast(ndarray<Dtype1>& lhs, ndarray<Dtype2>& rhs) {
  //   if (lhs.shape() == rhs.shape()) // no need to broadcast
  //     return;
  //   auto newshape = get_broadcasted_shape(lhs.view, rhs.view);
  //   // change shape & stride
  //   lhs.view.broadcast_to(newshape);
  //   rhs.view.broadcast_to(newshape);
  // }

  // }
}

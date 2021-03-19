#pragma once

namespace numpy {
  
  const shape_elem_type& broadcast_axis(const shape_elem_type& a, const shape_elem_type& b) noexcept(false) {
    if (a == b)
      return a;
    if (a == 1)
      return b;
    if (b == 1)
      return a;
    throw "Cannot broadcast";
  }

  shape_type get_broadcasted_shape(array_view& a, array_view& b) {
    auto [small, big] = std::minmax(a, b, [](const auto& view1, const auto& view2) {return view1.ndim < view2.ndim;});
    const_cast<shape_type&>(small.shape).insert(small.shape.begin(), big.ndim - small.ndim, 1);
    shape_type newshape;
    std::transform(small.shape.begin(), small.shape.end(), big.shape.begin(), std::back_inserter(newshape), broadcast_axis);
    return newshape;
    // return std::move(newshape);
    // https://stackoverflow.com/questions/14856344/when-should-stdmove-be-used-on-a-function-return-value
    // --- "When returning a named local variable or a temporary expression directly, you should avoid the explicit std::move. The compiler must (and will in the future) move automatically in those cases, and adding std::move might affect other optimizations."
  }
  
  template <typename Dtype1, typename Dtype2>
  void broadcast(ndarray<Dtype1>& lhs, ndarray<Dtype2>& rhs) {
    if (lhs.shape() == rhs.shape()) // no need to broadcast
      return;
    array_view lhs_view_copy(lhs.view);
    array_view rhs_view_copy(rhs.view);
    auto newshape = get_broadcasted_shape(lhs.view, rhs.view);
    // change shape & stride
    lhs.view.broadcast_to(newshape);
    rhs.view.broadcast_to(newshape);
  }

}

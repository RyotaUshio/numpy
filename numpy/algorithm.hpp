#pragma once

#include <type_traits>

namespace numpy {

  template <class ArrayLike, class CompFunc>
  auto max(const ArrayLike& arr, CompFunc is_smaller_than) -> typename std::remove_reference<decltype(*arr.begin())>::type {
    using T = typename std::remove_reference<decltype(*arr.begin())>::type;
    T maxval = *arr.begin();
    for (const auto e : arr)
      if (is_smaller_than(maxval, e))
	maxval = e;
    return maxval;
  }

  template <class ArrayLike>
  auto max(const ArrayLike& arr) -> decltype(max(arr, 0)) {
    return max(arr, [](auto small, auto large){return small < large;});
  }
  
  template <class ArrayLike>
  auto min(const ArrayLike& arr) -> decltype(max(arr)) {
    return max(arr, [](auto small, auto large){return small > large;});
  }
  
}

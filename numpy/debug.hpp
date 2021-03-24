#pragma once

#include <chrono>
#include <utility>

namespace numpy {
  
  struct debug {
    /* Functionalities for debugging including accessors to private members of some classes. */
    
    template <class Dtype1, class Dtype2>
    static void broadcast(ndarray<Dtype1>& a, ndarray<Dtype2>& b) {
      auto outshape = array_view::get_broadcasted_shape(a.view, b.view);
      array_view::broadcast(array_view(outshape), a.view, b.view);
    }

    template <class Function, class... Args>
    static auto timeit(Function f, Args... args)
      -> decltype(std::declval<std::chrono::microseconds>().count()) {
      auto begin = std::chrono::steady_clock::now();
      f(args...);
      auto end = std::chrono::steady_clock::now();
      return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    }
    
  };
  
}

#pragma once

namespace numpy {
  
  namespace debug {
    /* Functionalities for debugging including accessors to private members of some classes. */
    
    template <class Dtype1, class Dtype2>
    void broadcast(ndarray<Dtype1>& a, ndarray<Dtype2>& b) {
      auto outshape = array_view::get_broadcasted_shape(a.view, b.view);
      array_view::broadcast(array_view(outshape), a.view, b.view);
    }
    
  } // namespace debug
  
}

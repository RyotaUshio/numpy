#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include "slice.hpp"

namespace python {
  
  template <class T> std::string str(const T& obj) {
    return obj.__str__();
  }
  
  template <class T> std::string repr(const T& obj) {
    return obj.__repr__();
  }

  void print() {
    std::cout << std::endl;
  }
  
  template <class Head, class... Tail> void print(const Head& head, const Tail&... tail) {
    std::cout << head << " ";
    print(tail...);
  }
}

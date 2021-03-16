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
  
  template <class... Args> void print(const std::string& head, const Args&... args) {
    std::cout << head << " ";
    print(args...);
  }
}

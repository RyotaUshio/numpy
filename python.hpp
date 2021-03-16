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
  
}

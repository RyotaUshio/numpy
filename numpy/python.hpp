#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility> // forward
#include <numpy/pyobject.hpp>
#include <numpy/slice.hpp>

namespace python {
  
  template <class T> std::string str(const T& obj) {
    return obj.__str__();
  }
  
  template <class T> std::string repr(const T& obj) {
    return obj.__repr__();
  }
  
  std::ostream& operator<<(std::ostream& os, const object& a) {
    os << a.__repr__();
    return os;
  }

  void _print_impl(bool first, std::string sep) {
    std::cout << std::endl;
  }
  
  template <class Head, class... Tail>
  void _print_impl(bool first, std::string sep, Head&& head, Tail&&... tail) {
    if (not first)
      std::cout << sep;
    std::cout << head;
    _print_impl(false, sep, std::forward<Tail>(tail)...);
  }

  template <class... Args>
  void print_sep(std::string sep, Args&&... args) {
    std::cout << std::boolalpha;
    _print_impl(true, sep, std::forward<Args>(args)...);
  }

  template <class... Args>
  void print(Args&&... args) {
    print_sep(" ", std::forward<Args>(args)...);
  }
  
}

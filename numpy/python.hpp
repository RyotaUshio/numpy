#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility> // forward
#include <type_traits>
#include <numpy/slice.hpp>
#include <numpy/typename.hpp>
#include <numpy/dtype.hpp>

namespace python {

  template <typename T>
  auto str(const T& obj) -> decltype(obj.__str__()) {
    return obj.__str__();
  }
  
  template <typename T>
  std::string str(const T& obj) {
    std::stringstream ss;
    if constexpr (std::is_same_v<T, numpy::bool_>) {
      return static_cast<bool>(obj) ? "False" : "True";
    } else {
      ss << obj;
      return ss.str();
    }
  }  

  template <typename T>
  std::string str(const std::vector<T>& vec) {
    std::stringstream ss;
    bool first = true;
    ss << "(";
    for(const auto e : vec) {
      if (first) {
	first = false;
      } else {
	ss << ", ";
      }
      ss << e;
    }
    ss << ")";
    return ss.str();
  }

  template <>
  std::string str<std::type_info>(const std::type_info& type) {
    return getNameByTypeInfo(type);
  }

  template <typename T>
  std::string str() {
    return str(typeid(T));
  }
  
  template <typename T>
  auto repr(const T& obj) -> decltype(std::declval<T>().__repr__()) {
    return obj.__repr__();
  }

  template <typename T>
  auto operator<<(std::ostream& os, const T& a) -> decltype(a.__repr__(), os) {
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

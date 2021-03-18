#pragma once
#include <iostream>
#include <vector>
#include <numpy/typename.hpp>

namespace numpy {

  template <typename T>
  class shared_memory {

  private:
    static int _constructed_count;
    static int _destructed_count;

  public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    
    std::vector<T> data;

    shared_memory(std::vector<T> vec)
      : data(vec) {
      _constructed_count++;
    }

    ~shared_memory() {
      data.clear();
      data.shrink_to_fit();
      _destructed_count++;
      manager();
    }

    // cf) https://cpprefjp.github.io/reference/vector/vector/begin.html
    inline iterator begin() noexcept {
      return data.begin();
    }

    inline const_iterator begin() const noexcept {
      return data.begin();
    }

    inline iterator end() noexcept {
      return data.end();
    }

    inline const_iterator end() const noexcept {
      return data.end();
    }

    // cf) https://cpprefjp.github.io/reference/vector/vector/cbegin.html
    const_iterator cbegin() const noexcept {
      return data.cbegin();
    }

    const_iterator cend() const noexcept {
      return data.cend();
    }

    static void manager() {
      std::cout << "\033[35m[memory manager of numpy::shared_memory<" << python::str<T>() << ">]\n"
		<< "Constructed: " << _constructed_count << " time(s)\n"
		<< "Destructed  : " << _destructed_count << " time(s)\n"
		<< "Remaining undestructed: \033[01m"
		<< _constructed_count - _destructed_count
		<< " object(s)\033[0m" << std::endl;
    }
    
  };

  template <typename T> int shared_memory<T>::_constructed_count = 0;
  template <typename T> int shared_memory<T>::_destructed_count = 0;

}

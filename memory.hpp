#pragma once
#include <iostream>
#include <vector>

namespace numpy {

  template <typename T>
  struct shared_memory {
    std::vector<T> data;
    static int _constructed_count;
    static int _destructed_count;

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

    static void manager() {
      std::cout << "[memory manager of numpy::shared_memory<" << typeid(T).name() << ">]\n"
		<<"Constructed: " << _constructed_count << " time(s)\n"
		<< "Destruced  : " << _destructed_count << " time(s)\n"
		<< "Remaining undestructed: "
		<< _constructed_count - _destructed_count
		<< " object(s)" << std::endl;
    }

    
    
  };

  template <typename T> int shared_memory<T>::_constructed_count = 0;
  template <typename T> int shared_memory<T>::_destructed_count = 0;

}

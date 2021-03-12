#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace numpy {
  typedef unsigned int uint;

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
      std::cout << "Constructed: " << _constructed_count << " times\n"
		<< "Destruced  : " << _destructed_count << " times\n"
		<< "Remaining undestructed: "
		<< _constructed_count - _destructed_count
		<< " object(s)" << std::endl;
    }

    T operator[](std::size_t i) const {
      return data[i];
    }

    T& operator[](std::size_t i) {
      return data[i];
    }
  };

  template <typename T> int shared_memory<T>::_constructed_count = 0;
  template <typename T> int shared_memory<T>::_destructed_count = 0;

  template <typename T> class base_iter;

  template <typename T>
  class ndarray {
    friend base_iter<T>;

    // attributes
    boost::shared_ptr<shared_memory<T> > memory_ptr;
    std::size_t offset;
    
  public:
    std::vector<uint> shape;
    uint size;
    uint ndim;

    // constructors
    ndarray(boost::shared_ptr<shared_memory<T> > ptr, std::vector<uint> shape_, std::size_t offset_)
      : memory_ptr(ptr), shape(shape_), offset(offset_) {
      size = _shape_to_size();
      ndim = shape.size();
    }

    ndarray(shared_memory<T> *ptr, std::vector<uint> shape_, std::size_t offset_)
      : ndarray<T>(boost::shared_ptr<shared_memory<T> >(ptr), shape_, offset_) {}
    
    ndarray(std::vector<T> data, std::vector<uint> shape_)
      : ndarray<T>(new shared_memory(data), shape_, 0) {}
    
    ndarray(T* first, T* last, std::vector<uint> shape_)
      : ndarray<T>(std::vector<T>(first, last), shape_) {}

    ndarray(const ndarray<T>& src)
      : ndarray(src.memory_ptr, src.shape, src.offset) {}
    
    std::vector<T> data() const {
      return memory_ptr->data;
    }

    // operators
    // ndarray<T>& operator+=(ndarray<T> rhs) {
    //   if (not std::equal(shape, rhs.shape))
    // 	throw std::invalid_argument("Cannot add arrays with different shape");
      
    //   // iteratorが必要
    //   return *this;
    // }

    ndarray<T> operator[](const std::size_t irow) { //とりあえず2次元配列前提
      if (ndim <= 0)
	throw std::out_of_range("Cannot subscript a 0-D array");
      
      std::vector<uint> shape_(shape.begin() + 1, shape.end());
      std::size_t offset_ = size / shape[0] * irow;
      ndarray<T>row(memory_ptr, shape_, offset_);
      return row;
    }
    
    // other methods
    std::string str() const {
      std::stringstream ss;
      // iteratorが必要
    }
    
  private:
    uint _shape_to_size() const {
      uint product = 1;
      for(const auto e : shape)
	product *= e;
      return product;
    }
  };
}

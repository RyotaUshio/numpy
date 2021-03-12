#pragma once

namespace numpy {
  typedef unsigned int uint;
  template <typename T> class ndarray;

  template <typename T>
  struct base_iter {
    std::size_t index;
    ndarray<T>& array;
    uint axis;
    std::size_t stride;
    
    base_iter(array<T>& arr, std::size_t idx, uint ax)
      : array(arr), index(idx), axis(ax) {
      switch(axis)
    }

    base_iter(array_iterator<T>& src) // copy constructor
      : base_iter(src.array, src.index) {}

    T& operator*() {
      return arr._data[index];
    }
    
    base_iter<T>& operator++() { // pre-increment
      std::size_t stride = 1; // may vary
      index += stride;
      return *this;
    }
    
    base_iter<T> operator++(int) { // post-increment: takes a dummy parameter of int
      base_iter<T> tmp(*this);
      ++(*this);
      return tmp;
    }
    
    bool operator!=(const array_iterator<T>& rhs) {
      return (index != rhs.index);
    }

    bool operator==(const array_iterator<T>& rhs) {
      return (not (*this) != rhs);
    }
  };
}




// template <typename T>
// class array {
//   friend array_iterator<T>; // allow array_iterator to access 'm_size' and 'm_data'
  
//   std::size_t m_size; // size of array
//   T* m_data;

// public:
//   typedef array_iterator<T> iterator; // enable reference to array_iterator as 'array<T>::iterator'
  
//   array(std::size_t size); // numpy.empty(size)
//   array(std::size_t size, T val); // numpy.full(size, val)
//   ~array();

//   T operator[](std::size_t index) const; // read-only
//   T& operator[](std::size_t index);
//   std::string str() const;
//   array<T>::iterator begin() const;
//   array<T>::iterator end() const;
// };



// template <typename T>
// struct array_iterator {
//   std::size_t m_index;
//   array<T>* m_array;

//   array_iterator(std::size_t index, array<T>* array_);
//   array_iterator(array_iterator<T>& src); // copy constructor
//   T& operator*();
//   array_iterator<T>& operator++(); // pre-increment
//   array_iterator<T> operator++(int); // post-increment: takes a dummy parameter of int
//   bool operator!=(const array_iterator<T>& rhs);
// };



// template <typename T>
// array<T>::array(std::size_t size)
//   : m_size(size) {
//   m_data = new T[m_size];
// }

// template <typename T>
// array<T>::array(std::size_t size, T val)
//   : array(size) {
//   for(auto& e : *this)
//     e = val;
// }

// template <typename T>
// array<T>::~array() {
//   delete[] m_data;
// }

// template <typename T>
// T array<T>::operator[](std::size_t index) const {
//   return m_data[index];
// }

// template <typename T>
// T& array<T>::operator[](std::size_t index) {
//   return m_data[index];
// }

// template <typename T>
// std::string array<T>::str() const {
//   std::stringstream ss;
//   for(const auto e : *this) {
//     ss << e << ' ';
//   }
//   return ss.str();
// }



// template <typename T>
// array_iterator<T>::array_iterator(std::size_t index, array<T>* array_)
//   : m_index(index), m_array(array_) {}

// template <typename T>
// array_iterator<T>::array_iterator(array_iterator<T>& src)
//   : array_iterator<T>(src.m_index, src.m_array) {}

// template <typename T>
// T& array_iterator<T>::operator*() {
//   return (*m_array)[m_index];
// }

// template <typename T>
// array_iterator<T>& array_iterator<T>::operator++() {
//   m_index++;
//   return *this;
// }

// template <typename T>
// array_iterator<T> array_iterator<T>::operator++(int) {
//   array_iterator<T> tmp(*this);
//   ++(*this);
//   return tmp;
// }

// template <typename T>
// bool array_iterator<T>::operator!=(const array_iterator& rhs) {
//   return (m_index != rhs.m_index);
// }

// template <typename T>
// array_iterator<T> array<T>::begin() const {
//   return {0, (array<T>*)this};
// }

// template <typename T>
// array_iterator<T> array<T>::end() const {
//   return {m_size, (array<T>*)this};
// }

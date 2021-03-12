#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include "ndarray.hpp"

template <typename T>
void print_vector(std::vector<T> vec) {
  for(const auto e : vec)
    std::cout << e << ", ";
  std::cout << std::endl;
}

int main() {
  numpy::ndarray<int> a({1, 0, 0, 1}, {2, 2});
  assert(a.size == 4);
  assert(a.ndim == 2);
}

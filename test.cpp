#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <algorithm>
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

  double src[] = {0, 1, 2, 3};
  numpy::ndarray<double> b(src, src+4, {2, 2});
  assert(b.size == 4);
  assert(b.ndim == 2);
  numpy::ndarray<double> c(std::begin(src), std::end(src), {2, 2});
  assert(b.size == 4);
  assert(b.ndim == 2);
}

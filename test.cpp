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

void manager() {
  std::cout << "[numpy::shared_memory<int>::manager()]" << std::endl;
  numpy::shared_memory<int>::manager();
  std::cout << "[numpy::shared_memory<double>::manager()]" << std::endl;
  numpy::shared_memory<double>::manager();
  std::cout << std::endl;
}

int main() {

  // array([[[ 1,  0],
  //         [ 0,  1]],
  //
  //        [[ 2, -1],
  //         [ 5,  4]]])
  numpy::ndarray<int> a({1, 0, 0, 1, 2, -1, 5, 4}, {2, 2, 2});
  assert(a.size == 8);
  assert(a.ndim == 3);

  // array([[ 2, -1],
  // 	    [ 5,  4]])
  auto a1 = a[1];
  assert(a1.size == 4);
  assert(a1.ndim == 2);

  // array([ 2, -1])
  auto a10 = a1[0];
  assert(a10.size == 2);
  assert(a10.ndim == 1);

  // array(-1)
  auto a101 = a10[1];
  assert(a101.size == 1);
  assert(a101.ndim == 0);

  try {
    auto a1010 = a101[0];
  } catch(std::out_of_range e) {}

  // initialization from C's raw array
  double src[] = {0, 1, 2, 3};
  numpy::ndarray<double> b(src, src+4, {2, 2});
  assert(b.size == 4);
  assert(b.ndim == 2);
  numpy::ndarray<double> c(std::begin(src), std::end(src), {2, 2});
  assert(b.size == 4);
  assert(b.ndim == 2);
}

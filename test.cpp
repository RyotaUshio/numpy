#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "ndarray.hpp"

int main() {

  // array([[[ 1,  0],
  //         [ 0,  1]],
  //
  //        [[ 2, -1],
  //         [ 5,  4]]])
  numpy::ndarray<int> a({1, 0, 0, 1, 2, -1, 5, 4}, {2, 2, 2});
  std::cout << a.__repr__() << std::endl;
  std::cout << "a.dtype.name() = " << a.dtype.name() << std::endl;
  assert(a.size == 8);
  assert(a.ndim == 3);

  std::cout << "\n<operator()>" << std::endl;
  int i, j, k;
  for(i=-1; i<int(a.shape[0]); i++)
    for(j=-1; j<int(a.shape[1]); j++)
      for(k=-1; k<int(a.shape[2]); k++) 
	std::cout << "a({" << i << ", " << j << ", " << k << "}) = "
		  << a({i, j, k}).__repr__() << std::endl;
      
  

  std::cout << "\n<operator[]>" << std::endl;
  // array([[ 2, -1],
  // 	    [ 5,  4]])
  auto a1 = a[1];
  std::cout << a1.__repr__() << std::endl;
  assert(a1.size == 4);
  assert(a1.ndim == 2);

  // array([ 2, -1])
  auto a10 = a1[0];
  std::cout << a10.__repr__() << std::endl;
  assert(a10.size == 2);
  assert(a10.ndim == 1);

  // array(-1)
  auto a101 = a10[1];
  std::cout << a101.__repr__() << std::endl;
  assert(a101.size == 1);
  assert(a101.ndim == 0);

  try {
    auto a1010 = a101[0];
  } catch(std::out_of_range e) {}

  // a, a1, a10, a101 are sharing the memory
  

  // initialization from C's raw array
  int src[] = {0, 1, 2, 3};
  numpy::ndarray<int> b(src, src+4, {2, 2});
  assert(b.size == 4);
  assert(b.ndim == 2);
  numpy::ndarray<int> c(std::begin(src), std::end(src), {2, 2});
  assert(b.size == 4);
  assert(b.ndim == 2);

  std::cout << "\n<range-based for loop>" << std::endl;
  for(const auto e : a1)
    std::cout << e << std::endl;

  std::cout << std::endl;
}

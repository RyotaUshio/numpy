#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "python.hpp"
#include "numpy.hpp"

using namespace python;
namespace np = numpy;

int main() {
  std::cout << "<initialization from a std::vector>" << std::endl;
  std::vector<int> src;
  for(int i=0; i<24; i++)
    src.push_back(i);
  np::ndarray<int> a(src, {2, 3, 4});

  std::cout << "\n<indexing 1>" << std::endl;
  auto b = a(":", 0, ":");
  auto c = a(0, ":", 1);
  auto d = c(1);
  auto e = a(":", ":", 1);

  np::utils::test(a);
  np::utils::test(b);
  np::utils::test(c);
  np::utils::test(d);
  np::utils::test(e);

  std::cout << "\n<indexing 2>" << std::endl;
  int i, j, k;
  for(i=0; i<int(a.shape[0]); i++)
    for(j=0; j<int(a.shape[1]); j++)
      for(k=0; k<int(a.shape[2]); k++) 
	std::cout << "a(" << i << ", " << j << ", " << k << ") = "
		  << a(i, j, k).__repr__() << std::endl;
      
  std::cout << "\n<initialization from a C-style array>" << std::endl;
  int src_carray[] = {0, 1, 2, 3};
  np::ndarray<int> carray1(src_carray, src_carray+4, {2, 2});
  assert(carray1.size == 4);
  assert(carray1.ndim == 2);
  np::ndarray<int> carray2(std::begin(src_carray), std::end(src_carray), {2, 2});
  assert(carray2.size == 4);
  assert(carray2.ndim == 2);

  std::cout << "\n<reshaping>" << std::endl;
  std::cout << str(carray1) << std::endl;
  std::cout << str(np::reshape(carray1, {1, 4})) << std::endl;
  std::cout << str(np::reshape(carray1, {4, 1})) << std::endl;
  std::cout << str(np::reshape(carray1, {4})) << std::endl;

  std::cout << "\n<transpose() method>" << std::endl;
  std::cout << repr(a) << std::endl;
  std::cout << repr(np::transpose(a, {0, 1, 2})) << std::endl;
  std::cout << repr(np::transpose(a, {0, 2, 1})) << std::endl;
  std::cout << repr(np::transpose(a, {1, 0, 2})) << std::endl;
  std::cout << repr(np::transpose(a, {1, 2, 0})) << std::endl;
  std::cout << repr(np::transpose(a, {2, 0, 1})) << std::endl;
  std::cout << repr(np::transpose(a, {2, 1, 0})) << std::endl;
  std::cout << repr(np::transpose(a)) << std::endl;

  std::cout << "\n<.T attribute>" << std::endl;
  np::ndarray<int> a_T = a.T;
  std::cout << repr(a_T) << std::endl;

  // std::cout << "\n<range-based for loop>" << std::endl;
  // for(const auto e : a1)
  //   std::cout << e << std::endl;

  // std::cout << std::endl;
}

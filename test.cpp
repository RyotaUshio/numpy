#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "python.hpp"
#include "numpy.hpp"

using namespace python;
namespace np = numpy;

int main() {
  print("<initialization from a std::vector>");
  std::vector<int> src;
  for(int i=0; i<24; i++)
    src.push_back(i);
  np::ndarray<int> a(src, {2, 3, 4});

  print("\n<indexing 1>");
  auto b = a(":", 0, ":");
  auto c = a(0, ":", 1);
  auto d = c(1);
  auto e = a(":", ":", 1);

  np::utils::test(a);
  np::utils::test(b);
  np::utils::test(c);
  np::utils::test(d);
  np::utils::test(e);

  print("\n<indexing 2>");
  int i, j, k;
  for(i=0; i<int(a.shape[0]); i++)
    for(j=0; j<int(a.shape[1]); j++)
      for(k=0; k<int(a.shape[2]); k++) 
	std::cout << "a(" << i << ", " << j << ", " << k << ") = "
		  << a(i, j, k).__repr__() << std::endl;;
      
  print("\n<initialization from a C-style array>");
  int src_carray[] = {0, 1, 2, 3};
  np::ndarray<int> carray1(src_carray, src_carray+4, {2, 2});
  assert(carray1.size == 4);
  assert(carray1.ndim == 2);
  np::ndarray<int> carray2(std::begin(src_carray), std::end(src_carray), {2, 2});
  assert(carray2.size == 4);
  assert(carray2.ndim == 2);

  print("\n<reshaping>");
  print(carray1);
  print(np::reshape(carray1, {1, 4}));
  print(np::reshape(carray1, {4, 1}));
  print(np::reshape(carray1, {4}));

  print("\n<transpose() method>");
  print(a);
  print(np::transpose(a, {0, 1, 2}));
  print(np::transpose(a, {0, 2, 1}));
  print(np::transpose(a, {1, 0, 2}));
  print(np::transpose(a, {1, 2, 0}));
  print(np::transpose(a, {2, 0, 1}));
  print(np::transpose(a, {2, 1, 0}));
  print(np::transpose(a));

  print("\n<.T attribute>");
  np::ndarray<int> a_T = a.T;
  print(a_T);
  
  print("\n<full, zeros, ones>");
  print(np::full({2, 3}, np::pi));
  print(np::zeros({2, 3}));
  print(np::ones<np::int64>({2, 3}));
  
}

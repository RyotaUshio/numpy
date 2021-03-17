#include <numpy/python.hpp>
using namespace python;
#include <numpy/numpy.hpp>
#include <iostream>

namespace np = numpy;

int main() {
  np::ndarray<int> a({0, 1, 2, 3, 4, 5}, {2, 3});
  std::cout << std::boolalpha;
  
  for(np::ndarray<int>::iterator it=a.begin(); it!=a.end(); it++)
    std::cout << *it << std::endl;

  for(const auto e : a)
    std::cout << e << std::endl;

  // 現時点で可能なもっともPythonicな書き方
  for(auto e : a)
    print(e);
}

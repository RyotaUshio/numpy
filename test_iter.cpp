#include <iostream>
#include <cassert>
#include <string>
#include "numpy.hpp"

int main() {
  std::vector<int> src;
  for(int i=0; i<24; i++)
    src.push_back(i);
  numpy::ndarray<int> a(src, {2, 3, 4});
  // numpy::ndarray<int> a({1, 0, 0, 1, 2, -1, 5, 4}, {2, 2, 2});
  
  auto b = a(":", 0, ":");
  auto c = a(0, ":", 1);
  auto d = c(1);
  auto e = a(":", ":", 1);

  numpy::utils::test(a);
  numpy::utils::test(b);
  numpy::utils::test(c);
  numpy::utils::test(d);
  numpy::utils::test(e);
}

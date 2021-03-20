#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  auto a = np::ndarray<np::float64>({1, 2, 3, 4}, {4, 1});
  auto b = np::ndarray<np::float64>({1, 2, 3}, {1, 3});

  // before broadcasting
  print(a);
  print(b);

  for(const auto e : a) std::cout << e << " "; std::cout << std::endl;
  for(const auto e : b) std::cout << e << " "; std::cout << std::endl;

  // np::broadcast(a, b);

  // // after broadcasting
  // print(a);
  // print(b);

  // for(const auto e : a) std::cout << e << " "; std::cout << std::endl;
  // for(const auto e : b) std::cout << e << " "; std::cout << std::endl;

  a += b;
  
  print(a);

  print(type(a));
  
}

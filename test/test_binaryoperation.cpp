#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  auto a = np::ndarray<np::float64>({1, 2, 3, 4}, {4, 1});
  auto b = np::ndarray<np::float64>({1, 2, 3}, {1, 3});

  print(a);
  print(b);
  
  a += b;

  print(a);
  print(b);
}

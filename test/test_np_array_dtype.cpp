#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  // When `dtype` is not specified, it is determined by the 1st argument (`const array_like& object`)'s element. In this case, `int`
  auto a = np::array(range(12)).reshape({3, 4});
  print(a);
  print(type(a));
  print(type(*a.begin()));
  // Explicit specification of dtype is also allowed.
  auto b = np::array<np::single>(range(12)).reshape({3, 4});
  print(b);
  print(type(b));
  print(type(*b.begin()));
}

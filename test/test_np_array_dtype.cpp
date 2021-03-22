#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  auto a = np::array<np::single>(range(12)).reshape({3, 4});
  print(a);
  print(type(a));
  print(type(*a.begin()));
}

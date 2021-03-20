#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {

  auto x1 = np::array(range(9)).reshape({3, 3});
  auto x2 = x1.T();
  print(x1);
  print(x2);
  print(np::_add<int, int>()(1, 2));
  print(np::_exp<float>()(1.0));
  // print(np::add(x1, x2));
}


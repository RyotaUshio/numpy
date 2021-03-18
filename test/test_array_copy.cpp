#include <numpy/python.hpp>
#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  print(">>> auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});");
  auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});

  print("\n>>> auto b = a;");
  auto b = a;

  print("\n>>> print(a);");
  print(a);

  print("\n>>> print(a.T());");
  print(a.T());

  print("\n>>> b = a;");
  b = a;

  print("\n>>> print(b);");
  print(b);

  print("\n>>> print(a);");
  print(a);
}

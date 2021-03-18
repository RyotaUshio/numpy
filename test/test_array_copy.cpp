// >>> auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});

// >>> auto b = a;
// array_view::(copy constructor) is called

// >>> print(a);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

// >>> print(a.T());
// array([[0.000000,	2.000000],
// 	[1.000000,	3.000000]])

// >>> b = a;
// array_view::(copy constructor) is called
// ndarray::(copy assignment operator) is called
// ndarray::swap is called
// array_view::swap is called
// array_view::swap is called

// >>> print(b);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

// >>> print(a);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

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

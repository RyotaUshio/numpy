// $ ./test/test_copy_move 
// >>> auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});

// >>> auto b = a; # this does NOT call the copy assignment operator; just the copy constructor (https://cpprefjp.github.io/lang/cpp11/auto.html)
// ndarray::(copy constructor) is called

// >>> print(a);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

// >>> print(a.T());
// array([[0.000000,	2.000000],
// 	[1.000000,	3.000000]])

// >>> b = a; # here the copy assignment operator is called
// ndarray::(copy assignment operator) is called
// ndarray::(copy constructor) is called
// ndarray::swap is called
// array_view::swap is called
// array_view::swap is called

// >>> print(b);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

// >>> print(a);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

// >>> auto c = std::move(a); # this invokes the move constructor
// ndarray::(move constructor) is called
// ndarray::swap is called
// array_view::swap is called
// array_view::swap is called

// >>> print(c);
// array([[0.000000,	1.000000],
// 	[2.000000,	3.000000]])

// >>> # print(a); -> Segmentation Fault (`a` has already moved)

// >>> c = b.transpose() # the move assignment operator is called
// ndarray::(move assignment operator) is called
// ndarray::swap is called
// array_view::swap is called
// array_view::swap is called

// >>> print(c);
// array([[0.000000,	2.000000],
// 	[1.000000,	3.000000]])
// [memory manager of numpy::shared_memory<double>]
// Constructed: 1 time(s)
// Destruced  : 1 time(s)
// Remaining undestructed: 0 object(s)


#include <utility>
#include <numpy/python.hpp>
#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  print(">>> auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});");
  auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});

  print("\n>>> auto b = a; # this does NOT call the copy assignment operator; just the copy constructor (https://cpprefjp.github.io/lang/cpp11/auto.html)");
  auto b = a;

  print("\n>>> print(a);");
  print(a);

  print("\n>>> print(a.T());");
  print(a.T());

  print("\n>>> b = a; # here the copy assignment operator is called");
  b = a;

  print("\n>>> print(b);");
  print(b);

  print("\n>>> print(a);");
  print(a);

  print("\n>>> auto c = std::move(a); # this invokes the move constructor");
  auto c = std::move(a);

  print("\n>>> print(c);");
  print(c);

  print("\n>>> # print(a); -> Segmentation Fault (`a` has already moved)");

  print("\n>>> c = b.transpose() # the move assignment operator is called");
  c = b.transpose();
  
  print("\n>>> print(c);");
  print(c);
  
}

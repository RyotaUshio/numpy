#include <utility> // std::move
#include <numpy/python.hpp>
#include <numpy/numpy.hpp>

using namespace python;
namespace np = numpy;

int main() {  
  auto a = np::ndarray<np::float64>({0, 1, 2, 3}, {2, 2});
  auto b = a; // this does NOT call the copy assignment operator; just the copy constructor (https://cpprefjp.github.io/lang/cpp11/auto.html)
  print(a);
  print(a.T());
  b = a; // here the copy assignment operator is called
  print(b);
  print(a);
  auto c = std::move(a); // this invokes the move constructor
  print(c);
  // print(a); -> Segmentation Fault (`a` has already been moved)
  c = b.reshape({4, 1}); // the move assignment operator is called
  print(c);
}

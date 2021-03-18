# NumPy in C++

This is an attempt to imitate [NumPy](https://numpy.org/) in C++.

This project provides not only an API for Python/NumPy-like array programming but also the `py++` tool, which enables you to see execution outcomes in a pretty Python-inpreter-like fashion, as shown below.

For example, Suppose you have [this code](https://github.com/RyotaUshio/numpy/blob/main/test/test_copy_move.cpp).
```c++
// test_copy_move.cpp

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
  // print(a); -> Segmentation Fault (`a` has already moved)
  c = b.reshape({4, 1}); // the move assignment operator is called
  print(c);
}
```
All you have to do to see the result is only type the following command.
```
$ ./py++ test_copy_move
```
`py++` will auto-generate & compile some codes, and then show you something like this:

![a](https://github.com/RyotaUshio/numpy/blob/main/fig/py++.png?raw=true)


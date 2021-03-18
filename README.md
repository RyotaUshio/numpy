# NumPy in C++

An attempt to imitate [NumPy](https://numpy.org/) in C++.

This project provides not only an API for Python/NumPy-like array programming but also the `py++` tool, which enables you to see the execution outcome in a pretty Python-inpreter-like fashion, as shown below.

```c++
#include <utility>
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
  c = b.transpose(); // the move assignment operator is called
  print(c);
  
}
```

![a](https://github.com/RyotaUshio/numpy/blob/main/fig/py++.png?raw=true)


# NumPy in C++

This is an attempt to imitate [NumPy](https://numpy.org/) in C++.

This project provides not only an API for Python/NumPy-like array programming but also the `py++` tool, which enables you to see execution outcomes in a pretty Python-interpreter-like fashion, as shown below.

For example, let us suppose you have [this code](https://github.com/RyotaUshio/numpy/blob/main/test/example.cpp).

```c++
// test/example.cpp

#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  
  // Array Creation
  auto x = np::array(range(12)).reshape({3, 4});
  auto y = np::array(range(12)).reshape(3, 4); // equivalent to the above
  print(x); // some of Python's built-in functions, including `print()`, are supported
  print(y);

  // Array's Attribute Accessors
  print(x.shape());
  print(x.T());
  print(np::may_share_memory(x, x.T())); // transposition is just a new "view"
  
  // Indexing & Slicing
  print(x[-1]);
  print(x(-1)); // equivalent to the above
  print(x("1:3","2::")); // slice is also available
  print(x(1,"::-1")); // you can't do `x[1,"::-1"]` due to the C++ language specification
  // indexing is done without copying
  print(np::may_share_memory(x, x[-1])); // -> True
  print(np::may_share_memory(x, x("1:3","2::"))); // -> True
  print(np::may_share_memory(x, x(1,"::-1"))); // -> True

  // Broadcasting & Ufuncs
  auto a = np::array(range(5)).reshape(4, 1);
  auto b = np::array(range(4)).reshape(1, 3);
  print(a);
  print(b);

  // `a` and `b` have different shapes, but arithmetic operations between them are possible.
  print(a + b); // `a` & `b` is broadcasted to the common shape (4, 3) in the internal of `np::add()`
  print(np::add(a, b)); // equivalent to the above

  auto out = np::empty<np::int32>({4, 3}); // `out` parameter is also available, just as the original NumPy!
  np::add(a, b, out);
  print(out);

  // Handling Overlapping Memories
  x = np::array(range(9)).reshape(3, 3);
  print(x);
  print(x.T());
  print(x.T() + x); // just fine!

  // Copy & Move Assignment
  print(np::may_share_memory(a, b)); // -> False
  b = a; // this calls the copy assignment operator, but it does not copy the contents of the array.
  // Just as in Python, `b` now refers to the same location of memory as `a`.
  print(np::may_share_memory(a, b)); // -> True
  a = a + b; // this calls the move assignment operator
  print(a);
  
}

```

All you have to do is just type the following command.
```
$ ./py++ example
```
`py++` will auto-generate & compile some codes, and then show you something like this:

![](https://github.com/RyotaUshio/numpy/blob/main/fig/py++1.png?raw=true)
![](https://github.com/RyotaUshio/numpy/blob/main/fig/py++2.png?raw=true)
![](https://github.com/RyotaUshio/numpy/blob/main/fig/py++3.png?raw=true)
![](https://github.com/RyotaUshio/numpy/blob/main/fig/py++4.png?raw=true)

Of course, you can compile that code with a C++ compiler as usual:

```
g++ -std=c++17 -O3 -Wall -I .  -o test/example test/example.cpp
```

or

```
make test/example
```

Note that C++17 or later is required.
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
  
  // Indexing % Slicing
  print(x[-1]);
  print(x(-1)); // equivalent to the above
  print(x(1,"::-1")); // you can't do `x[2,"::-1"]` due to the C++ language specification
  // indexing is done without copying
  print(np::may_share_memory(x, x[-1])); // -> True
  print(np::may_share_memory(x, x(1, "::-1"))); // -> True

  // Broadcasting & Ufuncs
  auto a = np::array(range(5)).reshape(4, 1);
  auto b = np::array(range(4)).reshape(1, 3);
  print(a);
  print(b);

  // `a` and `b` have different shapes, but arithmetic operations between them are possible.
  print(a + b); // `a` & `b` is broadcasted to the common shape (4, 3) in the internal of `np::add()`
  print(np::add(a, b)); // equivalent to the above

  auto out = np::empty<np::int32>({4, 3}); // and `out` parameter is also available, just as the original NumPy!
  np::add(a, b, out);
  print(out);

  // Copy & Move Assignment
  print(np::may_share_memory(a, b)); // -> False
  b = a; // this calls the copy assignment operator, but it does not copy the contents of the array.
  // Just as in Python, `b` now refers to the same location of memory as `a`.
  print(np::may_share_memory(a, b)); // -> True
  // this calls the move assignment operator
  a = a + b;
  print(a);
  
}

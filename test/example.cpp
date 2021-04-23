#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {

  try {
    // Array Creation
    auto x = np::array(range(12)).reshape({3, 4});
    auto y = np::array(range(12)).reshape(3, 4); // equivalent to the above
    print(x); // some of Python's built-in functions, including `print()`, are supported
    print(y);

    print(np::arange(0.0, 1.0, 0.05));
    print(np::arange(3, 7));
    print(np::arange(5));
    print(np::linspace(0, 1, 11));
    print(np::linspace(0, 1, 10, false));

    auto scolar = np::array(123);
    print(scolar); // scolar as array
    print(scolar.begin());

    auto vec = std::vector<int>{3, 1, 4, 1, 5, 9, 2};
    // auto z = np::asarray<int>(vec); // copy
    // np::gettype(vec);
    // print(np::array(vec));
    // print(z);
    // print(type(z));
    // auto w = np::asarray<np::float_>(z); // no copy is performed
    // print(w);
  
    // Iterate Over Arrays
    for(const auto e : x) print(e);
    for(const auto e : scolar) print(e);

    // Array's Attribute Accessors
    print(x.shape());
    print(x.T());
    print(np::may_share_memory(x, x.T())); // transposition is just a new "view"
  
    // Indexing & Slicing
    print(x[{-1, 3}]);
    print(x(-1)); // equivalent to the above
    print(x("1:3","2::")); // slice is also available
    print(x(1,"::-1")); // you can't do `x[1,"::-1"]` due to the C++ language specification
    print(x.__getitem__({2, 2}));
    print(x.__getitem__({2, -1}));
    // print(x[{3, 1}]); // raises IndexError
    // print(x(3, 1)); // raises IndexError
    // indexing is done without copying
    print(np::may_share_memory(x, x(-1))); // -> True
    print(np::may_share_memory(x, x("1:3","2::"))); // -> True
    print(np::may_share_memory(x, x(1,"::-1"))); // -> True
    
    // Broadcasting & Ufuncs
    auto a = np::array(range(4)).reshape(4, 1);
    auto b = np::array(range(3)).reshape(1, 3);
    print(a);
    print(b);

    // `a` and `b` have different shapes, but arithmetic operations between them are possible.
    print(a + b); // `a` & `b` is broadcasted to the common shape (4, 3) in the internal of `np::add()`
    print(np::add(a, b)); // equivalent to the above

    auto out = np::empty<np::int32>({4, 3}); // `out` parameter is also available, just as the original NumPy!
    np::add(a, b, out);
    print(out);
    
    print(np::exp(out));

    // Handling Overlapping Memories
    x = np::array(range(9)).reshape(3, 3);
    print(x);
    print(x.T());
    print(x + x.T()); // just fine!
    print(x.T() + x); // just fine!

    // Copy & Move Assignment
    print(np::may_share_memory(a, b)); // -> False
    b = a; // this calls the copy assignment operator, but it does not copy the contents of the array.
    // Just as in Python, `b` now refers to the same location of memory as `a`.
    print(np::may_share_memory(a, b)); // -> True
    a = a + b; // this calls the move assignment operator
    print(a);

    // Assignment through indexing
    x(0) *= 100;
    print(x);
    x(-1, -1) = 500;
    print(x);
    x[{-1, -2}] = 999;
    print(x);
    x(":", 1) = np::array(range(1000, 4000, 1000));
    print(x);
    x(-1) = 0;
    print(x);

    // make an array 1-dimensional
    auto x1 = x.ravel();
    auto x2 = x.flatten();
    print(x1);
    print(x2);
    print(x1.memory_info());
    print(x2.memory_info());

    // max & min
    print(np::max(x));
    print(np::min(x));
    print(a);
    print(b);
    print(np::maximum(a, b));
    print(np::heaviside(a, b));

    // linalg
    print(np::linalg::norm(a));
    print(np::linalg::norm(a, 1));

    print(np::add(1, a));
    print(np::add(a, 1));
    print(np::add(1, 1));
    print(np::negative(a));
    print(np::negative(1));
    // print(100.0 * x);
    print(b / 2.0);

  } catch (const std::exception& e) {print(e);}
}

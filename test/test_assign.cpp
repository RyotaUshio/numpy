#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {

  try {

    auto x = np::array(range(6)).reshape(2, 3);
    print(x);
    auto scolar = np::array(range(100, 101))[0];
    print(scolar);
    x(-1, -1) *= scolar; // OK
    print(x);
    x[0] += 10; // OK
    print(x);
    x[0] = x[1]; // OK!
    print(x);
    
  } catch (const std::exception& e) {
    print(e);
  }
  
}

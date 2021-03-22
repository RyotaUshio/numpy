#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  
  try {
    auto a = np::array(range(1, 5)).reshape({4, 1});
    auto b = np::array(range(1, 4)).reshape({1, 3});

    // before broadcasting
    print(a);
    for(const auto e : a) print(e);
    print(b);
    for(const auto e : b) print(e);

    np::debug::broadcast(a, b);

    // after broadcasting
    print(a);
    for(const auto e : a) print(e);
    print(b);
    for(const auto e : b) print(e);
    
  } catch (const std::exception& e) {
    print(e);
  }
}

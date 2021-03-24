#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  try {
    
    auto A = np::arange(9).reshape(3, 3);
    auto x = np::arange(3);

    print(A);
    print(x);
    print(np::matmul(A, x));
    print(A);
    print(x);
    print(np::matmul(x, A));
    print(np::matmul(x, np::array(100)));
    
  } catch(const std::exception& e) {
    print(e);
  }
}

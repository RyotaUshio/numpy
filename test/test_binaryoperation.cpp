#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  auto a = np::array(range(1, 5)).reshape({4, 1});
  auto b = np::array(range(1, 4)).reshape({1, 3});

  print(a);
  print(b);
  
  auto c = a + b;

  print(a);
  print(b);
  print(c);

  // a += b; -> ValueError: non-broadcastable output operand with shape (4, 1) doesn't match the broadcast shape (4, 3)
}

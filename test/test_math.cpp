#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  int N = 10;
  auto a = np::linspace(0, 1, N * N).reshape(N, N);
  auto b = np::linspace(10, 20, N * N).reshape(N, N);
  print(a);
  print(b);
  
  auto time1 = np::debug::timeit([&]{print(np::_matmul_recursive(a, b));}); // very slow
  auto time2 = np::debug::timeit([&]{print(np::matmul(a, b));});
  print(time1, "micro sec.");
  print(time2, "micro sec.");
}

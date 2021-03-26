#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  int N = 100000;
  auto a = np::arange(N);
  auto time = np::debug::timeit([&](){
				  auto it = a.begin();
				  while (it != a.end())
				    ++it;
				});
  print("N =", N, ":", time, "[micro sec]");

  // issue #1
  // N = 100000
  // new : 24054 [micro sec] <- !!
  // old : 42684 [micro sec]
}

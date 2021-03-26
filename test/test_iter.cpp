#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

template <class Dtype>
void timeit(const np::ndarray<Dtype>& a) {
  auto time = np::debug::timeit([&a](){
				  auto it = a.begin();
				  auto end = a.end();
				  while (it != end)
				    ++it;
				});
  print("shape =", a.shape(), ":", time / 1000, "[ms]");
}

int main() {
  auto a = np::arange(1000 * 1000);
  // 1-D array
  timeit(a);
  // 2-D array
  timeit(a.reshape(100, 10000));
  // 3-D array
  timeit(a.reshape(10, 100, 1000));
}

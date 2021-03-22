#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;


int main() {
  auto a = np::array(range(5)).reshape({4, 1});
  auto b = np::array(range(4)).reshape({1, 3});
  auto c = np::array(range(12)).reshape({1, 4, 3});
  print(np::array_view::is_same_shape(a.get_view(), b.get_view(), c.get_view()));
  print(np::array_view::get_broadcasted_shape(a.get_view(), b.get_view(), c.get_view()));
}

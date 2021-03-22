#include <numpy/numpy.hpp>

using namespace python;
namespace np = numpy;

int main() {

  // functors that will be applied element-wise
  print(np::_ufunc_internal::_add<int, np::float_>()(1, 2));
  print(np::_ufunc_internal::_exp<float>()(1.0));

  // define operand arrays `x1`, `x2`
  auto x1 = np::array(range(9)).reshape({3, 3});
  auto x2 = x1.T();
  print(x1);
  print(x2);

  // compute `x1 + x2` in two ways
  auto x3 = np::add(x1, x2); // (1)
  print(x3);
  auto x4 = np::empty<np::int32>({3, 3}); // (2)
  np::add(x1, x2, x4);
  print(x4);

  // unary ufuncs
  print(np::exp(x1));
  print(np::log1p(x1));

}


#include <numpy/python.hpp>
#include <numpy/numpy.hpp>

using namespace python;
namespace np = numpy;

int main() {
  auto Ab = np::ndarray<np::float_>({3, 3, -5, -6,     1, 0, 0, 0,     1, 2, -3, -1,     0, 1, 0, 0,     2, 3, -5, -3,     0, 0, 1, 0,     -1, 0, 0, 1,     0, 0, 0, 1}, {4, 8});
  auto A = Ab(":", ":4");
  auto b = Ab(":", "4:");
  // print(A);
  // print(b);
  // auto p = np::linalg::_forward_elimination(Ab);
  // print(A);
  // print(b);
  // np::linalg::_backward_substitution_upper(A, b);
  // print(A);
  // print(b);

  A = np::ndarray<np::float_>({1, 1, 2, 1,     1, 1, 3, 2,     2, -2, 2, -1,     -1, 1, 0, 1}, {4, 4});
  b = np::ndarray<np::float_>({-1, 2, -1, 1}, {4});
  auto LU = np::linalg::LU_decomposition(A);
  print(b);
  print(LU.LU);
  auto L = LU.L();
  auto U = LU.U();
  print(L);
  print(U);
  print(np::matmul(L, U));
  print(LU.permute(A));
  print(LU.solve(b));
}

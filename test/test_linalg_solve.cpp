#include <numpy/python.hpp>
#include <numpy/numpy.hpp>
#include <scipy/linalg/linalg.hpp>

using namespace python;
namespace np = numpy;

int main() {
  // Solve linear system
  auto A = np::ndarray<np::float_>({1, 1, 2, 1,     1, 1, 3, 2,     2, -2, 2, -1,     -1, 1, 0, 1}, {4, 4});
  auto b = np::ndarray<np::float_>({-1, 2, -1, 1}, {4});

  // With LU decomposition
  print(A);
  print(b);
  
  auto LU = scipy::linalg::LU_decomposition(A);
  print(b);
  print(LU.LU);
  auto L = LU.L();
  auto U = LU.U();
  print(L);
  print(U);
  print(np::matmul(L, U));
  print(LU.permute(A));
  print(LU.solve(b));

  // With Gaussian elimination
  print(A);
  print(b);
  
  auto gauss = scipy::linalg::Gaussian_elimination(A, b);
  print(gauss.solve());

  // With the iterative methods

  // spectral radius >= 1 so the iteration will not converge
  // auto x = scipy::linalg::Gauss_Seidel(A, b);
  // print(x);

  // An example taken from
  // http://www.math.ritsumei.ac.jp/yasutomi/jugyo/Numerical_Analysis/note5.pdf
  A = np::ndarray<np::float_>({3, 1, 1, 1, 3, 1, 1, 1, 3}, {3, 3});
  b = np::ndarray<np::float_>({0, 4, 6}, {3});
  print(A);
  print(b);

  // Jacobi method
  auto jcb = scipy::linalg::Jacobi(A, b);
  auto x = jcb.solve();
  print(x);
  print(np::matmul(A, x));

  // Gauss-Seidel method
  auto gs = scipy::linalg::Gauss_Seidel(A, b);
  x = gs.solve();
  print(x);
  print(np::matmul(A, x));

  // SOR method
  auto sor = scipy::linalg::SOR(A, b, 1.15);
  x = sor.solve();
  print(x);
  print(np::matmul(A, x));

  // Conjugate Gradient method
  x = scipy::linalg::cg(A, b);
  print(x);
  print(np::matmul(A, x));


  // Let's try direct methods as well
  x = scipy::linalg::lu_solve(scipy::linalg::lu_factor(A), b);
  print(x);

  // Find the inverse matrix
  A = np::ndarray<np::float_>({3, 3, -5, -6,   1, 2, -3, -1,   2, 3, -5, -3,   -1, 0, 0, 1}, {4, 4});
  print(A);
  auto A_inv = scipy::linalg::inv(A);
  print(A_inv);
  print(np::matmul(A, A_inv));

}

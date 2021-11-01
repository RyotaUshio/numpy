#include <numpy/python.hpp>
#include <numpy/numpy.hpp>
#include <scipy/linalg/linalg.hpp>

using namespace python;
namespace np = numpy;

int main() {

  auto a1 = np::ndarray<np::float_>({1, 0, 0, 0, 2, 4, 0, 3, 1}, {3, 3});
  auto a2 = np::ndarray<np::float_>({2, 0, 0, 0, 2, -1, 0, 1, 4}, {3, 3});

  auto eig1 = scipy::linalg::_eigen::PowerMethod(a1);
  eig1.set_eig();
  // auto eig2 = scipy::linalg::_eigen::PowerMethod(a2);

  print(eig1.eigval, eig1.eigvec);
  // print(eig2.eigval, eig2.eigvec);

  auto eig1_inv = scipy::linalg::_eigen::InverseIteration(a1, -2.1);
  eig1_inv.set_eig();
  print(eig1_inv.eigval, eig1_inv.eigvec);

  auto eig1_inv_ = scipy::linalg::_eigen::InverseIteration(a1, 4.9);
  eig1_inv_.set_eig();
  print(eig1_inv_.eigval, eig1_inv_.eigvec);

  auto eig1_inv__ = scipy::linalg::_eigen::InverseIteration(a1, 1.1);
  eig1_inv__.set_eig();
  print(eig1_inv__.eigval, eig1_inv__.eigvec);

  auto eig2_inv = scipy::linalg::_eigen::InverseIteration(a2);
  eig2_inv.set_eig();
  print(eig2_inv.eigval, eig2_inv.eigvec);

  auto [Q, R] = scipy::linalg::_eigen::GramSchmidt(a1);
  print(a1);
  print(Q);
  print(R);
  print(np::matmul(Q, R));

  // 重複固有値がある場合のべき乗法
  auto A = np::ndarray<np::float_>({0, 0, 0, 1, -2, 0, -2, 0, 3, 1, 3, -1, -2, 0, 0, 3}, {4, 4});
  auto eigA = scipy::linalg::_eigen::PowerMethod(A);
  eigA.set_eig();
  print(eigA.eigval, eigA.eigvec);

  // QR
  print(a1);
  auto qr = scipy::linalg::_eigen::QR(a1, scipy::linalg::eps_default, 1.0e-5);
  qr.set_eigvals_();
  //qr.set_eig();
  print(qr.eigvals);
  print(qr.eigvecs);
}

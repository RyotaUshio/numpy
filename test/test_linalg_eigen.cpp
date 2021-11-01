#include <numpy/python.hpp>
#include <numpy/numpy.hpp>
#include <scipy/linalg/linalg.hpp>

using namespace python;
namespace np = numpy;

int main() {

  auto a1 = np::ndarray<np::float_>({1, 0, 0, 0, 2, 4, 0, 3, 1}, {3, 3});
  auto a2 = np::ndarray<np::float_>({2, 0, 0, 0, 2, -1, 0, 1, 4}, {3, 3});

  auto eig1 = scipy::linalg::_eigen::PowerMethod(a1);
  eig1.eig();
  // auto eig2 = scipy::linalg::_eigen::PowerMethod(a2);

  print(eig1.eigval, eig1.eigvec);
  // print(eig2.eigval, eig2.eigvec);

  auto eig1_inv = scipy::linalg::_eigen::InverseIteration(a1, -2.1);
  eig1_inv.eig();
  print(eig1_inv.eigval, eig1_inv.eigvec);

  auto eig1_inv_ = scipy::linalg::_eigen::InverseIteration(a1, 4.9);
  eig1_inv_.eig();
  print(eig1_inv_.eigval, eig1_inv_.eigvec);

  auto eig1_inv__ = scipy::linalg::_eigen::InverseIteration(a1, 1.1);
  eig1_inv__.eig();
  print(eig1_inv__.eigval, eig1_inv__.eigvec);

  auto eig2_inv = scipy::linalg::_eigen::InverseIteration(a2);
  eig2_inv.eig();
  print(eig2_inv.eigval, eig2_inv.eigvec);
}

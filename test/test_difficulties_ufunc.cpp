#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {
  
  // self-assinment & compound assinment
  // メモリを共有するndarrayどうしの演算結果を、ふたたびその同じメモリに書き込むのでおかしなことになる
  auto x1 = np::array(range(9)).reshape({3, 3});
  auto x2 = x1.T();

  print(x1);
  print(x2);
  x1 += x2;
  print(x1);
  print(x2);

  // problems regarding broadcasting
  // これはよくわからないが、broadcastを伴わない例では起こらない問題なのでbroadcastと関係がありそう
  auto a = np::array(range(1, 5)).reshape({4, 1});
  auto b = np::array(range(1, 4)).reshape({1, 3});

  print(a);
  print(b);
  
  auto c = a + b;
  
  print(a);
  print(b);
  print(c);
  
}

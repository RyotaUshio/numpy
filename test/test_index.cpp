#include <numpy/numpy.hpp>
using namespace python;
namespace np = numpy;

int main() {

  try{
    auto a = np::array(range(24)).reshape({2, 3, 4});
    print(a);
    print(a.get_view());

    auto b = a(-1);
    print(b);
    print(b.get_view());

    auto c = a(-1, -2);
    print(c);
    print(c.get_view());

    auto d = a(-1, -2, "::-1");
    print(d);
    print(d.get_view());

    auto e = a(-1, -2, ":-2:-1");
    print(e);
    print(e.get_view());

    a(1, ":"); // this is OK

    auto C = a(-1, "-2"); // this is invalid
    print(C);
    print(C.get_view());

    auto D = a(-1, "-2", "::-1");
    print(D);
    print(D.get_view());

    auto E = a(-1, "-2", ":-2:-1");
    print(E);
    print(E.get_view());
    
  } catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

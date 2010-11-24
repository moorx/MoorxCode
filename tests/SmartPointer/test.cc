#include <MXCore/text_output.h>
#include <MXCore/smart_pointer.h>

using namespace mxcore;

class Foo {
 public:
  Foo() {
    Print("constructing\n");
  }

  ~Foo() {
    Print("destructing\n");
  }
};

void function2(SmartPointer<Foo>& s) {
  SmartPointer<Foo> c = s;
  SmartPointer<Foo> d(new Foo());
}

void function1() {
  SmartPointer<Foo> a(new Foo());
  SmartPointer<Foo> b(a);
  function2(b);
}

int main()
{
  function1();
  return 0;
}

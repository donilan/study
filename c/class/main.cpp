#include <iostream>

class A {
public:
  const int MAX;
  int getMax();
};

int A::getMax() { return 0; };

int main(void) {
  A *a = new A();
  std::cout << "class A getMax is: " << a->getMax() << std::endl;
  return 0;
}

#include <iostream>

void change(int &v) {
  v = 2;
}


int main(void) {
  int v(1);
  std::cout << "v value is: " << v << std::endl;
  change(v);
  std::cout << "after change: " << v << std::endl;

  std::cout << "-----------" << std::endl;

  const int &rv = v;

  std::cout << "rv address is: " << &rv << std::endl;
  std::cout << "v address is:  " << &v << std::endl;
  
  return 0;
}

#include <iostream>

int& test_return(int i) {
  std::cout << "v" << i << " in function address is: " << &i << std::endl;
  return i;
}

int main(int argc, char *argv[] ) {
  int v1 = test_return(1);
  int v2 = test_return(2);
  int v3 = test_return(3);

  std::cout << "v1 outside function address is: " << &v1 << "v1 = " << v1 << std::endl;
  std::cout << "v2 outside function address is: " << &v2 << "v2 = " << v2 << std::endl;
  std::cout << "v3 outside function address is: " << &v3 << "v3 = " << v3 << std::endl;
  return 0;
}

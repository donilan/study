#define NR_test 50

#define call(name) NR_##name

#include <iostream>

int main(void) {
  std::cout << call(test) << std::endl;
  return 0;
}

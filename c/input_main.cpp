#include <iostream>

int main() {
  int sum = 0, value = 0;

  while(std::cout << "Please enter a number: " << std::endl && std::cin >> value) {
    sum += value;
  }

  std::cout << "Sum is: " << sum << std::endl;
  return 0;
}
n is: 229

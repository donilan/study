#include <iostream>
#include <string>

int main() {
  std::cout << "Hello world\n\0";
  std::cout << "sizeof L'a': " << sizeof(L'a') << std::endl;
  std::cout << "sizeof 'a' : " << sizeof('a') << std::endl;

  std::cout << "Hello"
    " world"
    "\n";

  std::cout << "1 not_eq 0 : " << (1 not_eq 0) << std::endl;
  std::cout << "1 or 0 : " << (1 or 0) << std::endl;

  std::string chinese = "中文";
  std::cout << chinese << " and size is: " << chinese.size() << std::endl;
  for(int i = 0; i < chinese.size(); ++i) {
    std::cout << (unsigned short)chinese[i] << ", ";
  }
  std::cout << std::endl;

  

  return 0;
}

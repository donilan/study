#include <iostream>
#include <string>

int main(void) {
  int v1, v2 = 0;
  std::string split(40, '=');

  std::cout << "v1 address: " << &v1 << std::endl
	    << "v2 address: " << &v2
	    << std::endl;

  std::cout << split << std::endl;

  double doubles[5] = {0};
  double *pDouble = &doubles[0];
  for(int i = 0; i < 5; ++i) {
    std::cout << "double address: " << pDouble++ << std::endl;
  }

  int ints[5] = {0};
  int *pInt = &ints[0];
  for(int i = 0; i < 5; ++i) {
    std::cout << "int address: " << pInt++ << std::endl;
  }



  std::cout << split << std::endl;

  char ca1[] = {'a', 'b', 'c', 'd', 'e'};
  char ca2[] = "Hello world.\n";
  std::cout << ca1 << std::endl;

  std::cout << split << std::endl;

  //  char *pchar = 0;

  //  std::cout << pchar << std::endl;

  std::cout << "size of string: " << sizeof(std::string) << std::endl;

  return 0;
}

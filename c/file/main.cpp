#include <iostream>
#include <fstream>

int main(void) {
  std::ofstream ofile;
  ofile.open("out");
  if(ofile) {
    ofile << "test" << std::endl;
    ofile.close();
    
  }

  return 0;
}

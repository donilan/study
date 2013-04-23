#include <dirent.h>

void err_sys(const char *, ...);

int main(int argc, char *argv[]) {
  DIR *dp;
  struct dirent *dirp;

  if(argc != 2) {
    err_sys("Usage: ls directory_name");
  }
}

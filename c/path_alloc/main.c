#include <limits.h>
#include <dirent.h>
#include <stdio.h>

int main(void) {
  int len;
  path_alloc(&len);
  printf("path alloc length: %d\n", len);
  return 0;
}

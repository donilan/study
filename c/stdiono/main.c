#include <unistd.h>

int main(void) {
  printf("STDIN_FILENO : %d, STDOUT_FILENO : %d, STDERR_FILENO : %d, \
OPEN_MAX : %d", STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO, OPEN_MAX);
  return 0;
}

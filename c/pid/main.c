#include <stdio.h>
#include <unistd.h>

int main(void) {
  pid_t pid;

  if((pid = fork()) < 0) {
    printf("error\n");
    return -1;
  } else if(pid == 0) {
    printf("child pid is: %d\n", getpid());
  } else {
    printf("farther pid is: %d\n", getpid());
  }
  return 0;
}

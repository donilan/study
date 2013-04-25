#include <signal.h>
#include <stdio.h>

void sig_int(int signo) {
  printf("interrupt \n");
}

int main(void) {
  char buf[4098];

  if(signal(SIGINT, sig_int) == SIG_ERR) {
    printf("signal error\n");
  }
  
  while(fgets(buf, 4098, stdin) != NULL) {
    printf("%% ");
    printf("%s\n", buf);
  }
}

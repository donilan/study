#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

char *BUFF = "Hello world!";


int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Usage: %s number\n", argv[0]);
  }

  int fileno = atoi(argv[1]);

  int err = write(fileno, BUFF, strlen(BUFF));
  
  if(err == -1)
    printf("error no: %d", errno);

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 4098

int main(void) {
  int n;
  char buff[BUFF_SIZE];

  while(( n = read(STDIN_FILENO, buff, BUFF_SIZE)) > 0){

    if( write(STDOUT_FILENO, buff, n) != n) {
      printf("error\n");
    }
  }

  exit(0);
}

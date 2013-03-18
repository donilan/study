#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int read_port(void)
{
  int fd = open("/dev/ttyS11", O_RDONLY | O_NOCTTY);
  if (fd == -1)
    {
      /* Could not open the port. */
      perror("open_port: Unable to open /dev/ttyS0 - ");
    }
  else
    {
      printf("read success!\n");
    }
  

  char buffer[32];
  int n = read(fd, buffer, sizeof(buffer));
  if (n < 0)
    fputs("read failed!\n", stderr);
  return (fd);
}

int main(void)
{
  return read_port();
}

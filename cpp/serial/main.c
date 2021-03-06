#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(void)
{
  int fd, c, res;
  struct termios oldtio, newtio;
  char buf[255];
  
  fd = open(MODEMDEVICE, O_RDWR| O_NOCTTY);
  if (fd < 0) {perror(MODEMDEVICE); return -1;}

  tcgetattr(fd, &oldtio);
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

  newtio.c_iflag = IGNPAR | ICRNL;
  newtio.c_oflag = 0;

  newtio.c_lflag = ICANON;

  newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
  newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
  newtio.c_cc[VERASE]   = 0;     /* del */
  newtio.c_cc[VKILL]    = 0;     /* @ */
  newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
  newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
  newtio.c_cc[VSWTC]    = 0;     /* '\0' */
  newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
  newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
  newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
  newtio.c_cc[VEOL]     = 0;     /* '\0' */
  newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
  newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
  newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
  newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
  newtio.c_cc[VEOL2]    = 0;     /* '\0' */

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  while (STOP == FALSE) {
    res = read(fd, buf, 255);
    buf[res] = 0;
    printf(":%s:%d\n", buf, res);
    if(buf[0] == 'z') STOP = TRUE;
  }
  tcsetattr(fd, TCSANOW, &oldtio);
  return 0;
}

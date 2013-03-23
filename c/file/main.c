#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
  int fd;
  if((fd = creat("/tmp/file.hole", FILE_MODE)) < 0)
    printf("create error\n");

  printf("file id is: %d\b", fd);

  if(write(fd, buf1, 10) != 10) 
    printf("buf1 write error\n");
  
  if(lseek(fd, 16384, SEEK_SET) == -1)
    printf("lseek error\n");

  if(write(fd, buf2, 10) != 10)
    printf("buf2 write error\n");

  return 0;
}

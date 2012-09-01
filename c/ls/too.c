#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap) {
  char buf[4096];
  vsnprintf(buf, 4096, fmt, ap);
  if(errnoflag)
    snprintf(buf+strlen(buf), 4096-strlen(buf), ": %s", strerror(error));
  strcat(buf, "\n");
  fflush(stdout);
  fputs(buf, stderr);
  fflush(NULL);
}

void err_sys(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  err_doit(1, errno, fmt, ap);
  va_end(ap);
  exit(1);
}

void err_quit(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  err_doit(0, 0, fmt, ap);
  va_end(ap);
  exit(1);
}

int main(int argc, char *argv[]) {
  DIR *dp;
  struct dirent *dirp;
  
  if(argc != 2)
    err_quit("usage: ls directory_name");
  
  if((dp = opendir(argv[1])) == NULL)
    err_sys("can't open %s", argv[1]);

  while((dirp = readdir(dp)) != NULL)
    printf("%s\n", dirp->d_name);

  closedir(dp);
  exit(0);
}

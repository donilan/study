#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  setlocale(LC_ALL, "zh_CN.UTF-8");
  bindtextdomain("hello", "locale");
  textdomain("hello");
  printf(gettext("Hello, world!\n"));
  exit(0);
}

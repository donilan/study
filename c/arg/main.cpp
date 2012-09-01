#include <stdarg.h>
#include <stdio.h>

void arg_test(int count, ...) {
  va_list ap;
  int i;
  va_start(ap, count);
  for( i = va_arg(ap, int) ; i != -1; i = va_arg(ap, int)){
    printf("arg: %d\n", i);
  }
}

int main(void) {
  arg_test(1, 2, 3, 4);

}

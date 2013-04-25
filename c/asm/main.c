#include <stdio.h>



#define show_fs(val) __asm__("mov %%sp, %0"	\
	  :"=r"(val)\
	  :\
	  :"%ax"\
	  );\
  printf("fs: %x\n", val)


int main(void) {
  int a = 10, b;
  register unsigned short c;
  
  show_fs(c);

  __asm__("movl %1, %%eax\n\t"
	  "movl %%eax, %0\n\t"
	  :"=r"(b)
	  :"r"(a)
	  :"%eax"
	  );
  show_fs(c);

  printf("Result: %d, %d\n", a, b);

  show_fs(c);

  int tr;
  __asm__("str %0"
	  : "=r"(tr)
	  : "0"(0)
	  : "%ax"
	  );
  printf("str is: %x\n", tr);
  return 0;
}

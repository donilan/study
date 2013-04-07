#include <stdio.h>
#include "dll.h"

extern "C" {
	EXPORT void hello(void)
	{
	  printf("Hello world!\n");
	}
}
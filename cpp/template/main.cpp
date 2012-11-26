#include <stdio.h> 

template <class T> T add(T v1, T v2)
{
  return v1 + v2;
}

int main(void) 
{ 
  int v = add(1, 2);
  printf("v is: %d\n", v);
}


#include <stdio.h>

typedef struct st1{
  int a;
  int b;
} St1;


int hello(St1 *st)
{
  st->a = 1;
  st->b = 2;
  return 1;
}

St1* make()
{
  return (St1 *) malloc(sizeof(St1));
}

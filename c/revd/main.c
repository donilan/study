#include <stdio.h>

void revd_and_print(int val)
{
  if(val == 0){
    return;
  }
  printf("%d", val%10);

  revd_and_print(val/10);


}

 
int main(void) {

  revd_and_print(12345678);
}



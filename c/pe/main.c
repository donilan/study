#include <stdio.h>
#include <windows.h>
#include <WinNT.h>

int main()
{
  char *filename = "NOTEPAD.EXE";
  FILE *f = NULL;
  f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  size_t f_size = ftell(f);
  printf("File size is: %dkb\n", f_size/1024);
  fseek(f, 0, SEEK_SET);

  char *buff = (char *)malloc(f_size);
  size_t r_size = fread(buff, sizeof(char), f_size, f);
  if(r_size != f_size)
    {
      printf("Can't read the whole file. Just read %db data.\n", r_size);
    }
  fclose(f);

  printf("dos stub: %x\n", ((unsigned short *)buff)[0]);
  unsigned int pe_offset = *((unsigned int *)(buff + 0x3c));
  unsigned int pe_signature = *((unsigned int*)(buff+pe_offset));
  printf("%x, %x, %x\n", buff+0x3c, pe_offset, pe_signature);
  printf("this file %s\n", __FILE__);
  
  free(buff);
  return 0;
}

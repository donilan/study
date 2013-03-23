#include <Hello.h>
#include <stdio.h>

JNIEXPORT void JNICALL Java_Hello_hello(JNIEnv *env, jobject cls)
{
  printf("Hello world!\n");
}

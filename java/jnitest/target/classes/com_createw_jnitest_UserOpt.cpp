#include "com_createw_jnitest_UserOpt.h"
#include <stdio.h>

/*
 * Class:     com_createw_jnitest_UserOpt
 * Method:    getUser
 * Signature: (Ljava/lang/String;)Lcom/createw/jnitest/User;
 */
JNIEXPORT jobject JNICALL Java_com_createw_jnitest_UserOpt_getUser
  (JNIEnv *env, jobject obj, jstring str)
{
  printf("In getUser method.\n");
  jclass userClass = env->FindClass("com/createw/jnitest/User");
  jmethodID userMethod = env->GetMethodID(userClass, "<init>", "()V");
  jobject u = env->NewObject(userClass, userMethod);
  jmethodID userSetUsername = env->GetMethodID(userClass, "setUsername", "(Ljava/lang/String;)V");

  env->CallObjectMethod(u, userSetUsername, env->NewStringUTF("DoniLan"));
  return u;
}


/*
 * Class:     com_createw_jnitest_UserOpt
 * Method:    saveUser
 * Signature: (Lcom/createw/jnitest/User;)V
 */
JNIEXPORT void JNICALL Java_com_createw_jnitest_UserOpt_saveUser
  (JNIEnv * env, jobject obj, jobject user)
{
  jclass userClass = env->GetObjectClass(user);
  jmethodID userGetUsername = env->GetMethodID(userClass, "getUsername", "()Ljava/lang/String;");
  jstring username = (jstring)env->CallObjectMethod(user, userGetUsername);
  jboolean b = true;
  const char *pUsername = env->GetStringUTFChars(username, &b);
  printf("In saveUser method. username: %s\n", pUsername);
}

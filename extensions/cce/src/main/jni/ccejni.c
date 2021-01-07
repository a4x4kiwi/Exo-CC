/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "cce.h"
#include "android_log.h"

#ifdef CPU_FEATURES_ARCH_ARM
#include "cpuinfo_arm.h"  // NOLINT
#endif                    // CPU_FEATURES_ARCH_ARM
#ifdef CPU_FEATURES_COMPILED_ANY_ARM_NEON
#include <arm_neon.h>
#endif  // CPU_FEATURES_COMPILED_ANY_ARM_NEON
#include <jni.h>

#include <stdint.h>
#include <string.h>
#include <pthread.h>


#define CCE_FUNC(RETURN_TYPE, NAME, ...)                         \
 /* extern "C" {                                                       \
  JNIEXPORT RETURN_TYPE                                              \
      Java_com_google_android_exoplayer2_ext_cce_CceLibrary_##NAME( \
          JNIEnv* env, jobject thiz, ##__VA_ARGS__);                 \
  } */                                                                 \
  JNIEXPORT RETURN_TYPE                                              \
      Java_com_google_android_exoplayer2_ext_cce_CceLibrary_##NAME( \
          JNIEnv* env, jobject thiz, ##__VA_ARGS__)


JavaVM* gl_JVM = NULL;
jclass  gl_ActivityClass;
jobject gl_ActivityObj;

pthread_t thread_id;

static jmethodID mid_callback;

JNIEnv* getEnv() {
  JNIEnv *env;
  if (gl_JVM == NULL)
    return NULL;
  int status = (*gl_JVM)->GetEnv(gl_JVM, (void**)&env, JNI_VERSION_1_6);
  if(status < 0) {
    status = (*gl_JVM)->AttachCurrentThread(gl_JVM, &env, NULL);
    if(status < 0) {
      return NULL;
    }
  }
  return env;
}

CCE_FUNC(void, jniInitCcextractor/*, jstring logFilePath*/) {

  deinit_ccextractor();

  (*env)->GetJavaVM(env, &gl_JVM);
  jclass cls = (*env)->GetObjectClass(env, thiz);
  gl_ActivityClass = (*env)->NewGlobalRef(env, cls);
  gl_ActivityObj = (*env)->NewGlobalRef(env, thiz);

  if (cls != NULL) {
    mid_callback = (*env)->GetMethodID(env, cls, "appendSubtitle",  "([B)V");
  }

  if (thread_id != 0) {
    int status;
    if ( (status = pthread_kill(thread_id, SIGUSR2)) != 0)
    {
      LOGI("Error cancelling thread %d, error = %d (%s)", thread_id, status, strerror(status));
      if (log_fp) fprintf(log_fp, "Error cancelling thread %d, error = %d (%s)", thread_id, status, strerror(status));
    }
    thread_id = 0;
  }

 // const char *logFileString = (*env)->GetStringUTFChars(env, logFilePath, 0);

  int argc = 5;
  char *argv[5];
  argv[0] = (char*)"ccextractor";
  argv[1] = (char*)"-out=ass";
  argv[2] = (char*)"-bom";
  argv[3] = (char*)"-latin1";
  argv[4] = (char*)"vfile.ts";

  init_ccextractor(argc, argv/*, null*/);

  LOGI("Initialize Extractor.");
  if (log_fp) fprintf(log_fp, "Initialize Extractor.\n");

 // (*env)->ReleaseStringUTFChars(env, logFilePath, logFileString);
  (*env)->DeleteLocalRef(env, cls);
}


CCE_FUNC(void, jniRunCcextractor, jlong inputlength) {
  pthread_create(&thread_id, NULL, run_ccextractor, inputlength);
}


CCE_FUNC(void, jniWriteSharedBytes, jbyteArray data, jint length) {
  jbyte* jData = (*env)->GetByteArrayElements(env, data, 0);

  unsigned  char* buffer;

  buffer = (unsigned  char*) malloc(sizeof(unsigned char) * length);

  memcpy(buffer, jData, length);

  write_shared_bytes(buffer, length);

  free(buffer);

  (*env)->ReleaseByteArrayElements(env, data, jData, JNI_ABORT);
}


CCE_FUNC(jbyteArray, jniGetSsaHeader) {
    unsigned char* data;
    int size;

    size = get_header(&data);

    jbyteArray ret = (*env)->NewByteArray(env, size);

    if (ret == NULL)
        return NULL;

    jbyte *bytes = (*env)->GetByteArrayElements(env, ret, 0);
    memcpy(bytes, data, size);

    (*env)->SetByteArrayRegion(env, ret, 0, size, bytes);

    (*env)->ReleaseByteArrayElements(env, ret, bytes, JNI_ABORT);

    return ret;
}

void append_subtitle() {
  JNIEnv *env;

  env = getEnv();
  if ((*env) == NULL)
    return;
  (*env)->ExceptionClear(env);
  int size = shared_subtitles_size;
  jbyteArray ba = (*env)->NewByteArray(env, size);
  if (ba == NULL)
    return;
  jbyte *bytes = (*env)->GetByteArrayElements(env, ba, 0);
  memcpy(bytes, shared_subtitles, size);
  (*env)->SetByteArrayRegion(env, ba, 0, size, bytes);
  if (mid_callback != NULL) {
    (*env)->CallVoidMethod(env, gl_ActivityObj, mid_callback, ba);
  }

  (*env)->ReleaseByteArrayElements(env, ba, bytes, JNI_ABORT);
  (*env)->DeleteLocalRef(env, ba);
}

CCE_FUNC(void, jniFinishedHeader) {
  shared_subtitles_lines = 0;
}

CCE_FUNC(void, jniEndCcextractor) {
    LOGI("Exit the extractor.");
    if (log_fp) fprintf(log_fp, "Exit the extractor.\n");

    if (thread_id == 0)
        return;

    int status;
    if ((status = pthread_kill(thread_id, SIGUSR2)) != 0) {
      LOGI("Error cancelling thread %d, error = %d (%s)", thread_id, status, strerror(status));
      if (log_fp)
        fprintf(log_fp, "Error cancelling thread %d, error = %d (%s)", thread_id, status,
                strerror(status));
    }

    (*env)->DeleteGlobalRef(env, gl_ActivityClass);
    (*env)->DeleteGlobalRef(env, gl_ActivityObj);

    thread_id = 0;
}

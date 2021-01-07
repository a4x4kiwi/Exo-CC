
#ifndef EXOPLAYER_RELEASE_V2_ANDROID_LOG_H
#define EXOPLAYER_RELEASE_V2_ANDROID_LOG_H

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "CCEJNI"
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

#endif //EXOPLAYER_RELEASE_V2_ANDROID_LOG_H

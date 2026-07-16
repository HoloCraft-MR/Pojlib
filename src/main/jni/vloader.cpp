//
// Created by Judge on 12/23/2021.
//
#include <thread>
#include <chrono>
#include <string>
#include <errno.h>
#include <android/hardware_buffer.h>
#include <fcntl.h>
#include <unistd.h>
#include <jni.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_android.h>
#include <environ/environ.h>
#include <GLES3/gl32.h>
#include <EGL/egl.h>
#include <openxr/openxr.h>
#include <sys/system_properties.h>
#include "log.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vivecraft_util_VLoader_getEGLDisplay(JNIEnv* env, jclass clazz) {
    return reinterpret_cast<jlong>(eglGetCurrentDisplay());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vivecraft_util_VLoader_getEGLContext(JNIEnv* env, jclass clazz) {
    return reinterpret_cast<jlong>(eglGetCurrentContext());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vivecraft_util_VLoader_getEGLConfig(JNIEnv* env, jclass clazz) {
    EGLConfig cfg;
    EGLint num_configs;

    static const EGLint attribs[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            // Minecraft required on initial 24
            EGL_DEPTH_SIZE, 24,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_NONE
    };

    eglChooseConfig(eglGetCurrentDisplay(), attribs, &cfg, 1, &num_configs);
    return reinterpret_cast<jlong>(cfg);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vivecraft_util_VLoader_getDalvikVM(JNIEnv* env, jclass clazz) {
    return reinterpret_cast<jlong>(pojav_environ->dalvikJavaVMPtr);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vivecraft_util_VLoader_getDalvikActivity(JNIEnv* env, jclass clazz) {
    return reinterpret_cast<jlong>(pojav_environ->activity);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_vivecraft_util_VLoader_setupAndroid(JNIEnv* env, jclass clazz) {
    JNIEnv *newEnv;
    pojav_environ->dalvikJavaVMPtr->AttachCurrentThread(&newEnv, NULL);
    jclass apiClass = pojav_environ->apiClass;
    jfieldID fieldID = newEnv->GetStaticFieldID(apiClass, "gameReady", "Z");
    newEnv->SetStaticBooleanField(apiClass, fieldID, true);
    jfieldID stoppedFieldID = newEnv->GetStaticFieldID(apiClass, "launcherXRStopped", "Z");
    for (int i = 0; i < 1000 && !newEnv->GetStaticBooleanField(apiClass, stoppedFieldID); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_getEGLDisplay(JNIEnv* env, jclass clazz) {
    return Java_org_vivecraft_util_VLoader_getEGLDisplay(env, clazz);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_getEGLContext(JNIEnv* env, jclass clazz) {
    return Java_org_vivecraft_util_VLoader_getEGLContext(env, clazz);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_getEGLConfig(JNIEnv* env, jclass clazz) {
    return Java_org_vivecraft_util_VLoader_getEGLConfig(env, clazz);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_getDalvikVM(JNIEnv* env, jclass clazz) {
    return Java_org_vivecraft_util_VLoader_getDalvikVM(env, clazz);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_getDalvikActivity(JNIEnv* env, jclass clazz) {
    return Java_org_vivecraft_util_VLoader_getDalvikActivity(env, clazz);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_setupAndroid(JNIEnv* env, jclass clazz) {
    Java_org_vivecraft_util_VLoader_setupAndroid(env, clazz);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_vmstudio_visor_core_client_provider_openxr_QuestVLoader_getDeviceModel(JNIEnv* env, jclass clazz) {
    char model[PROP_VALUE_MAX] = {};
    __system_property_get("ro.product.model", model);
    return env->NewStringUTF(model);
}

extern "C"
JNIEXPORT void JNICALL
Java_pojlib_util_VLoader_setAndroidInitInfo(JNIEnv *env, jclass clazz, jobject ctx) {
    pojav_environ->activity = env->NewGlobalRef(ctx);
}

//
// Created by wanglong_PC on 2019/8/28.
//

#include<jni.h>
#include<stdio.h>
#include<string.h>
#include <android/log.h>

#define LOG_TAG "FunctionControl"
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

static const char* const kClassPathName = "soft/wl/control/FunctionControl";//kClassPathName定义
static const char* const kClassDataPathName = "android/os/Parcel";

int soft_wl_sendCommand(JNIEnv* env, jobject clazz, jint idCmd, jobject request, jobject reply);

static const JNINativeMethod method_table[] = {
    {
        "sendCommand",                    "(ILandroid/os/Parcel;Landroid/os/Parcel;)I",
        (void *)soft_wl_sendCommand
    },
};

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGD("JNI_OnLoad");
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
       return result;
    }

    jclass clazz;
    clazz = env->FindClass(kClassPathName);
    if(NULL == clazz) {
        LOGD("JNITest_sendCommand clazz == null");
        return result;
    }
    if (env->RegisterNatives(clazz, method_table, sizeof(method_table) / sizeof(method_table[0])) < 0) {
        LOGD("JNITest RegisterNatives fail");
        return result;
    }
    return JNI_VERSION_1_6;
}

int soft_wl_sendCommand(JNIEnv* env, jobject clazz, jint idCmd, jobject request, jobject reply)
{
    LOGD("JNITest_sendCommand idCmd : %d", idCmd);

    jclass jcs = env->FindClass(kClassDataPathName);

    if(NULL == jcs) {
        LOGD("FindClass jcs == null");
        return -1;
    } else {
        LOGD("FindClass jcs != null");
    }

    jmethodID id_writeInt = env->GetMethodID(jcs, "writeInt", "(I)V");
    jmethodID id_setDataPosition = env->GetMethodID(jcs, "setDataPosition", "(I)V");

    LOGD("GetMethodID Complete");

    if(NULL == id_writeInt) {
        LOGD("id_writeInt == null");
        return -1;
    }

    if(NULL == id_setDataPosition) {
        LOGD("id_setDataPosition == null");
        return -1;
    }

    env->CallVoidMethod(request, id_writeInt, 200);
    env->CallVoidMethod(request, id_writeInt, 1000);
    env->CallVoidMethod(request, id_setDataPosition, 0);

    return 10;
}
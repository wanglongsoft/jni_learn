# jni_demo
&ensp;&ensp;&ensp;&ensp;该demo主要实现Java层调用C++层的功能，通过Parcel传参数，C++层修改Parcel参数，Java层获取修改后的结果  
通过该demo，你可以掌握Android JNI和NDK的基本用法，如：  
* JNI动态注册，JNI_OnLoad函数中映射native函数的方式，JNI调用C++层和Java函数的方式
* AndroidStudio JNI工程配置
## 动态注册工程配置和关键代码
#### 1. 新建一个包含native方法的文件，如：FunctionControl.java
```java
package soft.wl.control;
import android.os.Parcel;

public class FunctionControl {
    static {
        System.loadLibrary("FunctionControl");
    }
    public FunctionControl() {
    }
    native public int sendCommand(int cmd , Parcel in, Parcel out);
}
```
#### 2. 在app/scr/main目录下，新建jni目录，并创建一个C++ resource 文件，soft_wl_control_function_control.cpp(命名可自定义)
#### 3. soft_wl_control_function_control.cpp实现JNI_OnLoad函数，代码如下：
```c++
static const char* const kClassPathName = "soft/wl/control/FunctionControl";
int soft_wl_sendCommand(JNIEnv* env, jobject clazz, jint idCmd, jobject request, jobject reply);
//soft_wl_sendCommand函数指针声明，须放在method_table定义之前，不然编译时报错:未定义的标识符：soft_wl_sendCommand
//soft_wl_sendCommand名称可自定义,该名称需要和method_table的对应项名称一致

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
```
#### 4. soft_wl_control_function_control.cpp实现soft_wl_sendCommand函数，代码如下：
```c++

static const char* const kClassDataPathName = "android/os/Parcel";

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
```
#### 5. soft_wl_control_function_control.cpp添加必要的头文件和宏定义：
```c++
#include<jni.h>
#include<stdio.h>
#include<string.h>
#include <android/log.h>//log输出引入头文件

#define LOG_TAG "FunctionControl"
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
```
#### 6. jni 目录下创建两个mk文件，一个是Android.mk,另一个是Application.mk  
###### Android.mk实现如下：
```mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDLIBS := -lm -llog
LOCAL_MODULE := FunctionControl
LOCAL_SRC_FILES := soft_wl_control_function_control.cpp
include $(BUILD_SHARED_LIBRARY)
```
###### Application.mk实现如下：
```mk
APP_ABI := all
```
#### 7. 打开 Terminal，到 app/src/main/jni 目录下，执行：ndk-build，如果该命令不识别，需要将ndk目录的build路径加入环境变量
#### 8. 在app目录的build.gradle，android块里面，层级和defaultConfig并列,添加如下：
```gradle
    sourceSets {
        main() {
            jniLibs.srcDirs = ['src/main/libs'] //生成的.so库路径
            jni.srcDirs = []//屏蔽掉默认的jni编译生成过程
        }
    }
```
#### 9. 配置完成，Activity中调用
```java
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        FunctionControl functionControl = new FunctionControl();
        Parcel in = Parcel.obtain();
        Parcel out = Parcel.obtain();
        functionControl.sendCommand(1024, in, out);
        Log.d(TAG, "onCreate readInt After One : " + in.readInt());
        Log.d(TAG, "onCreate readInt After Two : " + in.readInt());
        in.recycle();
        out.recycle();
    }
```
## JNI 静态注册
#### 1. 新建一个包含native方法的文件，如：FunctionControl.java,实现参照动态注册
#### 2. 点击 Make Project，生成 FunctionControl.class 文件（或者javac命令生成）
#### 3. 执行javah命令，生成soft_wl_control_FunctionControl.h文件，将该文件复制到app/scr/main/jni目录
#### 4. 新建soft_wl_control_FunctionControl.cpp文件，包含头文件：soft_wl_control_FunctionControl.h
#### 5. 实现soft_wl_control_FunctionControl.h的Java_soft_wl_control_FunctionControl_sendCommand方法
#### 6. 后续参照动态注册
## 常见问题
* javah生头文件时报找不到Android类库（android.os.Parcel）的错误  
执行javah命令时需要指定classpath，将SDK路径下的jar包引入就可以了，如：  
javah -classpath ....\platforms\android-29\android.jar;. soft.wl.control.FunctionControl（....代表SDK路径）
* ndk-build命令报错：未声明的标识符（method_table里的函数指针）  
int soft_wl_sendCommand(JNIEnv* env, jobject clazz, jint idCmd, jobject request, jobject reply);
//函数声明放在method_table的定义前面
## 总结  
&ensp;&ensp;&ensp;&ensp;Android的应用层的类都是以Java写的，这些Java类编译为Dex文件之后，必须靠Dalvik虚拟
机( Virtual Machine)来执行，假如在执行java程序时，需要载入C&C++函数时，Dalvik虚拟机就会去加载C&C++的库，
让java层能顺利地调用  这些本地函数。这些C&C++的函数并不是在Dalvik虚拟机中运行的，效率和速度要比在Dalvik虚
拟机中运行得快很多。

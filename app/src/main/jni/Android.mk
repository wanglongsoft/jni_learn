LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDLIBS := -lm -llog
LOCAL_MODULE := FunctionControl
LOCAL_SRC_FILES := soft_wl_control_function_control.cpp
include $(BUILD_SHARED_LIBRARY)
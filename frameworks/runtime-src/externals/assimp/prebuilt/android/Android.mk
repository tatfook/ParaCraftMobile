LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= libassimp
LOCAL_MODULE_FILENAME := libassimp
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libassimp.a
include $(PREBUILT_STATIC_LIBRARY)

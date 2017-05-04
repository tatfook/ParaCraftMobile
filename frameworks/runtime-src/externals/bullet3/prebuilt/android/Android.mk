LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= libBullet3Common
LOCAL_MODULE_FILENAME := libBullet3Common
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBullet3Common.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libPhysicsBT
LOCAL_MODULE_FILENAME := libPhysicsBT
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libPhysicsBT.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBullet3Geometry
LOCAL_MODULE_FILENAME := libBullet3Geometry
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBullet3Geometry.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBulletCollision
LOCAL_MODULE_FILENAME := libBulletCollision
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBulletCollision.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBullet3OpenCL_clew
LOCAL_MODULE_FILENAME := libBullet3OpenCL_clew
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBullet3OpenCL_clew.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBulletDynamics
LOCAL_MODULE_FILENAME := libBulletDynamics
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBulletDynamics.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBullet3Dynamics
LOCAL_MODULE_FILENAME := libBullet3Dynamics
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBullet3Dynamics.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBullet2FileLoader
LOCAL_MODULE_FILENAME := libBullet2FileLoader
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBullet2FileLoader.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBullet3Collision
LOCAL_MODULE_FILENAME := libBullet3Collision
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBullet3Collision.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libBulletSoftBody
LOCAL_MODULE_FILENAME := libBulletSoftBody
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libBulletSoftBody.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= libLinearMath
LOCAL_MODULE_FILENAME := libLinearMath
LOCAL_SRC_FILES:= $(TARGET_ARCH_ABI)/libLinearMath.a
include $(PREBUILT_STATIC_LIBRARY)

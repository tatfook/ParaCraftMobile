LOCAL_PATH := $(call my-dir)

######################################################################################################
# ParaEngine (core)
######################################################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := ParaEngineCore
LOCAL_MODULE_FILENAME := ParaEngineCore

##################################
# util
MY_UTIL_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/util/*.cpp)
LOCAL_SRC_FILES += $(MY_UTIL_FILES)
MY_UTIL_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/util/*.c)
LOCAL_SRC_FILES += $(MY_UTIL_FILES)

##################################
# math
MY_MATH_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/math/*.cpp)
LOCAL_SRC_FILES += $(MY_MATH_FILES)

##################################
# Core
MY_CORE_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/Core/*.cpp)
LOCAL_SRC_FILES += $(MY_CORE_FILES)

##################################
# IO
MY_IO_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/IO/*.cpp)
LOCAL_SRC_FILES += $(MY_IO_FILES)

##################################
# Luabind
MY_LUABIND_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/luabind/src/*.cpp)
LOCAL_SRC_FILES += $(MY_LUABIND_FILES)

##################################
# info center
MY_INFO_CENTER_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/ic/*.cpp)
LOCAL_SRC_FILES += $(MY_INFO_CENTER_FILES)

##################################
# curlua
MY_CURLUA_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/curlua/*.cpp)
LOCAL_SRC_FILES += $(MY_CURLUA_FILES)
MY_CURLUA_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/curlua/*.c)
LOCAL_SRC_FILES += $(MY_CURLUA_FILES)

##################################
# debug tools
MY_DEBUGTOOLS_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/debugtools/*.cpp)
LOCAL_SRC_FILES += $(MY_DEBUGTOOLS_FILES)

##################################
# jsoncpp
MY_JSONCPP_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/jsoncpp/src/lib_json/*.cpp)
LOCAL_SRC_FILES += $(MY_JSONCPP_FILES)

##################################
# Engine
MY_Engine_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/Engine/*.cpp)
LOCAL_SRC_FILES += $(MY_Engine_FILES)

##################################
# sqlite3
MY_SQLITE3_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/sqlite3/*.c)
LOCAL_SRC_FILES += $(MY_SQLITE3_FILES)

##################################
# pb
MY_MATH_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/protocol/*.cpp)
LOCAL_SRC_FILES += $(MY_MATH_FILES)

##################################
# tolua
MY_MATH_FILES := $(wildcard $(LOCAL_PATH)/../../../cocos2d-x/external/lua/tolua/*.c)
LOCAL_SRC_FILES += $(MY_MATH_FILES)

##################################
# AutoUpdate
MY_MATH_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/AutoUpdate/*.cpp)
LOCAL_SRC_FILES += $(MY_MATH_FILES)

######################################################################################################
LOCAL_SRC_FILES :=$(LOCAL_SRC_FILES:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/tinyxml \
					$(LOCAL_PATH)/../../Classes/renderer \
					$(LOCAL_PATH)/../../Classes/math \
					$(LOCAL_PATH)/../../Classes/3dengine \
					$(LOCAL_PATH)/../../Classes/BlockEngine \
					$(LOCAL_PATH)/../../Classes/Core \
					$(LOCAL_PATH)/../../Classes/IO \
					$(LOCAL_PATH)/../../Classes/util \
					$(LOCAL_PATH)/../../Classes/NPL \
					$(LOCAL_PATH)/../../Classes/ParaScriptBindings \
					$(LOCAL_PATH)/../../Classes/Engine \
					$(LOCAL_PATH)/../../Classes/jsoncpp/include \
					$(LOCAL_PATH)/../../Classes/sqlite3 \
					$(LOCAL_PATH)/../../Classes/luabind \
					$(LOCAL_PATH)/../../../cocos2d-x/external/tinyxml2 \
					$(LOCAL_PATH)/../../../cocos2d-x/external/curl/include/android/ \
					$(LOCAL_PATH)/../../external/boost/boost_1_55_0/boost \
					$(LOCAL_PATH)/../../proj.android \
					$(LOCAL_PATH)/../../../cocos2d-x/cocos/scripting/lua-bindings/manual/platform/android \
					$(LOCAL_PATH)/../../../cocos2d-x/cocos/scripting/lua-bindings/manual \
					$(LOCAL_PATH)/../../../cocos2d-x/cocos/scripting/lua-bindings/manual/cocos2d \
					$(LOCAL_PATH)/../../../cocos2d-x/cocos/ \
					$(LOCAL_PATH)/../../../cocos2d-x/cocos/platform \
					$(LOCAL_PATH)/../../../cocos2d-x/external \
					$(LOCAL_PATH)/../../../cocos2d-x/external/lua/lua \
					$(LOCAL_PATH)/../../../cocos2d-x/external/lua/tolua \
					$(LOCAL_PATH)/../../../cocos2d-x/external/lua/luajit/include \
					$(LOCAL_PATH)/../../../cocos2d-x/cocos/audio/include \
					$(LOCAL_PATH)/../../../cocos2d-x/extensions \
					$(LOCAL_PATH)/../../../cocos2d-x \
					$(LOCAL_PATH)/../../externals/assimp/include

LOCAL_CFLAGS += -Wno-psabi
LOCAL_CFLAGS += -DBOOST_SIGNALS_NO_DEPRECATION_WARNING
LOCAL_CFLAGS += -DPARAENGINE_MOBILE
LOCAL_CFLAGS += -DPLATFORM_ANDROID
LOCAL_CFLAGS += -DPE_CORE_EXPORTING
LOCAL_CFLAGS += -DAUTOUPDATE_PARAENGINE
LOCAL_CFLAGS += -DTIXML_USE_STL
LOCAL_CFLAGS += -DSTATIC_PLUGIN_CAUDIOENGINE
LOCAL_EXPORT_CFLAGS += -Wno-psabi

LOCAL_STATIC_LIBRARIES := cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES := libboost_atomic
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_chrono
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_date_time
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_filesystem
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_iostreams
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_regex
#LOCAL_WHOLE_STATIC_LIBRARIES += libboost_serialization
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_signals
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_system
LOCAL_WHOLE_STATIC_LIBRARIES += libboost_thread
#LOCAL_WHOLE_STATIC_LIBRARIES += libboost_wserialization

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(call my-dir)

######################################################################################################
# ParaEngine (graphics)
######################################################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := ParaEngineGraphics
LOCAL_MODULE_FILENAME := ParaEngineGraphics

##################################
# tinyxml
MY_RENDERER_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/tinyxml/*.cpp)
LOCAL_SRC_FILES += $(MY_RENDERER_FILES)

##################################
# renderer
MY_RENDERER_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/renderer/*.cpp)
LOCAL_SRC_FILES += $(MY_RENDERER_FILES)

##################################
# blockengine cpp
MY_BLOCKENGINE_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/BlockEngine/*.cpp)
LOCAL_SRC_FILES += $(MY_BLOCKENGINE_FILES)

##################################
# bmax model cpp
MY_BMAX_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/BMaxModel/*.cpp)
LOCAL_SRC_FILES += $(MY_BMAX_FILES)

##################################
# paracript bindings
MY_PARASCRIPT_BINDINGS_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/ParaScriptBindings/*.cpp)
LOCAL_SRC_FILES += $(MY_PARASCRIPT_BINDINGS_FILES)

##################################
# NPL
MY_NPL_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/NPL/*.cpp)
LOCAL_SRC_FILES += $(MY_NPL_FILES)

##################################
# 2dengine
MY_2DENGINE_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/2dengine/*.cpp)
LOCAL_SRC_FILES += $(MY_2DENGINE_FILES)

##################################
# PaintEngine
MY_PAINTENGINE_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/PaintEngine/*.cpp)
LOCAL_SRC_FILES += $(MY_PAINTENGINE_FILES)

##################################
# 3dengine
MY_3DENGINE_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/3dengine/*.cpp)
LOCAL_SRC_FILES += $(MY_3DENGINE_FILES)

##################################
# ParaXModel
MY_PARAXMODEL_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/ParaXModel/*.cpp)
LOCAL_SRC_FILES += $(MY_PARAXMODEL_FILES)

##################################
# terrain
MY_Terrain_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/terrain/*.cpp)
LOCAL_SRC_FILES += $(MY_Terrain_FILES)


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
					$(LOCAL_PATH)/../../Classes/WebSocket \
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
LOCAL_CFLAGS += -DTIXML_USE_STL
LOCAL_CFLAGS += -DPLATFORM_ANDROID
LOCAL_CFLAGS += -DPE_CORE_EXPORTING
LOCAL_CFLAGS += -DAUTOUPDATE_PARAENGINE
LOCAL_CFLAGS += -DSTATIC_PLUGIN_CAUDIOENGINE
LOCAL_CFLAGS += -DSTATIC_PLUGIN_PHYSICS_BT
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




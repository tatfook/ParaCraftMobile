LOCAL_PATH := $(call my-dir)
# @Note LiXizhi 2014.9.13: 
# separated paraengine files into several static library to avoid using APP_SHORT_COMMANDS will generate link error with too many argument. 
include $(LOCAL_PATH)/ParaEngineCore.mk
include $(LOCAL_PATH)/ParaEngineGraphics.mk

######################################################################################################
# cocos shared lib
######################################################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := paraenginemobile_shared
LOCAL_MODULE_FILENAME := libparaenginemobile

##################################
MY_MAIN_FILES  := $(wildcard $(LOCAL_PATH)/hellolua/*.cpp)
LOCAL_SRC_FILES := $(MY_MAIN_FILES)

##################################
# TODO: Root folder classes are unconverted code
MY_CLASSES_FILES := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
LOCAL_SRC_FILES += $(MY_CLASSES_FILES)

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
LOCAL_CFLAGS += -DPE_CORE_EXPORTING
LOCAL_CFLAGS += -DPLATFORM_ANDROID
LOCAL_CFLAGS += -DTIXML_USE_STL
LOCAL_CFLAGS += -DAUTOUPDATE_PARAENGINE
LOCAL_EXPORT_CFLAGS += -Wno-psabi
LOCAL_STATIC_LIBRARIES := cocos_curl_static
#LOCAL_STATIC_LIBRARIES += cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += cocos2dx_static
LOCAL_STATIC_LIBRARIES += luajit_static


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

LOCAL_CFLAGS += -DSTATIC_PLUGIN_CAUDIOENGINE
LOCAL_WHOLE_STATIC_LIBRARIES += libVorbis
LOCAL_WHOLE_STATIC_LIBRARIES += libOgg
#LOCAL_WHOLE_STATIC_LIBRARIES += libcAp_mp3Decoder
LOCAL_WHOLE_STATIC_LIBRARIES += libcommon
LOCAL_WHOLE_STATIC_LIBRARIES += libopenal
LOCAL_WHOLE_STATIC_LIBRARIES += libcAudio

#LOCAL_WHOLE_STATIC_LIBRARIES += libBullet3Geometry
#LOCAL_WHOLE_STATIC_LIBRARIES += libBullet3Common
#LOCAL_WHOLE_STATIC_LIBRARIES += libBullet3Collision
#LOCAL_WHOLE_STATIC_LIBRARIES += libBullet3Dynamics
LOCAL_WHOLE_STATIC_LIBRARIES += libBulletCollision
LOCAL_WHOLE_STATIC_LIBRARIES += libBulletDynamics
LOCAL_WHOLE_STATIC_LIBRARIES += libLinearMath
LOCAL_WHOLE_STATIC_LIBRARIES += libPhysicsBT
LOCAL_WHOLE_STATIC_LIBRARIES += libassimp

LOCAL_WHOLE_STATIC_LIBRARIES += ParaEngineCore
LOCAL_WHOLE_STATIC_LIBRARIES += ParaEngineGraphics


include $(BUILD_SHARED_LIBRARY)
$(call import-module,../runtime-src/external/boost/boost_1_55_0/android)
$(call import-module,../external/lua/luajit/prebuilt/android)
$(call import-module,../runtime-src/externals/cAudioEngine/prebuilt/android)
$(call import-module,../runtime-src/externals/bullet3/prebuilt/android)
$(call import-module,../runtime-src/externals/assimp/prebuilt/android)
$(call import-module,cocos)
#$(call import-module,scripting/lua-bindings/proj.android)

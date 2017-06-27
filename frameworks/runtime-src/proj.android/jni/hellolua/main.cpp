#include "AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

std::string getWorldUrl()
{
	JniMethodInfo methodInfo;
	
	if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lua/CustomActivity", "getWorldUrl", "()Ljava/lang/String;")) {
        return "";
    }
	
	jstring s = (jstring) methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	return JniHelper::jstring2string(s);
}

void cocos_android_app_init (JNIEnv* env, jobject thiz) {
    LOGD("cocos_android_app_init");
	std::string worldUrl = getWorldUrl();
	LOGD("hahahaha");
    AppDelegate *pAppDelegate = new AppDelegate(worldUrl);
}



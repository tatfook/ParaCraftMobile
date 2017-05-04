#include "Java_org_cocos2dx_lua_LocalBridge.h"
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "CCLuaJavaBridge.h"
#include <stdio.h>
#include "ParaEngine.h"
#include "NPLRuntime.h"
#include "NPL/NPLHelper.h"
#include "util/Mutex.hpp"
#include  <deque>
#include <string>
#define  LOG_TAG    "baidu_yun"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
using namespace NPL;
using namespace cocos2d;


extern "C" {
	// 把字节码转为十六进制码，一个字节两个十六进制，内部为字符串分配空间
	char* ByteToHex(const char* vByte, const int vLen)
	{
		if (!vByte)
			return NULL;
		char* tmp = new char[vLen * 2 + 1]; // 一个字节两个十六进制码，最后要多一个'\0'
		int tmp2;
		for (int i = 0; i < vLen; i++)
		{
			tmp2 = (int)(vByte[i]) / 16;
			tmp[i * 2] = (char)(tmp2 + ((tmp2 > 9) ? 'A' - 10 : '0'));
			tmp2 = (int)(vByte[i]) % 16;
			tmp[i * 2 + 1] = (char)(tmp2 + ((tmp2 > 9) ? 'A' - 10 : '0'));
		}
		tmp[vLen * 2] = '\0';
		return tmp;
	}
	// 把十六进制字符串，转为字节码，每两个十六进制字符作为一个字节
	unsigned char* HexToByte(const char* szHex)
	{
		if (!szHex)
			return NULL;
		int iLen = strlen(szHex);
		if (iLen <= 0 || 0 != iLen % 2)
			return NULL;
		unsigned char* pbBuf = new unsigned char[iLen / 2]; // 数据缓冲区
		int tmp1, tmp2;
		for (int i = 0; i < iLen / 2; i++)
		{
			tmp1 = (int)szHex[i * 2] - (((int)szHex[i * 2] >= 'A') ? 'A' - 10 : '0');
			if (tmp1 >= 16)
				return NULL;
			tmp2 = (int)szHex[i * 2 + 1] - (((int)szHex[i * 2 + 1] >= 'A') ? 'A' - 10 : '0');
			if (tmp2 >= 16)
				return NULL;
			pbBuf[i] = (tmp1 * 16 + tmp2);
		}
		return pbBuf;
	}

	JNIEXPORT jint JNICALL Java_org_cocos2dx_lua_LocalBridgeHandler_callLuaFunctionWithString
		(JNIEnv *env, jclass cls, jint callback, jstring key, jbyteArray value, jint size)
{
	int ret;
	jbyte* byteArr = env->GetByteArrayElements(value, NULL);
	jsize length = env->GetArrayLength(value);
	if (length != size)
	{
		OUTPUT_LOG("ByteArray may be wrong:%d %d", length, size);
		return ret;
	}
	
	//key
	const char* key_ = env->GetStringUTFChars(key, 0);
	//value
	char* buff = new char[length];
	env->GetByteArrayRegion(value, 0, length, reinterpret_cast<jbyte*>(buff));
	//OUTPUT_LOG("ByteToHex==============%s\n", ByteToHex(buff, length));

	std::string key_str(key_);
	std::string value_str(buff, length);
	delete buff;

	env->ReleaseStringUTFChars(key, key_);
	env->ReleaseByteArrayElements(value, byteArr, 0);

	NPL::NPLObjectProxy msg;
	msg["key"] = key_str;
	msg["value"] = value_str;
	std::string strOutput;

	if (!NPLHelper::NPLTableToString("msg", msg, strOutput))
	{
		OUTPUT_LOG("NPL TABLE test failed: %s\n");
		return ret;
	}
	//OUTPUT_LOG("NPL TABLE test succeeded: %s\n", strOutput.c_str());
	std::string activate_file = "script/mobile/API/local_service_wrapper.lua";
	if (ParaEngine::CGlobals::GetNPLRuntime()->GetRuntimeState("")->activate(activate_file.c_str(), strOutput.c_str(), strOutput.size())==0)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
		OUTPUT_LOG("Active file %s failed!", activate_file.c_str());
	}
	
    return ret;
}

} // extern "C"
void jni_localCall(const char* api_key, const char* data, int size, int callback)
{
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lua/LocalBridgeHandler", "Request_Msg", "(Ljava/lang/String;[BII)V")) {
		jstring stringArg1 = t.env->NewStringUTF(api_key);
		jbyte* buff = new jbyte[size];
		for (int i = 0; i < size; i++)
		{
			buff[i] = data[i];
		}

		jbyteArray outJNIArray = t.env->NewByteArray(size);  // allocate
		t.env->SetByteArrayRegion(outJNIArray, 0, size, buff);  // copy
		
		//OUTPUT_LOG("jni_localCall ByteToHex data==============%s\n", ByteToHex(data, size));
		//OUTPUT_LOG("jni_localCall ByteToHex buff==============%s\n", ByteToHex((char*)buff, size));
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, outJNIArray, size, callback);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(stringArg1);
		delete buff;

	}
}


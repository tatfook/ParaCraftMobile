#ifndef Java_org_cocos2dx_lua_LocalBridge_H
#define Java_org_cocos2dx_lua_LocalBridge_H
#include <jni.h>
#include <string>
extern "C" {
	char* ByteToHex(const char* vByte, const int vLen);
	unsigned char* HexToByte(const char* szHex);

	JNIEXPORT jint JNICALL Java_org_cocos2dx_lua_LocalBridgeHandler_callLuaFunctionWithString
		(JNIEnv *, jclass, jint, jstring, jbyteArray, jint);

} // extern "C"
extern void jni_localCall(const char* api_key, const char* data, int size, int callback);

#endif /* __Java_org_cocos2dx_lib_Cocos2dxHelper_H__ */

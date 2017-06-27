package org.cocos2dx.lua;
import java.io.UnsupportedEncodingException;
import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxHelper;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;
import org.cocos2dx.lua.AppActivity;





import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Intent;
import android.graphics.Bitmap;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.util.Log;
import android.content.Context;

import org.cocos2dx.lua.LocalBridgePB;

import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;

import org.cocos2dx.lua.BaiduYunHelper;
import org.cocos2dx.lua.AudioManagerAPI;
public class LocalBridgeHandler extends Handler {
	public enum API {
		vibrate,
		vibrateWithPattern,
		cancelVibrate,
		AudioManager_getStreamVolume,
		AudioManager_setStreamVolume,
		AudioManager_getStreamMaxVolume,
		AudioManager_setStreamMute,
		openURL,
		getMemoryInfo
	}
	private static LocalBridgeHandler instance = null;
   public static native int callLuaFunctionWithString(int callback, String key, byte[] value,int size);
	
	public static LocalBridgeHandler instance(){
		if(instance == null){
			instance = new LocalBridgeHandler();
		}
		return instance;
	}
	public static void makeResponse(int callback,String key, byte[] byteArr,String tag){
		Log.d("lllllllll", "hhhhhhhh");
		Log.d(key,tag);
	    LocalBridgeHandler.callLuaFunctionWithString(callback,key,byteArr,byteArr.length);
	}
	public static Boolean buffIsValid(byte[] buff,int size,String tag){
		if(buff.length == size){
			return true;
		}
		Log.d("buff Is Invalid:" + tag,"size:" + size + " buff.length:" + buff.length);
		return false;
	}
	public static void Request_Msg(String api_key,byte[] byteArr,int size, int callback){
		Message msg = new Message();
		Log.d("Request_Msg","api_key:" + api_key + " byteArr.length:" + byteArr.length + " size:" + size);
		if(byteArr.length != size){
			return;
		}
		msg.obj = byteArr;
		msg.arg1 = size;
		msg.arg2 = callback;
		API api = API.valueOf(api_key);
		if(api != null){
			int enum_index = api.ordinal();
			msg.what = enum_index;
			instance().sendMessage(msg);	
		}
	}
	public void handleMessage(Message msg) {
		int index = msg.what;
		int len = API.values().length;
		if(index < 0 || index > (len - 1)){
			return;
		}
		API api = API.values()[index];
		
		switch (api) {
		case vibrate:
			try {
				vibrate_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case vibrateWithPattern:
			try {
				vibrateWithPattern_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case cancelVibrate:
			try {
				cancelVibrate_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case AudioManager_getStreamVolume:
			try {
				AudioManagerAPI.AudioManager_getStreamVolume_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case AudioManager_getStreamMaxVolume:
			try {
				AudioManagerAPI.AudioManager_getStreamMaxVolume_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case AudioManager_setStreamVolume:
			try {
				AudioManagerAPI.AudioManager_setStreamVolume_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case AudioManager_setStreamMute:
			try {
				AudioManagerAPI.AudioManager_setStreamMute_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case openURL:
			try {
				openURL_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		case getMemoryInfo:
			try {
				getMemoryInfo_Request(msg);
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
			break;
		}
	}
	public static void vibrate_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.vibrate_Request req = LocalBridgePB.vibrate_Request.parseFrom(buff);
		Log.d("vibrate_Request",req.toString());
		
		Context context = Cocos2dxHelper.getActivity();
		Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
		LocalBridgePB.vibrate_Response.Builder response = LocalBridgePB.vibrate_Response.newBuilder();
		if(v != null)
		{
			v.vibrate((long)req.getTime());	
			response.setIssuccess(1);
		}else
		{
			response.setIssuccess(0);
		}
		makeResponse(callback,"vibrate_Response",response.build().toByteArray(),response.toString());
	}
	// Get instance of Vibrator from current Context
	// Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

	// Start without a delay
	// Vibrate for 100 milliseconds
	// Sleep for 1000 milliseconds
	// long[] pattern = {0, 100, 1000};

	// The '0' here means to repeat indefinitely
	// '0' is actually the index at which the pattern keeps repeating from (the start)
	// To repeat the pattern from any other point, you could increase the index, e.g. '1'
	// v.vibrate(pattern, 0);
	public static void vibrateWithPattern_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.vibrateWithPattern_Request req = LocalBridgePB.vibrateWithPattern_Request.parseFrom(buff);
		Log.d("vibrateWithPattern_Request",req.toString());
		
		Context context = Cocos2dxHelper.getActivity();
		Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
		
		LocalBridgePB.vibrate_Response.Builder response = LocalBridgePB.vibrate_Response.newBuilder();	
		if(v != null)
		{
			List<Integer> list = req.getPatternList();
			int len = list.size();
			long[] arr = new long[len];
			for(int i = 0;i < len; i++){
				arr[i] = list.get(i).longValue();
			}
			v.vibrate(arr,(int)req.getRepeatTime());	
			response.setIssuccess(1);
		}else
		{
			response.setIssuccess(0);
		}
		makeResponse(callback,"vibrate_Response",response.build().toByteArray(),response.toString());
	}
	public static void cancelVibrate_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.cancelVibrate_Request req = LocalBridgePB.cancelVibrate_Request.parseFrom(buff);
		Log.d("cancelVibrate_Request",req.toString());
		
		Context context = Cocos2dxHelper.getActivity();
		Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
		
		LocalBridgePB.vibrate_Response.Builder response = LocalBridgePB.vibrate_Response.newBuilder();	
		if(v != null)
		{
			v.cancel();	
			response.setIssuccess(1);
		}else
		{
			response.setIssuccess(0);
		}
		makeResponse(callback,"vibrate_Response",response.build().toByteArray(),response.toString());
	}
	public static void openURL_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.openURL_Request req = LocalBridgePB.openURL_Request.parseFrom(buff);
		Log.d("openURL_Request",req.toString());
		
		Activity context = Cocos2dxHelper.getActivity();
		Intent i = new Intent(Intent.ACTION_VIEW);
	    i.setData(Uri.parse(req.getUrl()));
	    context.startActivity(i);
	}
	public static void getMemoryInfo_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.getMemoryInfo_Request req = LocalBridgePB.getMemoryInfo_Request.parseFrom(buff);
		Log.d("getMemoryInfo_Request",req.toString());
		Activity context = Cocos2dxHelper.getActivity();
		ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);  
        MemoryInfo mi = new MemoryInfo();  
        am.getMemoryInfo(mi);  
		
		LocalBridgePB.getMemoryInfo_Response.Builder response = LocalBridgePB.getMemoryInfo_Response.newBuilder();
		long availMem = 0;
		long totalMem = 0;
		if(objectHasProperty(mi,"availMem"))
		{
			availMem = mi.availMem;
		}
		if(objectHasProperty(mi,"totalMem"))
		{
			totalMem = mi.totalMem;
		}
		response.setAvailMem(availMem);
		response.setTotalMem(totalMem);
		makeResponse(callback,"getMemoryInfo_Response",response.build().toByteArray(),response.toString());
	}
	private static Boolean objectHasProperty(Object obj, String propertyName){
		if(obj == null)
		{
			return false;
		}
	    List<Field> properties = getAllFields(obj);
	    for(Field field : properties){
	        if(field.getName().equalsIgnoreCase(propertyName)){
	            return true;
	        }
	    }
	    return false;
	}

	private static List<Field> getAllFields(Object obj){
	    List<Field> fields = new ArrayList<Field>();
	    getAllFieldsRecursive(fields, obj.getClass());
	    return fields;
	}

	private static List<Field> getAllFieldsRecursive(List<Field> fields, Class<?> type) {
	    for (Field field: type.getDeclaredFields()) {
	        fields.add(field);
	    }

	    if (type.getSuperclass() != null) {
	        fields = getAllFieldsRecursive(fields, type.getSuperclass());
	    }

	    return fields;
	}
}

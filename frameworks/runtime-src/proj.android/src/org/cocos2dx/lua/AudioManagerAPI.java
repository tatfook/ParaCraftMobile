package org.cocos2dx.lua;

import android.content.Context;
import android.media.AudioManager;
import android.os.Message;
import android.util.Log;

import com.google.protobuf.InvalidProtocolBufferException;
import org.cocos2dx.lua.LocalBridgePB;
import org.cocos2dx.lua.LocalBridgeHandler;
import org.cocos2dx.lib.Cocos2dxHelper;
public class AudioManagerAPI {
	public static AudioManager getAudioManager(){
		Context context = Cocos2dxHelper.getActivity();
		AudioManager audiomanage = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);  
		return audiomanage;
	}
	public static void AudioManager_getStreamVolume_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.AudioManager_getStreamVolume_Request req = LocalBridgePB.AudioManager_getStreamVolume_Request.parseFrom(buff);
		Log.d("AudioManager_getStreamVolume_Request",req.toString());
		int v = getAudioManager().getStreamVolume(AudioManager.STREAM_MUSIC);
		
		LocalBridgePB.AudioManager_getStreamVolume_Response.Builder response = LocalBridgePB.AudioManager_getStreamVolume_Response.newBuilder();
		response.setIssuccess(1);
		response.setValue(v);
		LocalBridgeHandler.makeResponse(callback,"AudioManager_getStreamVolume_Response",response.build().toByteArray(),response.toString());
	}
	public static void AudioManager_setStreamVolume_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.AudioManager_setStreamVolume_Request req = LocalBridgePB.AudioManager_setStreamVolume_Request.parseFrom(buff);
		Log.d("AudioManager_setStreamVolume_Request",req.toString());
		getAudioManager().setStreamVolume(AudioManager.STREAM_MUSIC,req.getValue(),AudioManager.FLAG_PLAY_SOUND);
		
		LocalBridgePB.AudioManager_setStreamVolume_Response.Builder response = LocalBridgePB.AudioManager_setStreamVolume_Response.newBuilder();
		response.setIssuccess(1);
		LocalBridgeHandler.makeResponse(callback,"AudioManager_setStreamVolume_Response",response.build().toByteArray(),response.toString());
	}
	public static void AudioManager_getStreamMaxVolume_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.AudioManager_getStreamMaxVolume_Request req = LocalBridgePB.AudioManager_getStreamMaxVolume_Request.parseFrom(buff);
		Log.d("AudioManager_getStreamMaxVolume_Request",req.toString());
		int v = getAudioManager().getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		
		LocalBridgePB.AudioManager_getStreamMaxVolume_Response.Builder response = LocalBridgePB.AudioManager_getStreamMaxVolume_Response.newBuilder();
		response.setIssuccess(1);
		response.setValue(v);
		LocalBridgeHandler.makeResponse(callback,"AudioManager_getStreamMaxVolume_Response",response.build().toByteArray(),response.toString());
	}
	public static void AudioManager_setStreamMute_Request(Message msg) throws InvalidProtocolBufferException{
		byte[] buff = (byte[])msg.obj;
		int size = msg.arg1;
		final int callback = msg.arg2;
		LocalBridgePB.AudioManager_setStreamMute_Request req = LocalBridgePB.AudioManager_setStreamMute_Request.parseFrom(buff);
		Log.d("AudioManager_setStreamMute_Request",req.toString());
		Boolean b = req.getValue() == 1?true:false; 
		getAudioManager().setStreamMute(AudioManager.STREAM_MUSIC,b);
		
		LocalBridgePB.AudioManager_setStreamMute_Response.Builder response = LocalBridgePB.AudioManager_setStreamMute_Response.newBuilder();
		response.setIssuccess(1);
		LocalBridgeHandler.makeResponse(callback,"AudioManager_setStreamMute_Response",response.build().toByteArray(),response.toString());
	}
}

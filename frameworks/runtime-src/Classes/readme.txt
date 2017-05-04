---++ ParaCraftMobile
Author: LiXizhi
Date: 2014.8.16

---++ Build

---++ Build or upgrade cocos2dx (andriod on win32)
cocos2dx (build or upgrade): http://www.cocos2d-x.org/
download and unzip it to: ParaCraftMobile/frameworks/cocos2d-x  (overwrite this folder if you are upgrading)

---+++ apply tinyxml2 fix
the default version of tinyxml2 from cocos is imcompatible with tinyxml, 
and generate errors when the file contains <%=Eval()%> which is required by mcml. 

To fix it, replace cocos's external file with following. 
From:   Mobile\trunk\ParaCraftMobile\frameworks\runtime-src\Classes\external\tinyxml2\tinyxml2.cpp
To:		Mobile\trunk\ParaCraftMobile\frameworks\cocos2d-x\external\tinyxml2\tinyxml2.cpp

---+++ (Optional) apply CCFileUtils thread-safty fix
Cocos CCFileUtils is NOT thead safe due to usage of _fullPathCache. 
modify cocos\platform\CCFileUtils.cpp(*.h) to make it thread safe.
See #define COCOS_IO_THREAD_SAFE_PARAENGINE

To fix it, replace cocos's file with following. 
For cocos v 3.4 patch: see below
From:   
	runtime-src\Classes\external\cocos_patch\platform\CCFileUtils.cpp
	runtime-src\Classes\external\cocos_patch\platform\CCFileUtils.h
To: 
	cocos2d-x\cocos\platform\CCFileUtils.*
If you do not apply this patch, ParaEngine will use its own lock at higher level, 
but will lead to dead lock for unfound file under win32 due to SendMessageToLog in cocos code. 
To achieve max parallism in file IO, one should always apply this patch in any operating system. 

---+++ Remove premultiplied alpha in CCImage.cpp
cocos will use premultiplied alpha image for all png files. This is slow on load time and contradict with default blending mode, we will disable it. 

in the function Image::initWithPngData in CCImage.cpp, do following. 
<verbatim>
	// premultiplied alpha for RGBA8888
	// LiXizhi disable premultiplied alpha 2015.4.13. 
	/*if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
	premultipliedAlpha();
	}
	else*/
    {
        _hasPremultipliedAlpha = false;
    }
</verbatim>

---+++ Setup build env variables.
Maker sure following build tools are also installed (currently I have them in E:/tools/ folders):
python 2.7:	https://www.python.org/downloads/
android sdk($ANDROID_SDK_ROOT):		http://developer.android.com/sdk/index.html
android ndk($NDK_ROOT):			http://developer.android.com/tools/sdk/ndk/index.html
ant($ANT_ROOT):				http://ant.apache.org/bindownload.cgi
java($JAVA_HOME)

Run: ParaCraftMobile/frameworks/cocos2d-x/setup.py  (it will setup default environment variables for you)

@Note: when upgrading cocos, one may need to delete following folder to force a cleaner rebuild. 
ParaCraftMobile\frameworks\runtime-src\proj.android\obj 


---+++ How to Build Boost for Android with CYGWin
1. Install CYGWin
2. Copy Android SDK and boost source to:
	D:/CYGWin/opt/boost_1_55_0/
	D:/CYGWin/opt/android-ndk-r9d/
3. run 
	/opt/boost_1_55_0/bootstrap.sh
4. edit Boost Config file:
	boost_1_55_0\tools\build\v2\user-config.jam
<verbatim>

import os ;  
   
if [ os.name ] = CYGWIN || [ os.name ] = NT {  
androidPlatform = windows-x86_64 ;  
}  
else if [ os.name ] = LINUX {  
androidPlatform = linux-x86_64 ;  
}  
else if [ os.name ] = MACOSX {  
androidPlatform = darwin-x86 ;  
}  
   
modules.poke : NO_BZIP2 : 1 ;  
ANDROID_NDK = ../android-ndk-r9d ;  
using gcc : android4.8 : $(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.8/prebuilt/$(androidPlatform)/bin/arm-linux-androideabi-g++ :  
<archiver>$(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.8/prebuilt/$(androidPlatform)/bin/arm-linux-androideabi-ar  
<ranlib>$(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.8/prebuilt/$(androidPlatform)/bin/arm-linux-androideabi-ranlib  
<compileflags>--sysroot=$(ANDROID_NDK)/platforms/android-9/arch-arm  
<compileflags>-I$(ANDROID_NDK)/sources/cxx-stl/gnu-libstdc++/4.8/include  
<compileflags>-I$(ANDROID_NDK)/sources/cxx-stl/gnu-libstdc++/4.8/libs/armeabi/include  
<compileflags>-DNDEBUG  
<compileflags>-D__GLIBC__  
<compileflags>-DBOOST_FILESYSTEM_VERSION=3  
<compileflags>-lstdc++  
<compileflags>-lgnustl_shared  
<compileflags>-mthumb  
<compileflags>-fno-strict-aliasing  
<compileflags>-std=gnu++11  
<compileflags>-O2  
;  

</verbatim>

5. modify boost source since we use gnu++11
	<boost>/libs/signals/src/signal_base.cpp
//       std::auto_ptr<iterator> slot(reinterpret_cast<iterator*>(data));  
       std::unique_ptr<iterator>slot(reinterpret_cast<iterator*>(data));  

5. In CYGWin, run:
	./b2 --with-chrono toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-system toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-thread toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-date_time toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-regex toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-signals toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-serialization toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-iostreams toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android
	./b2 --with-filesystem toolset=gcc-android4.8 link=static runtime-link=static target-os=linux --stagedir=android

---+++ Build Tips for Android
During linking: there may be following error during linking
	arm-linux-androideabi-g++: error: CreateProcess: No such file or directory

This is due to link.exe   {all obj files} command line. In Win32, CreateProcess command line can only be 32KB. 
If you have too many files, the link cmd will exceed 32KB. 
To fix this, create a shorter symblic link to your source directory, and start build from there, like below:
	mklink /D PC D:\lxzsrc\ParaEngineGit\Mobile\trunk\ParaCraftMobile
	D:/PC/BuildAndriodDebug.bat
For the moment, our files have not exceeded the limit with this shortest path, but who knows the future? 

Updated solution:
in Android.mk: I seperated the big shared library into several static library. 


---+++ Debugging on android
The most important debug tool is:
E:\Tools\adt-bundle-windows-x86-20140321\sdk\platform-tools\adb.exe

Method1 (Recommended!): to view log in text file on PC
navigate to your SDK platform-tools/ directory and execute:
$ adb logcat > log.txt
run the app on andriod, and examine the log on the development computer. 
log.txt is on the dev PC.

---+++ Analyze Crash dump in release build (Android)
Method1:
   * in Application.mk added 
			APP_OPTIM := release
		this will force release build even running "cocos -m debug"
   * Generate the debug APK file, open it and replace libs/*.so in the apk with 
		ParaCraftMobile\frameworks\runtime-src\proj.android\obj\local\armeabi\*.so  (this is the non-stripped version so 150MB+)
   * Resign the APK (with tools like APKEditor or apkprote.zip in Mobile\trunk\ExternalTools\)
   * Install the new APK (now containing non-stripped debug version of release so)
   * Run app and see the core dump info in "$ adb logcat > log.txt", it will output the function and line number now. 
@note: I invented above method by reading the actual build code in Tools\android-ndk-r9d\build\core\*.mk 
it striped the debug info from release so file at last step. I decided not to modify the android build script, but manually replace so file.

Method2:
	more advanced method is running GDBServer on remote device and use GDB for debugging (yes, even for release build). 
	However, if we only want to see the crash dump info with line number, method1 is the better way. 

Mathod3: 
	To analyze core dump in production APK (no debug info in the so file), we can use a command line tool called "addr2line" on the development PC. 
		the file is located in somewhere like below from android NDK:
		E:\Tools\android-ndk-r9d\toolchains\arm-linux-androideabi-4.8\prebuilt\windows\bin\arm-linux-androideabi-addr2line.exe
	But you need to have the full *.so file anyway on your development PC (the file must be identical to the one in the production APK except for the debug info section). 
		ParaCraftMobile\frameworks\runtime-src\proj.android\obj\local\armeabi\*.so (non-stripped version so)
		the above file is always generated in both debug/release version, however the android build script will 
		use cmd-strip to remove all debug info from above so file and save the final stripped version into lib/ folder. 
Finally, we look at release version's core dump in logcat. it is something like below. 
I/DEBUG   ( 1553): backtrace:
I/DEBUG   ( 1553):     #00  pc 0081e2dc  /data/data/com.ParaEngine.ParaCraftMobile/lib/libcocos2dlua.so 
(lj_vmeta_tgetv+32) <-- not available for release build

Please note if you are lucky you get a pc(program counter) inside your so file, then you can use the address 0081e2dc and the addr2line
to find the function and line number that actually crashed your app. Note: proj.android\obj\local\armeabi\ needs to be searcheable from addr2line
I never tried this yet, but Method1 is the preferred way to go. 

---+++ Cross Compiling with MinGW(+MSYS)
because Luajit2.0.1 crashes randomly in release build of some android hardware, I need to build new luajit2.0.3 from source instead 
of using the prebuild one by cocos. 
More info about the crash: http://comments.gmane.org/gmane.comp.lang.lua.luajit/2357

Unfortunately, NDK's cross compiler does not work with CYGWin very well, 
mostly because --sysroot is not working with CYGWin's gcc command.

So we need to use MinGW+msys to cross compile libluajit.a. To setup the MinGW
see http://www.mingw.org/wiki/Getting_Started
It is very important for msys to know where MinGW is installed. by adding 
	D:\MinGW   /mingw 
to  /etc/fstab

use mklink to link NDK and luajit source to msys, here I link them to /opt/NDK9/  and /opt/lxzsrc/
Edit the build script to point to the correct version of gcc cross compiler and platform versions like below: 
	/opt/lxzsrc/ParaEngineGit/Mobile/trunk/ParaCraftMobile/frameworks/runtime-src/external/luajit/build_android.sh
Finally copy the output *.a to replace in cocos's folder.

---++ more on NDK toolchain
customizable toolchain.
http://blog.csdn.net/smfwuxiao/article/details/6587709


---++ building iOS
---+++ Building boost under MacOS
The following environment is tested:
MacOS: 10.9.5
XCode: 6.1
Python: 2.7
iOS(Simulator): iOS 8.0
---++++ Steps to build: 
make sure "frameworks\runtime-src\external\boost\boost_1_55_0\tools\build\v2\user-config.jam" is not modified before (empty file)
copy the script "proj.ios_mac/buildboost_ios.sh" to "frameworks\runtime-src\external\boost" and run from there. 
everything will be build as expected.
---+++ Building cAudioEngine under ios
git clone externals
open cmake
Where is the source code:externals/cAudioEngine
Where to build the binaries:externals/cAudioEngine_ios
SET CAUDIO_IOS_BUILD true
SET CAUDIO_STATIC true
SET ParaEngineClient_SOURCE_DIR:runtime-src/Classes
open cAudioEngine_ios/cAudio.xcodeproj
run
    Procude
        BuildFor
            Profiling
load libs:
cAudioEngine_ios/Plugins/mp3Decoder/Release-iphoneos/libcAp_mp3Decoder.a
cAudioEngine_ios/DependenciesSource/libogg-1.2.2/Release-iphoneos/libOgg.a
cAudioEngine_ios/DependenciesSource/libvorbis-1.3.2/Release-iphoneos/libVorbis.a
cAudioEngine_ios/cAudio/Release-iphoneos/libcAudio.a
---+++ Building ParaCraft.app under MacOS
replace Library Search Paths:
cocos2d_libs.xcodeproj          Targets     libcocos2d Mac:     Architectures Universal(32/64-bit)
    "$(SRCROOT)/../external/glfw3/prebuilt/mac"      to "$(SRCROOT)/../../runtime-src/externals/mac/glfw-3.1/lib/i386_x86_64/"
    "$(SRCROOT)/../external/chipmunk/prebuilt/mac"   to "$(SRCROOT)/../../runtime-src/externals/mac/Chipmunk-6.2.2/lib/i386_x86_64/"
cocos2d_lua_bindings.xcodeproj  Targets     libluacocos2d Mac:  Architectures Universal(32/64-bit)
    $(SRCROOT)/../../../../external/lua/luajit/prebuilt/mac
to
    $(SRCROOT)/../../../../../runtime-src/externals/mac/luajit/lib/i386
    $(SRCROOT)/../../../../../runtime-src/externals/mac/lua/lib/x86_64
needs:
external/boost/boost_1_55_0/osx-build/i386_x64/stage/lib
externals/mac/Chipmunk-6.2.2/lib/i386_x86_64/libChipmunk.a
externals/mac/glfw-3.1/lib/i386_x86_64/libglfw3.a
externals/mac/lua/lib/x86_64/liblua.a
externals/mac/luajit/lib/i386/libluajit.a
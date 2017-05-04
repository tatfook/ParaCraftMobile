# ParaCraftMobile

# References

ParaEngine/Mobile/trunk/ParaCraftMobile/frameworks/runtime-src/proj.android/

https://github.com/LiXizhi/ParaEngine/tree/master/Mobile/trunk/ParaCraftMobile/frameworks/runtime-src/Classes

https://www.freefilesync.org/

## How to build android apk on windows
### Dependencies 
- [Cocos2d-x-3.6](http://www.cocos2d-x.org/filedown/cocos2d-x-3.6.zip) [Download Link](http://www.cocos2d-x.org/filedown/cocos2d-x-3.6.zip)
- [Android SDK 20140321](http://developer.android.com/sdk/index.html) [Download Link](http://dl.google.com/android/adt/22.6.2/adt-bundle-windows-x86-20140321.zip)
- [Android NDK r9b](http://developer.android.com/tools/sdk/ndk/index.html) [Download Link](http://dl.google.com/android/ndk/android-ndk-r9b-windows-x86.zip)
- [JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
- [Ant-1.9.4](http://ant.apache.org/bindownload.cgi) [Download Link](http://archive.apache.org/dist/ant/binaries/)
- [Python 2.7](https://www.python.org/downloads/)
- [Boost 1.55.0](https://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.gz/download) for debug using vs2013
### Setup Enviroment
- $ANDROID_SDK_ROOT
- $NDK_ROOT
- $ANT_ROOT
- $JAVA_HOME
- Add the root path of python 2.7 to $Path 
### Building Steps
- Download [Cocos2d-x-3.6](http://www.cocos2d-x.org/filedown/cocos2d-x-3.6.zip) and unzip to [ParaEngine\Mobile\trunk\ParaCraftMobile\frameworks\cocos2d-x]
- Setup Cocos environment. [Reference](http://www.cocos2d-x.org/wiki/How_to_run_cpp-tests_on_Android)
- Download [external](https://github.com/LiXizhi/external/archive/master.zip) and unzip to [ParaEngine\Mobile\trunk\ParaCraftMobile\frameworks\runtime-src\external]
- Download [res](https://github.com/tatfook/ParacraftArtResource) and unzip to [ParaEngine\Mobile\trunk\ParaCraftMobile\res]
- Run [BuildAndriodDebug.bat] or [BuildAndriodRelease.bat]
### Main location of files
```
ParaCraftMobile\frameworks\cocos2d-x
ParaCraftMobile\frameworks\runtime-src\external
ParaCraftMobile\res
ParaCraftMobile\BuildAndriodDebug.bat
```

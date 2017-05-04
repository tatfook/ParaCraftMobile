# 2014.9.12 LiXizhi: this is quoted from E:\android-ndk-r9d\build\core\build-binary.mk
# NOT working with externally defined modules like boost, etc. So I will not use it, instead separate larget modules into smaller ones.  
#
# When LOCAL_SHORT_COMMANDS is defined to 'true' we are going to write the
# list of all object files and/or static/shared libraries that appear on the
# command line to a file, then use the @<listfile> syntax to invoke it.
#
# This allows us to link or archive a huge number of stuff even on Windows
# with its puny 8192 max character limit on its command-line.
# 
# APP_SHORT_COMMANDS := true

APP_STL := gnustl_static
# LiXizhi: define COCOS2D_DEBUG = 1 to enable debug output in logcat. We can also enable opengl error stack trace in android phone settings. 
#APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1 -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -DNDEBUG
APP_OPTIM := release


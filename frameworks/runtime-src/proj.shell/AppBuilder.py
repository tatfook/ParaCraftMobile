import os
from NPLCompiler import NPLCompilerOSX,NPLCompilerWin32
class AbstractAppBuilder():
    def __init__(self,paraengine_sdk_folder,paraengine_git_folder,res_folder,pkg_32_64_finished_callback):
        pass
    def UpdateResource(self):
        pass
    def UpdateScript(self):
        pass
    def Build(self):
        pass
    def MakePKG(self):
        pass
    def MakePKG_Callback_32bits(self):
        pass
    def MakePKG_Callback_64bits(self):
        pass
    def CopyResource(self):
        pass
class AppBuilderOSX(AbstractAppBuilder):
    def __init__(self,paraengine_sdk_folder,paraengine_git_folder,res_folder,pkg_32_64_finished_callback):
        self.paraengine_sdk_folder = paraengine_sdk_folder
        self.paraengine_git_folder = paraengine_git_folder
        self.paraengine_mobile_folder = "%s/Mobile/trunk/ParaCraftMobile" % self.paraengine_git_folder
        self.res_folder = res_folder
        self.pkg_32_64_finished_callback = pkg_32_64_finished_callback;
    def UpdateResource(self):
        #pull resource
        print "pull resource %s" % self.res_folder
        os.chdir(self.res_folder)
        os.system("git pull origin master")
        #remove old resource
        print "remove old resource %s/res" % self.paraengine_mobile_folder
        os.system("rm -rf %s/res" % self.paraengine_mobile_folder)
        #copy resource
        print "copy resource %s/res %s/res" % (self.res_folder,self.paraengine_mobile_folder)
        os.system("cp -r %s/res %s/res" % (self.res_folder,self.paraengine_mobile_folder))
        #pull paracraft mobile
        print "pull ParaEngine %s" % self.paraengine_git_folder
        os.chdir(self.paraengine_git_folder)
        os.system("git pull origin master")
    def UpdateScript(self):
        print "update script"
        os.chdir(self.paraengine_sdk_folder)
        os.system("svn update script")
        os.system("svn update packages")
    def Build(self):
        print "start to build"
        xcode_project_path = "%s/frameworks/runtime-src/proj.ios_mac" % self.paraengine_mobile_folder
        app_inputpath = "%s/frameworks/runtime-src/ipa/Release/ParaCraft.app" % self.paraengine_mobile_folder
        app_outputpath = "%s/frameworks/runtime-src/ipa/Release/ParaCraft.ipa" % self.paraengine_mobile_folder
        os.system("rm -rf %s/frameworks/runtime-src/ipa/Release/" % self.paraengine_mobile_folder)
        os.chdir(xcode_project_path)
        #os.system("xcodebuild clean")
        os.system("xcodebuild -sdk iphoneos8.1")
        os.system("xcrun -sdk iphoneos8.1 PackageApplication -v %s -o %s" % (app_inputpath,app_outputpath))
    def MakePKG(self):
        npl_32 = NPLCompilerOSX(1,self.paraengine_sdk_folder,self.paraengine_git_folder,"32",self.MakePKG_Callback_32bits)
        npl_32.Start()
    def MakePKG_Callback_32bits(self):
        npl_64 = NPLCompilerOSX(1,self.paraengine_sdk_folder,self.paraengine_git_folder,"64",self.MakePKG_Callback_64bits)
        npl_64.Start()
    def MakePKG_Callback_64bits(self):
        self.CopyResource()
        if(self.pkg_32_64_finished_callback):
            self.pkg_32_64_finished_callback()
    def CopyResource(self):
        print "mv -f %s/installer/main_full_32bits.pkg %s/res/main_full_32bits.pkg" % (self.paraengine_sdk_folder,self.paraengine_mobile_folder)
        os.system("mv -f %s/installer/main_full_32bits.pkg %s/res/main_full_32bits.pkg" % (self.paraengine_sdk_folder,self.paraengine_mobile_folder))
        print "mv -f %s/installer/main_full_64bits.pkg %s/res/main_full_64bits.pkg" % (self.paraengine_sdk_folder,self.paraengine_mobile_folder)
        os.system("mv -f %s/installer/main_full_64bits.pkg %s/res/main_full_64bits.pkg" % (self.paraengine_sdk_folder,self.paraengine_mobile_folder))
class AppBuilderWin32Android(AbstractAppBuilder):
    def __init__(self,paraengine_sdk_folder,paraengine_git_folder,res_folder,pkg_32_64_finished_callback):
        self.paraengine_sdk_folder = paraengine_sdk_folder
        self.paraengine_git_folder = paraengine_git_folder
        self.paraengine_mobile_folder = "%s\\Mobile\\trunk\\ParaCraftMobile" % self.paraengine_git_folder
        self.res_folder = res_folder
        self.pkg_32_64_finished_callback = pkg_32_64_finished_callback;
    def UpdateResource(self):
        #pull resource
        print "pull resource"
        os.chdir(self.res_folder)
        #os.system("git pull origin master")
        #remove old resource
        print "remove old resource %s\\res" % self.paraengine_mobile_folder
        os.system("rd %s\\res /S /Q" % self.paraengine_mobile_folder) 
        #copy resource
        print "copy resource"
        os.system("xcopy %s\\res\\*.* %s\\res\ /S /E" % (self.res_folder,self.paraengine_mobile_folder))
        #pull paracraft mobile
        print "pull ParaEngine"
        
    def UpdateScript(self):
        os.chdir(self.paraengine_sdk_folder)
        os.system("svn update script")
        os.system("svn update packages")
    def Build(self):
        os.system("rd %s\\frameworks\\runtime-src\\proj.android\\assets /S /Q" % self.paraengine_mobile_folder)
        os.chdir(self.paraengine_mobile_folder)
        os.system("cocos run -p android")
    def MakePKG(self):
        npl_32 = NPLCompilerWin32(1,self.paraengine_sdk_folder,self.paraengine_git_folder,"32",self.MakePKG_Callback_32bits)
        npl_32.Start()
    def MakePKG_Callback_32bits(self):
        npl_64 = NPLCompilerWin32(1,self.paraengine_sdk_folder,self.paraengine_git_folder,"64",self.MakePKG_Callback_64bits)
        npl_64.Start()
    def MakePKG_Callback_64bits(self):
        self.CopyResource()
        if(self.pkg_32_64_finished_callback):
            self.pkg_32_64_finished_callback()
    def CopyResource(self):
        os.system("xcopy %s\\installer\\main_full_32bits.pkg %s\\res\ /F /Y" % (self.paraengine_sdk_folder,self.paraengine_mobile_folder))
        os.system("xcopy %s\\installer\\main_full_64bits.pkg %s\\res\ /F /Y" % (self.paraengine_sdk_folder,self.paraengine_mobile_folder))






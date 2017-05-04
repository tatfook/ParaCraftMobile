import os
import sys
import getpass
import zipfile
from PerpetualTimer import PerpetualTimer
from threading import Timer
class AbatractNPLCompiler():
    def __init__(self,interval,paraengine_sdk_folder,paraengine_git_folder,app_bits,callback):
        pass
        self.interval = interval
        self.paraengine_sdk_folder = paraengine_sdk_folder
        self.paraengine_git_folder = paraengine_git_folder
        self.app_bits = app_bits
        self.callback = callback
        self.pkg_txt_file_path = self.paraengine_sdk_folder+"/pkg_file.txt"
        self.zip_file_path = self.paraengine_sdk_folder + "/installer/main_full_mobile.zip"
        self.pkg_file_path = "%s/installer/main_full_mobile.pkg" % self.paraengine_sdk_folder
        self.pkg_txt_timer = PerpetualTimer(interval,self.CheckPkgTxtFile)
        self.pkg_timer = PerpetualTimer(interval,self.CheckPkgFile)
    def GetAppFullPath(self):
        pass
    def OpenApp(self,args):
        pass
    def CloseApp(self):
        pass
    def Clean(self):
        pass
    def MakeZip(self):
        pass
    def MakePkg(self):
        pass
    def RenamePkg(self):
        #self.CloseApp()
        filepath = "%s/installer/main_full_%sbits.pkg" % (self.paraengine_sdk_folder,self.app_bits)
        print "rename %s to %s" % (self.pkg_file_path,filepath) 
        os.rename(self.pkg_file_path,filepath)
        if(self.callback):
            self.callback()
    def CheckPkgFile(self):
        if(os.path.exists(self.pkg_file_path)):
            self.pkg_timer.cancel()
            print "found %s" % self.pkg_file_path
            timer = Timer(5,self.RenamePkg)
            timer.start()
        else:
            print "waiting for %s %sbits" % (self.pkg_file_path,self.app_bits)
    def CheckPkgTxtFile(self):
        if(os.path.exists(self.pkg_txt_file_path)):
            self.pkg_txt_timer.cancel()
            print "found %s" % self.pkg_txt_file_path
            self.MakeZip()
        else:
            print "waiting for %s %sbits" % (self.pkg_txt_file_path,self.app_bits)
    def Start(self):
        pass
class NPLCompilerOSX(AbatractNPLCompiler):
    def GetAppFullPath(self):
        app = "%s/Mobile/trunk/ParaCraftMobile/frameworks/runtime-src/macapp/Debug/ParaCraft%sbits.app" % (self.paraengine_git_folder,self.app_bits)
        return app
    def Clean(self):
        os.system("mkdir %s/installer" % self.paraengine_sdk_folder)
        os.system("rm -rf %s" % self.pkg_txt_file_path)
        os.system("rm -rf %s/bin/script" % self.paraengine_sdk_folder)
        os.system("rm -rf %s/installer/main_full_%sbits.pkg" % (self.paraengine_sdk_folder,self.app_bits)) 
        os.system("rm -rf %s/installer/main_full_mobile.pkg" % self.paraengine_sdk_folder)
        os.system("rm -rf %s/installer/main_full_mobile.zip" % self.paraengine_sdk_folder)
        pass
    def OpenApp(self,args):
        print "open -n %s --args \"%s\"" % (self.GetAppFullPath(),args)
        os.system("open -n %s --args \"%s\"" % (self.GetAppFullPath(),args))
    def CloseApp(self):
        print "CloseApp %s" % self.GetAppFullPath()
        os.system("osascript -e ' quit app \"%s\" ' " % self.GetAppFullPath())
    def MakeZip(self):
        print "MakeZip"
        f = open(self.pkg_txt_file_path,"r")
        z_f = zipfile.ZipFile(self.zip_file_path,"w",zipfile.ZIP_DEFLATED)
        for line in f.readlines():
            line = line.strip()
            print line
            z_f.write("%s/%s" % (self.paraengine_sdk_folder,line),line)
        z_f.close()
        f.close()
        self.CloseApp()
        #make pkg
        self.MakePkg()
    def MakePkg(self):
        print "MakePkg"
        self.OpenApp("bootstrapper=script/apps/Aries/Creator/Game/Commands/CommandShell.lua mc=true servermode=true func=BUILD_PKG_FROM_OSX")
        self.pkg_timer.start()
    def Start(self):
        self.Clean()
        self.OpenApp("bootstrapper=script/apps/Aries/Creator/Game/Commands/CommandShell.lua mc=true servermode=true func=BUILD_ZIP_FROM_OSX")
        self.pkg_txt_timer.start()
class NPLCompilerWin32(AbatractNPLCompiler):
    def GetAppFullPath(self):
        if(self.app_bits == "32"):
            app = "%s/ParaEngineClient.exe" % self.paraengine_sdk_folder
        else:
            app = "%s/bin64/ParaEngineClient.exe" % self.paraengine_sdk_folder
        return app
    def Clean(self):
        os.system("rd %s\\bin\\script /s /q" % self.paraengine_sdk_folder) 
        os.system("del %s\\installer\\main_full_%sbits.pkg /f /q" % (self.paraengine_sdk_folder,self.app_bits)) 
        os.system("del %s\\installer\\main_full_mobile.pkg /f /q" % self.paraengine_sdk_folder) 
        os.system("del %s\\installer\\main_full_mobile.zip /f /q" % self.paraengine_sdk_folder) 
        pass
    def OpenApp(self,args):
        print "OpenApp %s %s" % (self.GetAppFullPath(),args)
        os.system("start %s %s" % (self.GetAppFullPath(),args))
    def CloseApp(self):
        print "CloseApp %s" % self.GetAppFullPath()
        os.system("TASKKILL /F /IM %s" % self.GetAppFullPath())
    def Start(self):
        self.Clean()
        self.OpenApp('bootstrapper="script/apps/Aries/Creator/Game/Commands/CommandShell.lua" mc="true" servermode="true" func="BUILD_PKG_FROM_WIN32"')
        self.pkg_timer.start()

#coding=utf-8
#python Documents/ParaEngine/Mobile/trunk/ParaCraftMobile/frameworks/runtime-src/proj.shell/build.py
"""
build.py
接收参数
-os:                    win32_to_apk | osx_to_ipa
-r:UpdateResource()     更新资源
-s:UpdateScript()       更新脚本
-p:MakePKG()            编译脚本
-i:MakeIPA()            生成ipa
-a:-r -s -p -i

准备工作
windows下设置环境变量
    ParaEngineSDK:sdk目录
    ParaEngineGit:git程序目录
    ParacraftBuild:git资源目录
osx下设置路径
    ParaEngineSDK:Documents
    ParaEngineGit:Documents/ParaEngine git clone ParaEngine
    ParacraftBuild:Documents/ParacraftBuild git clone ParacraftBuild
checkout svn
    路径 Documents/script
    svn checkout svn://192.168.0.200/script/trunk/script --username Leio --password LeioParaEngine
    路径 Documents/packages
    svn checkout svn://192.168.0.200/script/trunk/packages --username Leio --password LeioParaEngine
在xcode下面编译debug版
    ParaCraftMobile Mac 32bits:My Mac(32-bit) 生成 macapp/Debug/ParaCraft32bits.app
    ParaCraftMobile Mac 64bits:My Mac(64-bit) 生成 macapp/Debug/ParaCraft64bits.app
"""
import os
import getpass
import getopt, sys
from optparse import OptionParser
from AppBuilder import AppBuilderOSX,AppBuilderWin32Android

def build():
    username = getpass.getuser()
    paraengine_sdk_folder = None
    paraengine_git_folder = None
    res_folder = None
    app_builder = None

    parser = OptionParser()
    parser.add_option("-o", "--operation", action="store",dest="os",type="string",default="win32_to_apk",help="win32_to_apk|osx_to_ipa")
    parser.add_option("-a", "--all", action="store_true",dest="all",default=False,help="-r:UpdateResource() -s:UpdateScript() -p:MakePKG() -i:MakeIPA() -a:-r -s -p -i")
    parser.add_option("-r", "--resource", action="store_true",dest="resource",default=False,help="UpdateResource()")
    parser.add_option("-s", "--script", action="store_true",dest="script",default=False,help="UpdateScript()")
    parser.add_option("-p", "--pkg", action="store_true",dest="pkg",default=False,help="MakePKG()")
    parser.add_option("-i", "--ipa", action="store_true",dest="ipa",default=False,help="MakeIPA()")

    (options, args) = parser.parse_args()
    args_os = options.os
    args_all = options.all
    args_resource = options.resource
    args_script = options.script
    args_pkg = options.pkg
    args_ipa = options.ipa
    if args_all:
        args_resource = True
        args_script = True
        args_pkg = True
        args_ipa = True
    print "args_os:        %s" % args_os
    print "args_all:        %s" % args_all
    print "args_resource:   %s" % args_resource
    print "args_script:     %s" % args_script
    print "args_pkg:        %s" % args_pkg
    print "args_ipa:        %s" % args_ipa
    if args_os == "win32_to_apk":
        paraengine_sdk_folder = os.getenv("ParaEngineSDK")
        paraengine_git_folder = os.getenv("ParaEngineGit")
        res_folder = os.getenv("ParacraftBuild")
    elif args_os == "osx_to_ipa":
        root_path = "/Users/%s/Documents" % username
        paraengine_sdk_folder = root_path
        paraengine_git_folder = "%s/ParaEngine" % root_path
        res_folder = "%s/ParacraftBuild" % root_path
    else:
        print "unkown operation system to build"
        return
    print "paraengine_sdk_folder:%s" % paraengine_sdk_folder 
    print "paraengine_git_folder:%s" % paraengine_git_folder 
    print "res_folder:%s" % res_folder
    if(paraengine_sdk_folder == None or paraengine_git_folder == None or res_folder == None):
        print "error:folder is none"
        return
    def pkg_32_64_finished_callback():
        if args_ipa:
            app_builder.Build()
    callback = None
    if args_ipa and args_pkg:
            callback = pkg_32_64_finished_callback
    if args_os == "win32_to_apk":
         app_builder = AppBuilderWin32Android(paraengine_sdk_folder,paraengine_git_folder,res_folder,callback)
    elif args_os == "osx_to_ipa":
         app_builder = AppBuilderOSX(paraengine_sdk_folder,paraengine_git_folder,res_folder,callback)
    else:
        print "error:app_builder is None"
        return   
    if args_resource:
        app_builder.UpdateResource()
    if args_script:
        app_builder.UpdateScript()
    if args_pkg:
        app_builder.MakePKG()
    if args_ipa and not args_pkg:
        app_builder.Build()

if __name__ == "__main__":
    build()
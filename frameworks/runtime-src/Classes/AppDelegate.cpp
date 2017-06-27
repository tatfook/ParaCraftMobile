//-----------------------------------------------------------------------------
// Class: MainLoop
// Authors:	LiXizhi, Andy, Leio
// Emails:	
// Date: 2014.8.16
// Notes:
//-----------------------------------------------------------------------------
#include "ParaEngine.h"
#include "AppDelegate.h"
#include "StringHelper.h"
#include "ParaEngineApp.h"
#include "NPLRuntime.h"
#include "AISimulator.h"
#include "ParaEngineSettings.h"
#include "MainLoopTimer.h"
#include "CGUIRootLayer.h"
#include "protocol/pb.h"
#include "ParaScriptBindings/local_bridge_manual.h"
#include "AutoUpdate/AutoUpdater.h"
#include "ParaXModel/FBXParser.h"

#include <regex>
using namespace NPL;
/** define this if you want to use cocos API with NPL. If you just use NPL, undefine this. */
// #define USE_COCOS_LUA_ENGINE_AS_MAIN_NPL_STATE

AppDelegate::AppDelegate(std::string worldUrl)
	:m_sScriptSearchPath("src")
{
	m_worldUrl = worldUrl;
}

AppDelegate::~AppDelegate()
{
	StopApp();
}


void ParaEngine::AppDelegate::GetCommandLine(std::string &sCmdLine)
{
	// TODO: modify command line here to bootstrap to different app. 
	sCmdLine.clear();
#ifdef WIN32
	const WCHAR* cmdLine = GetCommandLineW();
	sCmdLine = StringHelper::WideCharToAnsi(cmdLine);
#elif defined PLATFORM_MAC
    sCmdLine = m_MacCommandline;
#endif
	if (sCmdLine.find("bootstrapper") == string::npos)
		sCmdLine.clear();
	
	/** when servermode=true, the app can run without any rendering, mostly used as command line tool. */
	if (sCmdLine.empty()){
		sCmdLine = "bootstrapper=script/mobile/paracraft/main.lua mc=true";
		//sCmdLine = "bootstrapper=script/apps/Aries/Creator/Game/Commands/CommandShell.lua mc=true servermode=true";
	}
}
void ParaEngine::AppDelegate::SetMacCommandLine(std::string &sCmdLine){
    m_MacCommandline = sCmdLine;
}
void ParaEngine::AppDelegate::CreateParaEngineApp()
{
	std::string sCmdLine;

	// read config file
	CParaFile configFile;
	if (configFile.OpenFile("config.txt", true, NULL, false, FILE_ON_DISK))
	{
		configFile.GetNextAttribute("cmdline", sCmdLine);
		configFile.close();
	}
	if (!sCmdLine.empty())
	{
		// concat the world url if there is any
		if (!m_worldUrl.empty())
		{
			//try {
			//	std::regex re("paracraft://cmd/loadworld (.*)");
			//	std::smatch match;
			//	if (std::regex_search(m_worldUrl, match, re) && match.size() > 1)
			//	{
			//		OUTPUT_LOG("matched");
			//		std::string url = match.str(1);
			//		sCmdLine += " world=\"" + url + "\"";
			//		OUTPUT_LOG(sCmdLine.c_str());
			//	}
			//}
			//catch (std::regex_error& e) {
			//}
			// Regex match does not work here, use simple substring instead
			std::string url = m_worldUrl.substr(26);
			sCmdLine += " world=\"" + url + "\"";
		}
	}
	else
	{
		GetCommandLine(sCmdLine);
	}		
	m_pParaEngineApp.reset(new CParaEngineApp(sCmdLine.c_str()));
}

bool ParaEngine::AppDelegate::InitApp()
{
	InitSearchPath();
	CreateParaEngineApp();

	InitNPL();
	InitDirector();
	InitParaEngineApp();
	
	m_pMainTimer.reset(new MainLoopTimer(this));
	if (! m_pParaEngineApp->Is3DRenderingEnabled())
	{
		m_pParaEngineApp->Run(0);
		return false;
	}
	return true;
}

void ParaEngine::AppDelegate::StopApp()
{
	m_pMainTimer.reset();
	m_pParaEngineApp->StopApp();
}

void ParaEngine::AppDelegate::InitParaEngineApp()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vector2 screenSize(visibleSize.width, visibleSize.height);
	ParaEngineSettings::GetSingleton().SetCurrentLanguage((ParaEngine::LanguageType)getCurrentLanguage());
	m_pParaEngineApp->Init(0);
	if (m_pParaEngineApp->Is3DRenderingEnabled())
	{
		m_pParaEngineApp->SetScreenResolution(screenSize);
		m_pParaEngineApp->InitDeviceObjects();
		m_pParaEngineApp->RestoreDeviceObjects();
	}
}

const std::string& ParaEngine::AppDelegate::GetScriptSearchPath() const
{
	return m_sScriptSearchPath;
}

void ParaEngine::AppDelegate::FrameMove(float fElapsedTime)
{
	m_pParaEngineApp->FrameMove(GetTickCount()/1000.0);
	
	if (m_pParaEngineApp->GetAppState() == PEAppState_Exiting)
	{
#if PARA_TARGET_PLATFORM != PARA_PLATFORM_IOS
		StopApp();
		Director::getInstance()->end();
		exit(0);
#endif
	}
}

bool AppDelegate::applicationDidFinishLaunching()
{
	return InitApp();
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
	m_pParaEngineApp->applicationDidEnterBackground();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
	m_pParaEngineApp->applicationWillEnterForeground();
}

void AppDelegate::InitDirector()
{
	if (m_pParaEngineApp->Is3DRenderingEnabled())
	{
		Director* director = Director::getInstance();
		auto glview = director->getOpenGLView();
		if (!glview) {
#ifdef WIN32
			glview = GLViewImpl::createWithRect("ParaCraftMobile", Rect(0, 0, 1280, 720));
#elif defined PLATFORM_MAC
			glview = GLViewImpl::createWithRect("ParaCraftMobile", Rect(0, 0, 960, 640));
#else
			glview = GLViewImpl::createWithFullScreen("ParaCraftMobile");
#endif
			director->setOpenGLView(glview);
		}
		glview->setDesignResolutionSize(960, (int)(glview->getFrameSize().height), ResolutionPolicy::FIXED_HEIGHT);

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0

#else
		// turn on display FPS
		director->setDisplayStats(true);
#endif

		// set FPS. the default value is 1.0/60 if you don't call this
		// director->setAnimationInterval(1.0/30);

		InitDirectorScene();
	}
}

void ParaEngine::AppDelegate::InitDirectorScene()
{
	// create the default scene
	Scene* scene = Scene::create();
	Director::getInstance()->runWithScene(scene);

	// attach the GUIRoot object as a full screen quad layer
	CGUIRootLayer* pRootLayer = CGUIRootLayer::create();
	scene->addChild(pRootLayer);
}

void AppDelegate::InitSearchPath()
{
	cocos2d::FileUtils::getInstance()->addSearchPath("res");
	cocos2d::FileUtils::getInstance()->addSearchPath(GetScriptSearchPath().c_str());
#ifdef ANDROID
	// on android: the preferred writable path is sdcard 
	//OUTPUT_LOG("set writable path");
	const std::string writable_path = "/mnt/sdcard/paracraft/";
	ParaEngine::CParaFile::SetWritablePath(writable_path);
#endif
	// please note the writable path is also the search path. 
	cocos2d::FileUtils::getInstance()->addSearchPath(ParaEngine::CParaFile::GetWritablePath().c_str());
}

// function is called when NPL state is reloaded
void ParaEngine::AppDelegate::OnNPLStateLoaded(NPL::CNPLRuntimeState* pRuntimeState)
{
	lua_State* L = pRuntimeState->GetLuaState();
	// init local mobile related api, like baidu API, vibration, etc. 
	register_all_local_bridge(L);
	// init google protocol buffer
	luaopen_pb(L);
	// register auto updater module
	AutoUpdate::CAutoUpdater::RegisterAsNPLFile();
	// TODO: add your app specific external library here. 
}

void AppDelegate::InitNPL()
{
#ifdef USE_COCOS_LUA_ENGINE_AS_MAIN_NPL_STATE
	LuaEngine* pLuaEngine = LuaEngine::getInstance();
	lua_State* pState = pLuaEngine->getLuaStack()->getLuaState();

	// create and init NPL state based on cocos' lua_state
	CNPLRuntime::SetDefaultNPLStateType(NPLRuntimeStateType_NPL_ExternalLuaState);
	CNPLRuntime * pRuntime = CNPLRuntime::GetInstance();
	NPL::NPLRuntimeState_ptr pNPLRunTimeState = pRuntime->GetRuntimeState("");
	pNPLRunTimeState->CreateSetState(pState);
	// loading all NPL related libs
	pNPLRunTimeState->Init();

	ScriptEngineManager::getInstance()->setScriptEngine(pLuaEngine);
#else
	// do nothing, if we use default NPL engine. 
#endif
	auto pRuntime = CNPLRuntime::GetInstance()->GetRuntimeState("");

	pRuntime->StateLoaded.connect(AppDelegate::OnNPLStateLoaded);

	// force init on start. When NPL state is restarted later, it will be automatically called. 
	pRuntime->StateLoaded(pRuntime.get());
}


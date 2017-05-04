--[[
Title: main_loop
Author(s): LiXizhi, Leio, Andy
Date: 2014/8/17
Desc: set the bootstrapper to point to this file
use the lib:
------------------------------------------------------------
NPL.load("(gl)script/main.lua");
-------------------------------------------------------
]]
NPL.load("(gl)script/ide/IDE.lua")
NPL.load("(gl)script/mobile/cocos.lua");

local function RunParaEngineTest()
	LOG.std(nil, "info", "RunParaEngineTest",  "starting...");
	NPL.load("(gl)script/mobile/test/testGUI.lua");
	Tests.testGUI:TestAll();

	local api, ERR, TYPE, AUTH
	api, ERR, TYPE, AUTH = luaopen_sqlite3()
	if(api) then
		LOG.std(nil, "info", "RunParaEngineTest",  "db func"..type(luaopen_sqlite3));
		local status, handle = api.open("Database/apps.db")
		LOG.std(nil, "info", "RunParaEngineTest",  "db file opened");
	end
end

local function InitSearchPathAndPlatforms()
	collectgarbage("collect")
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)
	cc.FileUtils:getInstance():addSearchResolutionsOrder("src");
	cc.FileUtils:getInstance():addSearchResolutionsOrder("res");
	
	local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if (cc.PLATFORM_OS_IPHONE == targetPlatform) or (cc.PLATFORM_OS_IPAD == targetPlatform) or 
       (cc.PLATFORM_OS_ANDROID == targetPlatform) or (cc.PLATFORM_OS_WINDOWS == targetPlatform) or
       (cc.PLATFORM_OS_MAC == targetPlatform) then
        cclog("Current platform id is "..tostring(targetPlatform))
    end
end

local function InitApp()
	InitSearchPathAndPlatforms();

	-- running other tests:
	RunParaEngineTest();

end

local function activate()
	do
		-- redirect to sample 
		-- return NPL.activate("script/mobile/samples/HelloWorldMCML/main.lua");
		return NPL.activate("script/mobile/paracraft/main.lua");
	end

	if(main_state == nil) then
		main_state = "started";
		InitApp();
	else
		-- called each tick
		-- cclog(tostring(ParaGlobal.timeGetTime()).."\n")
	end
end
NPL.this(activate);
--[[
Author: LiXizhi
Date: 2014-8-26
Desc: testing GUI objects in mobile client
-----------------------------------------------
NPL.load("(gl)script/mobile/test/testGUI.lua");
Tests.testGUI:TestAll();
-----------------------------------------------
]]
NPL.load("(gl)script/ide/gui_helper.lua");
local testGUI = commonlib.gettable("Tests.testGUI");

function testGUI:TestAll()
	--testGUI:test_cocos_native_event();
	testGUI:test_cocos_native_Label();
	testGUI:test_gui_button();
	testGUI:test_gui_text();
	-- testGUI:test_MCML();
end

function testGUI:test_cocos_native_Label(text)
	-- debugging output
	local scene = cc.Director:getInstance():getRunningScene();
	local btnLabel = ccui.Text:create()
    btnLabel:setString(text or "Button")
    btnLabel:setFontSize(30)        
    btnLabel:setColor(cc.c3b(159, 168, 176))                
    btnLabel:setPosition(cc.p(300, 500))
    scene:addChild(btnLabel)  
end

-- Basic anim properties
function testGUI:test_gui_button()
	local _this = ParaUI.GetUIObject("test_button");
	if(_this:IsValid() == false) then
		_this = ParaUI.CreateUIObject("button", "test_button", "_ct", -300, -90, 600, 48);
		_this.text = "Test中文1";
		_this.background = "Texture/uncheckbox.png:6 6 6 6";
		--_this.background = "Texture/checkbox.png";
		--_this.background = "Texture/paracraft.jpg";
		_this:AttachToRoot();
	end
	_this.colormask = "255 255 255 128";	
	_this.scalingx = 1;
	_this.scalingy = 1;
end

function testGUI:test_gui_text()
	local _this = ParaUI.GetUIObject("test_GUIText");
	if(_this:IsValid() == false) then
		_this = ParaUI.CreateUIObject("text", "test_GUIText", "_lt", 10, 70, 200, 25);
		-- _this.text = "writablepath:"..cc.FileUtils:getInstance():getWritablePath();
		_this.text = "writablepath:"..ParaIO.GetWritablePath();
		_guihelper.SetFontColor(_this, "#00FF00");
		_this:AttachToRoot();
	end
end

function testGUI:test_MCML()
	NPL.load("(gl)script/ide/IDE.lua");
	-- 显示一个HTML页面
	NPL.load("(gl)script/kids/3DMapSystemApp/mcml/PageCtrl.lua");
	local page = Map3DSystem.mcml.PageCtrl:new({url="script/mobile/samples/HelloWorldMCML/HelloWorld.html"});
	page:Create("helloworldpage", nil, "_fi", 0, 0, 0, 0);
end

-- touch events
function testGUI:test_cocos_native_event()
	local function onTouchesEnd(touches,event)
		LOG.std(nil, "info", "onTouchesEnd");
		 for i = 1,table.getn(touches) do
			local location = touches[i]:getLocation()
		end
	end

	local scene = cc.Director:getInstance():getRunningScene()
	LOG.std(nil, "info", "touch test",  "11111111111");
	if(scene)then
		LOG.std(nil, "info", "touch test",  "22222222222");
		local size = cc.Director:getInstance():getWinSize()
		local layer = cc.Layer:create()
		layer:setTouchEnabled(true)
		LOG.std(nil, "info", "layer:isTouchEnabled()",  layer:isTouchEnabled());
		scene:addChild(layer);

		local sprite = cc.Sprite:create("Texture/blocks/top_grass_three.png")
		layer:addChild(sprite)
		sprite:setPosition(cc.p(size.width/2, size.height/2));
		local listener = cc.EventListenerTouchAllAtOnce:create()
		listener:registerScriptHandler(onTouchesEnd,cc.Handler.EVENT_TOUCHES_ENDED )
		local eventDispatcher = layer:getEventDispatcher()
		eventDispatcher:addEventListenerWithSceneGraphPriority(listener, layer)
	end
end
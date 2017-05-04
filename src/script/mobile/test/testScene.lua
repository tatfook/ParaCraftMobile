

	local visibleSize = cc.Director:getInstance():getVisibleSize()
    local origin = cc.Director:getInstance():getVisibleOrigin()
    local scene = cc.Scene:create()
	local alert = ccui.Text:create()
    alert:setString("Button")
    alert:setFontSize(30)        
    alert:setColor(cc.c3b(159, 168, 176))                
    alert:setPosition(cc.p(visibleSize.width / 2.0, visibleSize.height / 2.0))
    scene:addChild(alert)        

	cc.Director:getInstance():runWithScene(scene);


	--local loading_ui = ccs.GUIReader:getInstance():widgetFromJsonFile("UI/Loading/Loading.json")
 --   scene:addChild(loading_ui)

 --   local label = loading_ui:getChildByName("Label_5")
	--label:setString("hello world:"..(curTime or ""));

	--local progressbar = loading_ui:getChildByName("ProgressBar_3")
	--progressbar:setPercent(50);

	
	local function addNewSpriteWithCoords(parent,x,y)
		local sprite = cc.Sprite3D:create("boss1.obj")
		sprite:setTexture("farm.jpg")

		sprite:setScale(10.0)
		parent:addChild(sprite)
		sprite:setPosition(cc.p(x,y))

		local random = math.random(0, 1)
		local action = nil
		if random < 0.2 then
			action = cc.ScaleBy:create(3,2)
		elseif random < 0.4 then
			action = cc.RotateBy:create(3, 360)
		elseif random < 0.6 then
			action = cc.Blink:create(1, 3)
		elseif random < 0.8 then
			action = cc.TintBy:create(2, 0, -255, -255)
		else
			action  = cc.FadeOut:create(2)
		end

		local action_back = action:reverse()
		local seq = cc.Sequence:create(action, action_back)

		sprite:runAction(cc.RepeatForever:create(seq))
	end
	local function create()
		local layer = cc.Layer:create()
		addNewSpriteWithCoords(layer, visibleSize.width / 2, visibleSize.height / 2)
		return layer
	end
    --scene:addChild(create())

	NPL.load("(gl)script/UIFrameWork/PageManager.lua");
	local PageManager = commonlib.gettable("UIFrameWork.PageManager");
	PageManager.SetRoot(scene);
	PageManager.Show("LoadingPage",nil,true);
	PageManager.Show("LoginPage",nil,true);
--[[
Title: cocos2dx
Author(s): LiXizhi
Date: 2014/8/17
Desc: 
use the lib:
------------------------------------------------------------
NPL.load("(gl)script/mobile/cocos.lua");
-------------------------------------------------------
]]
-- cocos2d is already included in C++'s CCLuaEngine.cpp when luastate is created. 
--require "Cocos2d"
--require "Cocos2dConstants"

cclog = function(...)
    print(string.format(...))
end
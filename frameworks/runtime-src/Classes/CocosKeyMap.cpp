//-----------------------------------------------------------------------------
// Class: GUIRootLayer
// Authors:	LiXizhi
// Emails:	
// Date: 2014.8.26
// Desc: translate events to GUIRoot
//-----------------------------------------------------------------------------
#include "ParaEngine.h"
#include "VirtualKeyDef.h"
#include "CocosKeyMap.h"

using namespace ParaEngine;

DWORD ParaEngine::CocosKeyMap::s_cocos_key_map[256];

void ParaEngine::CocosKeyMap::InitCocosKeyMap()
{
	memset(s_cocos_key_map, 0, sizeof(s_cocos_key_map));
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_ESCAPE] = VK_ESCAPE;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_MENU] = VK_LWIN;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_SPACE] = VK_SPACE;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_BACKSPACE] = VK_BACK;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_CTRL] = VK_LCONTROL;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_SHIFT] = VK_LSHIFT;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_ALT] = VK_LMENU;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_KP_ENTER] = VK_RETURN;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_TAB] = VK_TAB;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_CAPS_LOCK] = VK_CAPITAL;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_SLASH] = VK_OEM_2;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_UP_ARROW] = VK_UP;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_DOWN_ARROW] = VK_DOWN;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_LEFT_ARROW] = VK_LEFT;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = VK_RIGHT;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_DELETE] = VK_DELETE;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_HOME] = VK_HOME;
	s_cocos_key_map[(int)EventKeyboard::KeyCode::KEY_END] = VK_END;
	
	for (int i = 0; i < 26;++i)
	{
		s_cocos_key_map[(int)(EventKeyboard::KeyCode::KEY_A)+i] = 'A'+i;
		s_cocos_key_map[(int)(EventKeyboard::KeyCode::KEY_CAPITAL_A) + i] = 'A' + i;
	}
	for (int i = 0; i < 9; ++i)
	{
		s_cocos_key_map[(int)(EventKeyboard::KeyCode::KEY_0) + i] = '0' + i;
	}
	for (int i = 0; i < 12; ++i)
	{
		s_cocos_key_map[(int)(EventKeyboard::KeyCode::KEY_F1) + i] = VK_F1 + i;
	}
	// TODO: add more 
}

DWORD ParaEngine::CocosKeyMap::TranslateCocosKey(EventKeyboard::KeyCode keyCode)
{
	static bool bInited = false;
	if (!bInited)
	{
		bInited = true;
		InitCocosKeyMap();
	}
	return s_cocos_key_map[(int)keyCode];
}

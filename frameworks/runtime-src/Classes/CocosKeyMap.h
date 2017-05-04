#pragma once

#include "cocos2d.h"

namespace ParaEngine
{
	USING_NS_CC;

	class CocosKeyMap
	{
	public:
		static DWORD s_cocos_key_map[256];
		static void InitCocosKeyMap();
		/** convert to virtual key code, such as VK_ESCAPE*/
		static DWORD TranslateCocosKey(EventKeyboard::KeyCode keyCode);
	};
	
}

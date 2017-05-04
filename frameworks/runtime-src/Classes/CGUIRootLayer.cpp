//-----------------------------------------------------------------------------
// Class: GUIRootLayer
// Authors:	LiXizhi
// Emails:	
// Date: 2014.8.26
// Desc: translate events to GUIRoot
//-----------------------------------------------------------------------------
#include "ParaEngine.h"
#include "cocos2d.h"
#include "ParaEngineApp.h"
#include "2dengine/GUIDirectInput.h"
#include "2dengine/GUIRoot.h"
#include "ParaWorldAsset.h"
#include "BaseObject.h"
#include "EventClasses.h"
#include "CGUIRootLayer.h"
#include "CocosKeyMap.h"

using namespace ParaEngine;

/** this is usually for testing touch input under win32*/
#define DISABLE_MOUSE_INPUT

CGUIRootLayer* s_pInstance = NULL;

ParaEngine::CGUIRootLayer::CGUIRootLayer()
{
	s_pInstance = this;
}

ParaEngine::CGUIRootLayer::~CGUIRootLayer()
{

}

CGUIRootLayer* ParaEngine::CGUIRootLayer::create()
{
	CGUIRootLayer *ret = new CGUIRootLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

CGUIRootLayer* ParaEngine::CGUIRootLayer::GetInstance()
{
	return s_pInstance;
}

bool ParaEngine::CGUIRootLayer::init()
{
	if (Layer::init())
	{
		// render at the bottom.
		setGlobalZOrder(-100);
		setPosition(0, 0);

		// register key event
		auto keyListener = EventListenerKeyboard::create();
		keyListener->onKeyPressed = CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onKeyPressed, this);
		keyListener->onKeyReleased = CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onKeyReleased, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);


		// register mouse event
		auto mouseListener = EventListenerMouse::create();
		mouseListener->onMouseDown = CC_CALLBACK_1(ParaEngine::CGUIRootLayer::onMouseDown, this);
		mouseListener->onMouseUp = CC_CALLBACK_1(ParaEngine::CGUIRootLayer::onMouseUp, this);
		mouseListener->onMouseMove = CC_CALLBACK_1(ParaEngine::CGUIRootLayer::onMouseMove, this);
		mouseListener->onMouseScroll = CC_CALLBACK_1(ParaEngine::CGUIRootLayer::onMouseScroll, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);


		// Register Touch Event
		auto touchListener = EventListenerTouchAllAtOnce::create();
		touchListener->onTouchesBegan = CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onTouchesBegan, this);
		touchListener->onTouchesMoved = CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onTouchesMoved, this);
		touchListener->onTouchesEnded = CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onTouchesEnded, this);
		touchListener->onTouchesCancelled = CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onTouchesCancelled, this);
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

		// register accelerometer event
		Device::setAccelerometerEnabled(true);
		Device::setAccelerometerInterval(0.05f);
		auto accelerationListener_ = EventListenerAcceleration::create(CC_CALLBACK_2(ParaEngine::CGUIRootLayer::onAcceleration, this));
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(accelerationListener_, this);

		// default blend function
		m_blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
	return true;
}

void ParaEngine::CGUIRootLayer::onEnter()
{
	Layer::onEnter();
}

void ParaEngine::CGUIRootLayer::InitGUIRoot()
{
	
}


void ParaEngine::CGUIRootLayer::update(float delta)
{
}

void ParaEngine::CGUIRootLayer::cleanup()
{
	// fonts assets have coco object references, so we need to release them early here. 
	CGlobals::GetAssetManager()->GetFontManager().InvalidateDeviceObjects();
	CGlobals::GetAssetManager()->GetFontManager().DeleteDeviceObjects();
}


void ParaEngine::CGUIRootLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	Layer::onKeyPressed(keyCode, event);
	DWORD virtual_key = CocosKeyMap::TranslateCocosKey(keyCode);
	if (virtual_key != 0)
	{
		// OUTPUT_LOG("-----------------key pressed: %d\n", (int)keyCode);
		// on android this is the back key. Shift+F1 to emulate it on windows
		MSG msg;
		msg.hwnd = NULL;
		msg.lParam = 0;
		msg.message = WM_KEYDOWN;
		msg.wParam = virtual_key;
		msg.time = GetTickCount();
		CGUIRoot::GetInstance()->GetKeyboard()->PushKeyEvent(msg);
		CGUIRoot::GetInstance()->GetKeyboard()->SetKeyPressed(CEventBinding::TranslateVKToDIK(msg.wParam), true);
	}
}

void ParaEngine::CGUIRootLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	Layer::onKeyReleased(keyCode, event);
	DWORD virtual_key = CocosKeyMap::TranslateCocosKey(keyCode);
	if (virtual_key != 0)
	{
		// OUTPUT_LOG("-----------------key released: %d\n", (int)keyCode);
		MSG msg;
		msg.hwnd = NULL;
		msg.lParam = 0;
		msg.message = WM_KEYUP;
		msg.wParam = virtual_key;
		msg.time = GetTickCount();
		CDirectKeyboard* pKeyboard = CGUIRoot::GetInstance()->GetKeyboard();
		if (pKeyboard)
		{
			DWORD dwKey = CEventBinding::TranslateVKToDIK(msg.wParam);
			if (!pKeyboard->IsKeyPressed(dwKey))
			{
				// some keys on android(like the ESC button) only has key release event and no key pressed event, so we will create a fake one. 
				msg.message = WM_KEYDOWN;
				// OUTPUT_LOG("-----------------key pressed: %d\n", (int)keyCode);
				CGUIRoot::GetInstance()->GetKeyboard()->PushKeyEvent(msg);
				msg.message = WM_KEYUP;
			}
			CGUIRoot::GetInstance()->GetKeyboard()->PushKeyEvent(msg);
			CGUIRoot::GetInstance()->GetKeyboard()->SetKeyPressed(dwKey, false);
		}
	}
}

/** for debugging only on win32*/
// #define DEBUG_MULTI_TOUCH 

void ParaEngine::CGUIRootLayer::translateTouchEvent(TouchEvent::TouchEventMsgType event_type, const std::vector<Touch*>& touches)
{
	int nCurTime = GetTickCount();
	for (auto touch : touches)
	{
		Vec2 pos = touch->getLocationInView();
		//OUTPUT_LOG("touch id: %d, type: %d, x: %d, y: %d\n", touch->getID(), (int)event_type, (int)pos.x, (int)pos.y);
		TouchEvent event(EH_TOUCH, event_type, touch->getID(), pos.x, pos.y, nCurTime);
		CGUIRoot::GetInstance()->handleTouchEvent(event);
#if defined(DEBUG_MULTI_TOUCH) && defined(WIN32)
		// place the second touch event at fixed position
		TouchEvent event2(EH_TOUCH, event_type, touch->getID()+1, 500, 500, nCurTime);
		CGUIRoot::GetInstance()->handleTouchEvent(event2);
#endif
	}
}

void ParaEngine::CGUIRootLayer::onTouchesCancelled(const std::vector<Touch*>&touches, cocos2d::Event *unused_event)
{
	Layer::onTouchesCancelled(touches, unused_event);
	// TODO: shall we match to touches cancelled?
	translateTouchEvent(TouchEvent::TouchEvent_POINTER_UP, touches);
}

void ParaEngine::CGUIRootLayer::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
{
	Layer::onTouchesEnded(touches, unused_event);
	translateTouchEvent(TouchEvent::TouchEvent_POINTER_UP, touches);
}

void ParaEngine::CGUIRootLayer::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
{
	Layer::onTouchesMoved(touches, unused_event);
	translateTouchEvent(TouchEvent::TouchEvent_POINTER_UPDATE, touches);
}

void ParaEngine::CGUIRootLayer::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
{
	Layer::onTouchesBegan(touches, unused_event);
	translateTouchEvent(TouchEvent::TouchEvent_POINTER_DOWN, touches);

}

void ParaEngine::CGUIRootLayer::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	m_customCommand.init(_globalZOrder);
	m_customCommand.func = CC_CALLBACK_0(CGUIRootLayer::onDraw, this, transform, flags);
	renderer->addCommand(&m_customCommand);
}

void ParaEngine::CGUIRootLayer::onDraw(const Mat4& transform, uint32_t flags)
{
	// all 2d/3d rendering here
	CGlobals::GetApp()->Render3DEnvironment();
	RestoreRenderStates();
}

void ParaEngine::CGUIRootLayer::RestoreRenderStates()
{
	// restore everything back to nil, so that cocos rendering can catch up. 
	CGlobals::GetEffectManager()->BeginEffect(TECH_NONE);
	CGlobals::GetEffectManager()->EndEffect();
	RenderDevicePtr pd3dDevice = CGlobals::GetRenderDevice();
	pd3dDevice->SetIndices(0);
	pd3dDevice->SetStreamSource(0, 0, 0, 0);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// Note by Xizhi: always enable zwrite otherwise z-clear will not working when cocos clear the zbuffer in the outer loop. 
	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	{
		// tricky: this will force blending state to be refreshed each frame, since cocos may change blending state when preparing font atlas texture. 
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); 
	}
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#ifdef USE_OPENGL_RENDERER
	GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
}

void ParaEngine::CGUIRootLayer::onAcceleration(Acceleration* acc, cocos2d::Event* unused_event)
{
	// OUTPUT_LOG("acceleration %f %f %f: %f \n", acc->x, acc->y, acc->z, acc->timestamp);
	AccelerometerEvent event(acc->x, acc->y, acc->z, acc->timestamp);
	CGUIRoot::GetInstance()->handleAccelerometerEvent(event);
}

void ParaEngine::CGUIRootLayer::onMouseDown(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;
	// mouse button: left 0, right 1, middle 2, y is height-y
	// OUTPUT_LOG("mouse down: %d  x: %f y %f\n", e->getMouseButton(), e->getCursorX(), e->getCursorY());
	CGlobals::GetApp()->SetTouchInputting(false);
	uint32 nButton = e->getMouseButton();
	int mouse_x = (int)e->getLocationInView().x;
	// invert y to convert to win32 style coordinates
	int nWidth, nHeight;
	CGlobals::GetApp()->GetWindowCreationSize(&nWidth, &nHeight);
	int mouse_y = nHeight - (int)e->getLocationInView().y;
	uint32 nMouseButton;
	if (nButton == 0){
		nMouseButton = WM_LBUTTONDOWN;
#ifdef DISABLE_MOUSE_INPUT
		return;
#endif
	}
	else if (nButton == 1)
		nMouseButton = WM_RBUTTONDOWN;
	else
		nMouseButton = WM_MBUTTONDOWN;
	CGUIRoot::GetInstance()->GetMouse()->PushMouseEvent(nMouseButton, 0, MAKELPARAM(mouse_x, mouse_y));
}

void ParaEngine::CGUIRootLayer::onMouseUp(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;
	// OUTPUT_LOG("mouse up: %d  x: %f y %f\n", e->getMouseButton(), e->getCursorX(), e->getCursorY());
	CGlobals::GetApp()->SetTouchInputting(false);
	uint32 nButton = e->getMouseButton();
	int mouse_x = (int)e->getLocationInView().x;
	// invert y to convert to win32 style coordinates
	int nWidth, nHeight;
	CGlobals::GetApp()->GetWindowCreationSize(&nWidth, &nHeight);
	int mouse_y = nHeight - (int)e->getLocationInView().y;
	uint32 nMouseButton;
	if (nButton == 0){
		nMouseButton = WM_LBUTTONUP;
#ifdef DISABLE_MOUSE_INPUT
		return;
#endif
	}
	else if (nButton == 1)
		nMouseButton = WM_RBUTTONUP;
	else
		nMouseButton = WM_MBUTTONUP;
	CGUIRoot::GetInstance()->GetMouse()->PushMouseEvent(nMouseButton, 0, MAKELPARAM(mouse_x, mouse_y));
}

void ParaEngine::CGUIRootLayer::onMouseMove(cocos2d::Event* event)
{
#ifndef DISABLE_MOUSE_INPUT
	EventMouse* e = (EventMouse*)event;
	CGlobals::GetApp()->SetTouchInputting(false);
	int mouse_x = (int)e->getLocationInView().x;
	// invert y to convert to win32 style coordinates
	int nWidth, nHeight;
	CGlobals::GetApp()->GetWindowCreationSize(&nWidth, &nHeight);
	int mouse_y = nHeight - (int)e->getLocationInView().y;
	CGUIRoot::GetInstance()->GetMouse()->PushMouseEvent(WM_MOUSEMOVE, 0, MAKELPARAM(mouse_x, mouse_y));
#endif
}

void ParaEngine::CGUIRootLayer::onMouseScroll(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;
	// step is -1 or 1 for each scroll in scrollY, x is usually 0. 
	// OUTPUT_LOG("mouse scroll: %f, %f\n", e->getScrollX(), e->getScrollY());
	int nScrollY = -(int)e->getScrollY()*WHEEL_DELTA;
	CGUIRoot::GetInstance()->GetMouse()->PushMouseEvent(WM_MOUSEWHEEL, MAKEWPARAM(0, nScrollY), 0);
}






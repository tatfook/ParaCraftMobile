#pragma once
#include "EventClasses.h"

namespace ParaEngine
{
	USING_NS_CC;

	/** ParaEngine GUI root layer on top of coco Layer
	* translate events to GUIRoot and render 
	*/
	class CGUIRootLayer : public Layer
	{
	public:
		CGUIRootLayer();
		~CGUIRootLayer();

	public:
		static CGUIRootLayer* create();
		static CGUIRootLayer* GetInstance();
	public:
		virtual void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);

		void translateTouchEvent(TouchEvent::TouchEventMsgType event_type, const std::vector<Touch*>& touches);

		virtual void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);

		virtual void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event *unused_event);

		virtual void onTouchesCancelled(const std::vector<Touch*>&touches, cocos2d::Event *unused_event);

		virtual void onMouseDown(cocos2d::Event* event);
		virtual void onMouseUp(cocos2d::Event* event);
		virtual void onMouseMove(cocos2d::Event* event);
		virtual void onMouseScroll(cocos2d::Event* event);

		virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

		virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

		virtual void onAcceleration(Acceleration* acc, cocos2d::Event* unused_event);

		virtual bool init() override;

		void InitGUIRoot();

		virtual void cleanup();

		virtual void update(float delta);

		virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
		
		virtual void onEnter();

	protected:
		void onDraw(const Mat4& transform, uint32_t flags);

		void RestoreRenderStates();

		BlendFunc m_blendFunc;
		CustomCommand m_customCommand;
	};
}
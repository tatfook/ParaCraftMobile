#pragma once

namespace ParaEngine
{
	USING_NS_CC;
	class AppDelegate;

	/** let the cocos main loop to provide ParaEngine and NPL heartbeat in main thread */
	class MainLoopTimer : public Ref
	{
	public:
		MainLoopTimer(AppDelegate* pMyApp);

		void RunTests();

		~MainLoopTimer();

		void update(float dt);
	private:
		AppDelegate* m_pApp;
	};
}

using namespace ParaEngine;



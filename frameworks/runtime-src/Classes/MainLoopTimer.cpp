//-----------------------------------------------------------------------------
// Class: MainLoop
// Authors:	LiXizhi
// Emails:	
// Date: 2014.8.16
// Notes: 
//-----------------------------------------------------------------------------
#include "ParaEngine.h"
#include "AppDelegate.h"
#include "MainLoopTimer.h"

MainLoopTimer::MainLoopTimer(AppDelegate* pMyApp)
: m_pApp(pMyApp)
{
#ifdef WIN32
	RunTests();
#endif
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
	// Director::getInstance()->getScheduler()->schedule(schedule_selector(MainLoopTimer::update), this, 0.0f, false);
}

MainLoopTimer::~MainLoopTimer()
{
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	//Director::getInstance()->getScheduler()->unschedule(schedule_selector(MainLoopTimer::update), this);
}

void MainLoopTimer::update(float dt)
{
	m_pApp->FrameMove(dt);
}

// temp test goes here:
void ParaEngine::MainLoopTimer::RunTests()
{

}

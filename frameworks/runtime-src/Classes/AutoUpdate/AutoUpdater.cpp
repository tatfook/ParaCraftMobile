//-----------------------------------------------------------------------------
// Class:	
// Authors:	LiXizhi
// Emails:	LiXizhi@yeah.net
// Company: ParaEngine Co.
// Date:	2015.3.7
// Desc: 
//-----------------------------------------------------------------------------
#include "ParaEngine.h"
#include "NPLTable.h"
#include "NPLHelper.h"
#include "INPLRuntimeState.h"
#include "AssetsWorker.h"
#include "AutoUpdater.h"
#include "NPLRuntime.h"
#include "Globals.h"
#include "SceneObject.h"
#include "MeshObject.h"
#include "MiniSceneGraph.h"
using namespace NPL;
using namespace ParaEngine;

void AutoUpdate::CAutoUpdater::RegisterAsNPLFile()
{
	ParaEngine::CGlobals::GetNPLRuntime()->GetMainRuntimeState()->RegisterFile("AutoUpdater.cpp", new CAutoUpdater());
}

NPL::NPLReturnCode AutoUpdate::CAutoUpdater::OnActivate(NPL::INPLRuntimeState* pState)
{
	auto msg = NPLHelper::MsgStringToNPLTable(pState->GetCurrentMsg(), pState->GetCurrentMsgLength());
	std::string debug_str;
	NPL::NPLHelper::NPLTableToString("msg", msg, debug_str);

	std::string sType = msg["type"];
	std::string sAction = msg["action"];
	std::string app_type = msg["app_type"];
	if (sType == "auto_update")
	{
		if (sAction == "check")
		{
			AutoUpdate::AssetsWorker::GetInstance()->OnInit(app_type);
			AutoUpdate::AssetsWorker::GetInstance()->Check();
		}
		else if (sAction == "apply")
		{
			AutoUpdate::AssetsWorker::GetInstance()->Apply();
		}
		else if (sAction == "refresh")
		{
			// Xizhi: what's this for? Remove this
			AutoUpdate::AssetsWorker::GetInstance()->Refresh();
		}
	}

	return NPL_OK;
}

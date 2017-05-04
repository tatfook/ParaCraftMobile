#pragma once

#include "NPLActivationFile.h"

namespace AutoUpdate
{
	/** patching local files from server
	* user interface and interaction should be invoked via script. e.g. 
	* 
	* NPL.activate("AutoUpdater.cpp", {type="auto_update", action="check"})
	* NPL.activate("AutoUpdater.cpp", {type="auto_update", action="apply"})
	*/
	class CAutoUpdater : public NPL::INPLActivationFile
	{
	public:
		/* call this at start up to make the service available to NPL script. */
		static void RegisterAsNPLFile();

	public:
		virtual NPL::NPLReturnCode OnActivate(NPL::INPLRuntimeState* pState);
	};

}
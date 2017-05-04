#include "ParaEngine.h"
#include "AssetsWorker.h"
#include "FileUtils.h"
#include "AutoUpdate/AssetsManagerGlobal.h"
#include "NPL/NPLHelper.h"
#include "NPLRuntime.h"
namespace AutoUpdate
{
	static AssetsWorker* instance_AssetsWorker = NULL;

	

	AssetsWorker::AssetsWorker()
		:isInit(false)
	{
		
	}
	AssetsWorker::~AssetsWorker()
	{

	}
	AssetsWorker* AssetsWorker::GetInstance()
	{
		if (instance_AssetsWorker == NULL)
		{
			instance_AssetsWorker = new AssetsWorker();
		}
		return instance_AssetsWorker;
	}
	void AssetsWorker::OnInit(const std::string& app_type)
	{
		if (!isInit)
		{
			isInit = true;
			AutoUpdate::AssetsManager::Config config;
			std::vector<std::string> patchHostList;
			if (app_type == "haqi2")
			{
				patchHostList.push_back("http://update.61.com/haqi/coreupdate_teen/");
				patchHostList.push_back("http://teenver.paraengine.com/");
				patchHostList.push_back("http://teenver.pala5.cn/");
				config.patchHostList = patchHostList;
				config.versionURL = "http://teenver.paraengine.com/version.php";
				config.localVersionTxt = FileUtils::getWritablePath() + "_apps/haqi2/version.txt";
				config.storagePath = FileUtils::getWritablePath() + "caches";
				config.writeablePath = FileUtils::getWritablePath();
				config.destStoragePath = FileUtils::getWritablePath() + "_apps/haqi2/";
			}
			else
			{
				patchHostList.push_back("http://tmlog.paraengine.com/");
				patchHostList.push_back("http://114.80.99.134/");
				patchHostList.push_back("http://update.61.com/haqi/coreupdate/");
				config.patchHostList = patchHostList;
				config.versionURL = "http://tmlog.paraengine.com/version.php";
				config.localVersionTxt = FileUtils::getWritablePath() + "_apps/paracraft/version.txt";
				config.storagePath = FileUtils::getWritablePath() + "caches";
				config.writeablePath = FileUtils::getWritablePath();
				config.destStoragePath = FileUtils::getWritablePath() + "_apps/paracraft/";
			}
			assets_manager = new AutoUpdate::AssetsManager(config);
			assets_manager->OnProgressCallback = std::bind(&AssetsWorker::OnProgressAssets, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
			assets_manager->OnMovingFileCallback = std::bind(&AssetsWorker::OnMovingFileCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			assets_manager->OnWorkingCallback = std::bind(&AssetsWorker::OnWorkingCallback, this, std::placeholders::_1);
		}
	}
	void AssetsWorker::Check()
	{
		assets_manager->check();
	}
	void AssetsWorker::Apply()
	{
		assets_manager->applyAsync();
	}
	void AssetsWorker::Refresh()
	{
		AssetsManagerGlobal::refresh();
	}
	void AssetsWorker::OnProgressAssets(std::string url, double cur_file_downloaded, double cur_file_total, double percent, double downloaded, double total)
	{
		NPL::NPLObjectProxy msg;
		msg["state"] = "DOWNLOADING_ASSETS";
		msg["url"] = url;
		msg["cur_file_downloaded"] = cur_file_downloaded;
		msg["cur_file_total"] = cur_file_total;
		msg["percent"] = percent;
		msg["downloaded"] = downloaded;
		msg["total"] = total;
		std::string msg_str;
		if (!NPL::NPLHelper::NPLTableToString("msg_auto_update", msg, msg_str)) {
			return;
		}
		Call(msg_str);
	}
	void AssetsWorker::OnMovingFileCallback(std::string filepath, int index, int len)
	{
		NPL::NPLObjectProxy msg;
		msg["state"] = "UPDATING";
		msg["filepath"] = filepath;
		msg["index"] = (double)index;
		msg["len"] = (double)len;
		std::string msg_str;
		if (!NPL::NPLHelper::NPLTableToString("msg_auto_update", msg, msg_str)) {
			return;
		}
		Call(msg_str);
	}
		void AssetsWorker::OnWorkingCallback(AssetsManager::State state)
	{
		NPL::NPLObjectProxy msg;
		switch (state)
		{
		case AutoUpdate::AssetsManager::State::UNCHECKED:
			msg["state"] = "UNCHECKED";
			break;
		case AutoUpdate::AssetsManager::State::PREDOWNLOAD_VERSION:
			msg["state"] = "PREDOWNLOAD_VERSION";
			break;
		case AutoUpdate::AssetsManager::State::DOWNLOADING_VERSION:
			msg["state"] = "DOWNLOADING_VERSION";
			break;
		case AutoUpdate::AssetsManager::State::VERSION_CHECKED:
			msg["state"] = "VERSION_CHECKED";
			if (assets_manager->isNeedUpdate())
			{
				msg["need_update"] = true;
				msg["cur_version"] = assets_manager->getCurVersion();
				msg["last_version"] = assets_manager->getLastVersion();
				assets_manager->download();
			}
			else
			{
				msg["need_update"] = false;
				msg["cur_version"] = assets_manager->getLastVersion();
			}
			break;
		case AutoUpdate::AssetsManager::State::VERSION_ERROR:
			msg["state"] = "VERSION_ERROR";
			break;
		case AutoUpdate::AssetsManager::State::MANIFEST_ERROR:
			msg["state"] = "MANIFEST_ERROR";
			break;
		case AutoUpdate::AssetsManager::State::PREDOWNLOAD_ASSETS:
			msg["state"] = "PREDOWNLOAD_ASSETS";
			break;
		case AutoUpdate::AssetsManager::State::DOWNLOADING_ASSETS:
			break;
		case AutoUpdate::AssetsManager::State::ASSETS_DOWNLOADED:
			msg["state"] = "ASSETS_DOWNLOADED";
			break;
		case AutoUpdate::AssetsManager::State::FAIL_TO_ASSETS_DOWNLOAD:
			msg["state"] = "FAIL_TO_ASSETS_DOWNLOAD";
			break;
		case AutoUpdate::AssetsManager::State::PREUPDATE:
			msg["state"] = "PREUPDATE";
			break;
		case AutoUpdate::AssetsManager::State::UPDATING:
			//msg["state"] = "UPDATING";
			break;
		case AutoUpdate::AssetsManager::State::UPDATED:
			msg["state"] = "UPDATED";
			break;
		case AutoUpdate::AssetsManager::State::FAIL_TO_UPDATED:
			msg["state"] = "FAIL_TO_UPDATED";
			break;
		default:
			break;
		}
		std::string msg_str;
		if (!NPL::NPLHelper::NPLTableToString("msg_auto_update", msg, msg_str)) {
			return;
		}
		Call(msg_str);
	}
	void AssetsWorker::Call(std::string msg)
	{
		std::string activate_file = "script/mobile/paracraft/Login/AutoUpdatePage.lua";
		ParaEngine::CGlobals::GetNPLRuntime()->GetRuntimeState("")->activate(activate_file.c_str(), msg.c_str(), msg.size());
	}
}
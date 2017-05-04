#ifndef AssetsWorker_H
#define AssetsWorker_H
#include "AutoUpdate/AssetsManager.h"
namespace AutoUpdate
{
	class AssetsWorker
	{
	public:
		AssetsWorker();
		~AssetsWorker();
		static AssetsWorker* GetInstance();
		void OnInit(const std::string& app_type);
		void Check();
		void Apply();
		void Refresh();
	private:
		void OnProgressAssets(std::string url, double cur_file_downloaded, double cur_file_total, double percent, double downloaded, double total);
		void OnMovingFileCallback(std::string filepath, int index, int len);
		void OnWorkingCallback(AssetsManager::State state);
		void Call(std::string msg);
		AssetsManager* assets_manager;
		bool isInit;
	};
}
#endif
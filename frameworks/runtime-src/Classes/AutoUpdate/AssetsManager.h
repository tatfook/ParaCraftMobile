#ifndef AssetsManager_H
#define AssetsManager_H
#include "AutoUpdate/Downloader.h"
#include <string>
#include <vector>
namespace AutoUpdate
{
	class AssetsManager
	{
	public:
		enum class State
		{
			UNCHECKED,
			PREDOWNLOAD_VERSION,
			DOWNLOADING_VERSION,
			VERSION_CHECKED,
			VERSION_ERROR,
			MANIFEST_ERROR,
			PREDOWNLOAD_ASSETS,
			DOWNLOADING_ASSETS,
			ASSETS_DOWNLOADED,
			FAIL_TO_ASSETS_DOWNLOAD,
			PREUPDATE,
			UPDATING,
			UPDATED,
			FAIL_TO_UPDATED
		};
		typedef std::function<void(std::string url, double cur_file_downloaded, double cur_file_total, double percent, double downloaded, double total)>			progress_callback;
		typedef std::function<void(std::string filepath,int index,int len)>			moving_file_callback;
		progress_callback OnProgressCallback;
		moving_file_callback OnMovingFileCallback;
		typedef std::function<void(State state)>			working_callback;
		working_callback OnWorkingCallback;
		
		/** suppose version number "1.2.3", where 1 is the first(major) version, 2 is the second version, 3 is the third version.*/
		enum CheckVersionEnum
		{
			CheckVersion_SameAsLast, // 0
			CheckVersion_FirstVersionChanged, // 1
			CheckVersion_SecondVersionChanged, // 2
			CheckVersion_ThirdVersionChanged, // 3
			CheckVersion_Error, // -1
		};
		struct Config
		{
			std::string versionURL;
			std::string localVersionTxt;
			std::vector<std::string> patchHostList;
			std::string storagePath; //caches storage path
			std::string writeablePath;
			std::string destStoragePath;//destination storage path
		};
		const static std::string VERSION_ID;
		const static std::string MANIFEST_ID;
		const static std::string BATCH_UPDATE_ID;
		const static std::string BATCH_UPDATE_FAILED_ID;
		AssetsManager(const Config& config);
		~AssetsManager();
		
		void check(const std::string &version = "");
		void download();
		void apply();
		void applyAsync();
		const std::string& getCurVersion(){ return _curVersion; }
		const std::string& getLastVersion(){ return _lastVersion; }
		bool  isNeedUpdate(){ return _needUpdate; }
		float getPercent(){ return _percent; };
		bool hasVersionFile(){ return _hasVersionFile; }
		Downloader::DownloadUnits* getFailedDownloadUnits(){ return &_failedDownloadUnits; }

	protected:
		Config _config;
		std::shared_ptr<Downloader> _downloader;
		std::string _curVersion;
		std::string _lastVersion;
		std::string _manifestTxt;
		std::string _validPatchHost;
		std::string _cacheVersionPath;
		std::string _cacheManifestPath;
		std::string _asstesCachesPath;

		CheckVersionEnum _comparedVersion;
		bool _needUpdate;
		bool _hasVersionFile;

		//! All assets unit to download
		Downloader::DownloadUnits _downloadUnits;
		//! All failed units
		Downloader::DownloadUnits _failedDownloadUnits;
		Downloader::DownloadUnits _failedMD5Units;
		Downloader::DownloadUnits _failedUncompressUnits;


		//! Download percent
		float _percent;
		//! Total file size need to be downloaded (sum of all file)
		double _totalSize;
		//! Downloaded size for each file
		std::unordered_map<std::string, double> _downloadedSize;

		int validHostIndex;

	private:
		void setStoragePath(const std::string& storagePath);
		void adjustPath(std::string &path);
		void onError(const Downloader::Error &error);
		void onProgress(double total, double downloaded, const std::string &url, const std::string &customId);
		void onSuccess(const std::string &srcUrl, const std::string &storagePath, const std::string &customId);
		void callWorkingCallback(State state);
		//读取本地version
		void loadLocalVersion();
		//加载远程version
		void downloadVersion();
		void parseVersion();
		//对比version
		CheckVersionEnum compareVersions();
		//下载更新列表
		void downloadManifest(CheckVersionEnum ret, int hostServerIndex);
		void parseManifest();
		bool checkMD5(const Downloader::DownloadUnit& unit);
		bool decompress(std::string sourceFileName, std::string destFileName);
		const std::string getPatchListUrl(bool is_full_list, int nCandidate /*= 0*/) const;
		void deleteOldFiles();
	};
}
#endif
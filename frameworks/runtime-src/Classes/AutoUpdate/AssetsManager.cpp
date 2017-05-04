#include "ParaEngine.h"
#include "AssetsManager.h"
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <functional>
#include <algorithm>
#include "AssetsManagerGlobal.h"
#include "FileUtils.h"
#include "tinyxml2.h"
#include "zlib.h"
#include <algorithm>
#include <thread>
#define CHUNK 16384
#include <cstdio>
#include <time.h>
#define DEFAULT_CONNECTION_TIMEOUT 8
/** full list or patch file's file extension. It used to be .txt, however, some computers seems to block any known file format. so .p is a better choice. */
// #define FILE_LIST_FILE_EXT	".txt"
#define FILE_LIST_FILE_EXT	".p"
#define VERSION_FILENAME        "version.manifest"
#define MANIFEST_FILENAME       "project.manifest"
namespace AutoUpdate
{
	const std::string AssetsManager::VERSION_ID = "@version";
	const std::string AssetsManager::MANIFEST_ID = "@manifest";
	const std::string AssetsManager::BATCH_UPDATE_ID = "@batch_update";
	const std::string AssetsManager::BATCH_UPDATE_FAILED_ID = "@batch_update_failed";
	AssetsManager::AssetsManager(const Config& config)
		:_config(config)
		, _validPatchHost("")
		, _cacheVersionPath("")
		, _cacheManifestPath("")
		, _asstesCachesPath("")
		, _needUpdate(false)
		, _percent(0)
		, _hasVersionFile(false)
		, validHostIndex(0)
	{
		_downloader = std::make_shared<Downloader>();
		_downloader->setConnectionTimeout(DEFAULT_CONNECTION_TIMEOUT);
		_downloader->setErrorCallback(std::bind(&AssetsManager::onError, this, std::placeholders::_1));
		_downloader->setProgressCallback(std::bind(&AssetsManager::onProgress,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4));
		_downloader->setSuccessCallback(std::bind(&AssetsManager::onSuccess, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		adjustPath(_config.storagePath);
		setStoragePath(_config.storagePath);
		_cacheVersionPath = _config.storagePath + VERSION_FILENAME;
		_cacheManifestPath = _config.storagePath + MANIFEST_FILENAME;

	}
	AssetsManager::~AssetsManager()
	{

	}
	void AssetsManager::setStoragePath(const std::string& storagePath)
	{
		FileUtils::createDirectory(storagePath);
	}

	void AssetsManager::adjustPath(std::string &path)
	{
		if (path.size() > 0 && path[path.size() - 1] != '/')
		{
			path.append("/");
		}
	}
	void AssetsManager::check(const std::string &version)
	{
		callWorkingCallback(State::PREDOWNLOAD_VERSION);
		_hasVersionFile = FileUtils::isFileExist(_config.localVersionTxt);
		_downloadUnits.clear();
		_downloadedSize.clear();
		_failedDownloadUnits.clear();
		_failedMD5Units.clear();
		_failedUncompressUnits.clear();
		_manifestTxt = "";
		_percent = _totalSize = 0;
		FileUtils::removeFile(_cacheVersionPath);
		FileUtils::removeFile(_cacheManifestPath);
		if (version.empty())
		{
			loadLocalVersion();
		}
		else
		{
			_curVersion = version;
		}
		downloadVersion();
	}
	void AssetsManager::loadLocalVersion()
	{
		std::string s = FileUtils::getStringFromFile(_config.localVersionTxt);
		s = AssetsManagerGlobal::trim(s.c_str());
		if (s.empty())
		{
			OUTPUT_LOG1("can't open version.txt,curver set to 0!\n");
			_curVersion = "0";
			return;
		}
		std::string::size_type start_pos = s.find("=");
		if (start_pos != std::string::npos)
		{
			start_pos += 1;
			_curVersion = s.substr(start_pos);
		}
		else
		{
			OUTPUT_LOG1("version.txt contains invalid version number,curver set to 0!\n");
			_curVersion = "0";
			return;
		}
	}
	void AssetsManager::downloadVersion()
	{
		time_t timer = time(NULL);
		tm* dt = std::localtime(&timer);
		std::string url = AssetsManagerGlobal::format("%s?dt=%04d%02d%02d%02d%02d%02d", _config.versionURL.c_str(), dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
		_downloader->downloadAsync(url, _cacheVersionPath, VERSION_ID);
	}
	void AssetsManager::parseVersion()
	{
		std::string buff = FileUtils::getStringFromFile(_cacheVersionPath);
		tinyxml2::XMLDocument doc(true, tinyxml2::COLLAPSE_WHITESPACE);
		std::string body = "<root>" + buff + "</root>";
		doc.Parse(body.c_str());
		tinyxml2::XMLNode* pChild = doc.FirstChild()->FirstChild();
		if (pChild)
		{
			for (; pChild; pChild = pChild->NextSibling())
			{
				if (pChild->ToElement())
				{
					tinyxml2::XMLElement* element = pChild->ToElement();
					std::string name = element->Name();
					std::string value = element->GetText();
					if (name == "UpdateVersion")
					{
						_lastVersion = value;
					}
				}
			}
		}
	}
	AssetsManager::CheckVersionEnum AssetsManager::compareVersions()
	{
		if (_curVersion == "0")
		{
			_needUpdate = true;
			return CheckVersion_FirstVersionChanged;
		}
		if (_curVersion == _lastVersion)
		{
			_needUpdate = false;
			return CheckVersion_SameAsLast;
		}
		std::string::size_type pos_curver = _curVersion.find(".");
		std::string::size_type pos_lastver = _lastVersion.find(".");
		if (pos_curver == std::string::npos || pos_lastver == std::string::npos)
		{
			return CheckVersion_Error;
		}
		std::string scurVN1 = _curVersion.substr(0, pos_curver);
		std::string slastVN1 = _lastVersion.substr(0, pos_lastver);
		if (scurVN1 != slastVN1)
		{
			_needUpdate = true;
			return CheckVersion_FirstVersionChanged;
		}
		std::string scurLeft = _curVersion.substr(pos_curver + 1);
		std::string slastLeft = _lastVersion.substr(pos_lastver + 1);

		pos_curver = scurLeft.find(".");
		pos_lastver = slastLeft.find(".");
		if (pos_curver == std::string::npos || pos_lastver == std::string::npos)
		{
			return CheckVersion_Error;
		}
		std::string scurVN2 = scurLeft.substr(0, pos_curver);
		std::string slastVN2 = slastLeft.substr(0, pos_lastver);
		if (scurVN2 != slastVN2)
		{
			_needUpdate = true;
			return CheckVersion_SecondVersionChanged;
		}
		_needUpdate = true;
		return CheckVersion_ThirdVersionChanged;
	}
	void AssetsManager::downloadManifest(CheckVersionEnum ret, int hostServerIndex)
	{
		int len = _config.patchHostList.size();
		if (hostServerIndex > (len - 1))
		{
			return;
		}
		std::string updatePackUrl;
		if (CheckVersion_FirstVersionChanged == ret || CheckVersion_SecondVersionChanged == ret)
		{
			updatePackUrl = getPatchListUrl(true, hostServerIndex);
		}
		else if (CheckVersion_ThirdVersionChanged == ret)
		{
			updatePackUrl = getPatchListUrl(false, hostServerIndex);
		}
		else
		{
			updatePackUrl = getPatchListUrl(true, hostServerIndex);
		}
		std::string hostServer = _config.patchHostList[validHostIndex];
		OUTPUT_LOG1("check host server:%s\n", hostServer.c_str());
		_downloader->downloadAsync(updatePackUrl, _cacheManifestPath, MANIFEST_ID);
	}
	void AssetsManager::parseManifest()
	{
		_manifestTxt = FileUtils::getStringFromFile(_cacheManifestPath);
		if (_manifestTxt.empty())
		{
			return;
		}
		std::string hostServer = _config.patchHostList[validHostIndex];
		OUTPUT_LOG1("valid host server:%s\n", hostServer.c_str());
		std::istringstream iss(_manifestTxt);
		while (!iss.eof())
		{
			std::string sTaskString;
			std::getline(iss, sTaskString);
			if (!sTaskString.empty())
			{
				std::vector<std::string> arr;
				std::string delim = ",";
				AssetsManagerGlobal::split(sTaskString, delim, &arr);
				if (arr.size() > 2)
				{
					std::string filename = arr[0];
					std::string md5 = arr[1];
					std::string size = arr[2];
					double i = atof(size.c_str());
					_totalSize += i;
					std::string download_path = AssetsManagerGlobal::format("%s,%s,%s.p", filename.c_str(), md5.c_str(), size.c_str());
					Downloader::DownloadUnit unit;
					unit.customId = filename;
					unit.srcUrl = AssetsManagerGlobal::format("%s/coredownload/update/%s",hostServer.c_str(),download_path.c_str());
					std::string storagePath = _asstesCachesPath + filename;
					std::string folder;
					size_t found = storagePath.find_last_of("/\\");
					if (std::string::npos != found)
					{
						folder = storagePath.substr(0, found);
					}
					else
					{
						folder = storagePath;
					}
					FileUtils::createDirectory(folder);
					unit.storagePath = storagePath;
					unit.resumeDownload = true;
					_downloadUnits.emplace(filename, unit);
				}
			}
		}
	}
	void AssetsManager::onError(const Downloader::Error &error)
	{
		// Skip version error occured
		if (error.customId == VERSION_ID)
		{
			callWorkingCallback(State::VERSION_ERROR);
		}
		else if (error.customId == MANIFEST_ID)
		{
			if (validHostIndex >= (_config.patchHostList.size() - 1))
			{
				callWorkingCallback(State::MANIFEST_ERROR);
			}
			else
			{
				validHostIndex++;
				downloadManifest(_comparedVersion, validHostIndex);
			}
		}
		else
		{
			auto unitIt = _downloadUnits.find(error.customId);
			// Found unit and add it to failed units
			if (unitIt != _downloadUnits.end())
			{
				Downloader::DownloadUnit unit = unitIt->second;
				_failedDownloadUnits.emplace(unit.customId, unit);
			}
		}
		
	}
	void AssetsManager::onProgress(double total, double downloaded, const std::string &url, const std::string &customId)
	{
		if (customId == VERSION_ID )
		{
			callWorkingCallback(State::DOWNLOADING_VERSION);
		}
		else if (customId == MANIFEST_ID)
		{
		}
		else if (customId == BATCH_UPDATE_ID)
		{
		}
		else
		{
			auto unitIt = _downloadUnits.find(customId);
			std::string url;
			if (unitIt != _downloadUnits.end())
			{
				Downloader::DownloadUnit unit = unitIt->second;
				url = unit.srcUrl;
			}
			
			_downloadedSize.emplace(customId, downloaded);
			double totalDownloaded = 0;
			for (auto it = _downloadedSize.begin(); it != _downloadedSize.end(); ++it)
			{
				if (it->first == customId)
				{
					it->second = downloaded;
				}
				totalDownloaded += it->second;
			}
			if (_totalSize > 0)
			{
				_percent = totalDownloaded / _totalSize;
			}
			if (OnProgressCallback)
			{
				OnProgressCallback(url, downloaded, total, _percent, totalDownloaded, _totalSize);
			}
			callWorkingCallback(State::DOWNLOADING_ASSETS);
		}
	}

	void AssetsManager::onSuccess(const std::string &srcUrl, const std::string &storagePath, const std::string &customId)
	{
		if (customId == VERSION_ID)
		{
			parseVersion();
			_comparedVersion = compareVersions();
			_asstesCachesPath = AssetsManagerGlobal::format("%s%s/", _config.storagePath.c_str(), _lastVersion.c_str());
			FileUtils::createDirectory(_asstesCachesPath);
			callWorkingCallback(State::VERSION_CHECKED);
		}
		else if (customId == MANIFEST_ID)
		{
			parseManifest();
			callWorkingCallback(State::PREDOWNLOAD_ASSETS);
			_downloader->batchDownloadAsync(_downloadUnits, BATCH_UPDATE_ID);
		}
		else if (customId == BATCH_UPDATE_ID)
		{
			if (_failedDownloadUnits.size()> 0)
			{
				_downloadUnits.clear();
				_downloadUnits = _failedDownloadUnits;
				_downloader->batchDownloadAsync(_downloadUnits, BATCH_UPDATE_FAILED_ID);
				return;
			}
			else
			{
				callWorkingCallback(State::ASSETS_DOWNLOADED);
			}
		}
		else if (customId == BATCH_UPDATE_FAILED_ID)
		{
			if (_failedDownloadUnits.size() > 0)
			{
				OUTPUT_LOG1("failedDownloadUnits:%d\n", _failedDownloadUnits.size());
				for (auto it = _failedDownloadUnits.cbegin(); it != _failedDownloadUnits.cend(); ++it)
				{
					Downloader::DownloadUnit unit = it->second;
					std::string srcUrl = unit.srcUrl;
					std::string storagePath = unit.storagePath;
					OUTPUT_LOG1("failed to download:%s\n", srcUrl.c_str());
				}
				callWorkingCallback(State::FAIL_TO_ASSETS_DOWNLOAD);
				return;
			}
			else
			{
				callWorkingCallback(State::ASSETS_DOWNLOADED);
			}
		}
		else
		{
			auto unitIt = _failedDownloadUnits.find(customId);
			// Found unit and delete it
			if (unitIt != _failedDownloadUnits.end())
			{
				// Remove from failed units list
				_failedDownloadUnits.erase(unitIt);
			}
		}
	}
	const std::string AssetsManager::getPatchListUrl(bool is_full_list, int nCandidate) const
	{
		int len = _config.patchHostList.size();
		nCandidate = min(nCandidate, len - 1);
		nCandidate = max(nCandidate, 0);
		std::string hostServer = _config.patchHostList[nCandidate];
		if (is_full_list)
		{
			//such as "http://update.paraengine.com/download/list/full.p";
			return hostServer + "coredownload/list/full" + FILE_LIST_FILE_EXT;
		}
		else
		{
			//such as "http://update.paraengine.com/download/list/patch_1.2.3.p";
			return hostServer + "coredownload/" + _lastVersion + "/list/patch_" + _curVersion + FILE_LIST_FILE_EXT;
		}
	}
	bool AssetsManager::checkMD5(const Downloader::DownloadUnit& unit)
	{
		std::string srcUrl = unit.srcUrl;
		std::string storagePath = unit.storagePath;
		std::vector<std::string> arr;
		std::string delim = ",";
		AssetsManagerGlobal::split(srcUrl, delim, &arr);

		std::string md5_str = arr[1];
		std::string size_str = arr[2];
		int size = atoi(size_str.c_str());
		char* outBuffer = NULL;
		int outSize;

		FileUtils::getDataFromFile(storagePath, &outBuffer, outSize);
		if (size != outSize)
		{
			return false;
		}
		std::string sMD5String = FileUtils::getMD5((const unsigned char*)outBuffer, outSize);
		delete outBuffer;
		return (stricmp(md5_str.c_str(), sMD5String.c_str()) == 0);
	}

	bool AssetsManager::decompress(std::string sourceFileName, std::string destFileName)
	{
		FILE* outfile = fopen(destFileName.c_str(), "wb");
		if (!outfile)
		{
			return false;
		}
		gzFile sSourceFile;
		char in[CHUNK + 1];
		int uncomprLen;

		sSourceFile = gzopen(sourceFileName.c_str(), "rb");
		if (!sSourceFile)
		{
			return false;
		}
		while ((uncomprLen = gzread(sSourceFile, in, CHUNK)) > 0)
		{
			if (uncomprLen < CHUNK)
			{
				memset(in + uncomprLen, '\0', CHUNK - uncomprLen);
				fwrite(in, sizeof(char), uncomprLen, outfile);
				break;
			}
			in[CHUNK] = '\0';
			fwrite(in,sizeof(char),uncomprLen,outfile);
		}
		fclose(outfile);
		gzclose(sSourceFile);
		return true;
	}
	void AssetsManager::download()
	{
		if (!_needUpdate)
		{
			return;
		}
		downloadManifest(_comparedVersion, validHostIndex);

	}
	void AssetsManager::apply()
	{
		callWorkingCallback(State::PREUPDATE);
		int i = 0;
		int len = _downloadUnits.size()-1;
		for (auto it = _downloadUnits.cbegin(); it != _downloadUnits.cend(); ++it)
		{
			Downloader::DownloadUnit unit = it->second;
			std::string storagePath = unit.storagePath;

			unsigned long found = storagePath.find_last_of(".");
			if (found != std::string::npos)
			{
				std::string name = storagePath.substr(0, found);
				std::string app_dest_folder = name.substr(_asstesCachesPath.length());
				std::string abs_app_dest_folder = _config.destStoragePath + app_dest_folder;
				if (!checkMD5(unit))
				{
					OUTPUT_LOG1("failed to compare md5 file: %s \n", storagePath.c_str());
					_failedMD5Units.emplace(unit.customId, unit);
					FileUtils::removeFile(storagePath);
				}
				else
				{
					if (!decompress(storagePath, name))
					{
						OUTPUT_LOG1("failed to uncompress file: %s \n", storagePath.c_str());
						_failedUncompressUnits.emplace(unit.customId, unit);
						FileUtils::removeFile(storagePath);
					}
					else
					{
						if (!FileUtils::removeFile(storagePath))
						{
							OUTPUT_LOG1("failed to delete file: %s \n", storagePath.c_str());
						}
						if (!FileUtils::moveFile(name, abs_app_dest_folder))
						{
							OUTPUT_LOG1("failed to move file: %s -> %s\n", name.c_str(), abs_app_dest_folder.c_str());
						}
						if (OnMovingFileCallback)
						{
							OnMovingFileCallback(app_dest_folder, i, len);
						}
					}
				}
				
			}
			i++;
		}
		deleteOldFiles();
		_needUpdate = false;
		_hasVersionFile = true;
		if (_failedMD5Units.size() > 0 || _failedUncompressUnits.size() > 0)
		{
			callWorkingCallback(State::FAIL_TO_UPDATED);
			FileUtils::removeFile(_config.localVersionTxt);
		}
		else
		{
			FileUtils::removeDirectory(_config.storagePath);
			callWorkingCallback(State::UPDATED);
		}
	}
	void AssetsManager::applyAsync()
	{
		auto t = std::thread(&AssetsManager::apply, this);
		t.detach();
	}
	void AssetsManager::callWorkingCallback(State state)
	{
		if (OnWorkingCallback)
		{
			OnWorkingCallback(state);
		}
		
	}
	void AssetsManager::deleteOldFiles()
	{
		std::string delete_file_path = AssetsManagerGlobal::format("%sdeletefile.list", _config.writeablePath.c_str());
		OUTPUT_LOG1("beginning delete old files from:%s\n", delete_file_path.c_str());
		if (FileUtils::isFileExist(delete_file_path))
		{
			std::string data = FileUtils::getStringFromFile(delete_file_path);
			std::vector<std::string> arr;
			std::string delim = ",";
			AssetsManagerGlobal::split(data, delim, &arr);
			int len = arr.size();
			for (int i = 0; i < len; i++)
			{
				std::string name = arr[i];
				name = AssetsManagerGlobal::trim(name.c_str());
				std::string full_path = AssetsManagerGlobal::format("%s%s", _config.writeablePath.c_str(), name.c_str());
				if (FileUtils::isFileExist(full_path))
				{
					FileUtils::removeFile(full_path);
					OUTPUT_LOG1("try to delete old files:%s\n", full_path.c_str());
				}
			}
		}
		FileUtils::removeFile(delete_file_path);
		OUTPUT_LOG1("ending delete old files:%s\n", delete_file_path.c_str());
	}
}
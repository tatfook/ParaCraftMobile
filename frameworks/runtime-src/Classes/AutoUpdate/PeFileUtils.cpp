#include "ParaEngine.h"
#ifdef AUTOUPDATE_PARAENGINE
#include "FileUtils.h"
#include "CCFileUtils.h"
#include "io/FileUtils.h"
#include "MD5.h"
#include <string>

namespace AutoUpdate
{
	bool FileUtils::createDirectory(const std::string& dirPath)
	{
		return cocos2d::FileUtils::getInstance()->createDirectory(dirPath);
	}
	bool FileUtils::removeDirectory(const std::string& dirPath)
	{
		return cocos2d::FileUtils::getInstance()->removeDirectory(dirPath);

	}
	std::string FileUtils::getStringFromFile(const std::string& filename)
	{
		return cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
	}
	bool FileUtils::isFileExist(const std::string& filename)
	{
		return cocos2d::FileUtils::getInstance()->isFileExist(filename);
	}
	bool FileUtils::removeFile(const std::string& filepath)
	{
		return cocos2d::FileUtils::getInstance()->removeFile(filepath);

	}
	bool FileUtils::renameFile(const std::string& path, const std::string& oldname, const std::string& name)
	{
		return cocos2d::FileUtils::getInstance()->renameFile(path,oldname,name);

	}
	bool FileUtils::moveFile(const std::string& sourceFileName, const std::string& destFileName)
	{
		if (!sourceFileName.empty() && !destFileName.empty())
		{
			std::string folder;
			size_t found = destFileName.find_last_of("/\\");
			if (std::string::npos != found)
			{
				folder = destFileName.substr(0, found);
			}
			FileUtils::createDirectory(folder);

			int errorCode = rename(sourceFileName.c_str(), destFileName.c_str());

			if (0 == errorCode)
			{
				return true;
			}

		}
		return false;
	}
	long FileUtils::getFileSize(const std::string &filepath)
	{
		return cocos2d::FileUtils::getInstance()->getFileSize(filepath);

	}
	std::string FileUtils::getWritablePath()
	{
		return ParaEngine::CFileUtils::GetWritablePath();
	}
	void FileUtils::getDataFromFile(const std::string& filename, char** outBuffer, int& outSize)
	{
		ParaEngine::FileData data = ParaEngine::CFileUtils::GetDataFromFile(filename.c_str());
		outSize = data.GetSize();

		char* m_buffer = new char[outSize + 1];
		memcpy(m_buffer, data.GetBytes(), outSize);
		m_buffer[outSize] = '\0';
		*outBuffer = m_buffer;
	}
	std::string FileUtils::getMD5(const unsigned char* buff, int size)
	{
		ParaEngine::MD5 md5_hash;
		md5_hash.feed(buff, size);
		return md5_hash.hex();
	}
}
#endif

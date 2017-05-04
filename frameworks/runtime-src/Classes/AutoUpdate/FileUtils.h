#ifndef FileUtil_H
#define FileUtil_H
#include <string>
namespace AutoUpdate
{
	namespace FileUtils
	{
		bool createDirectory(const std::string& dirPath);
		bool removeDirectory(const std::string& dirPath);
		std::string getStringFromFile(const std::string& filename);
		bool isFileExist(const std::string& filename);
		bool removeFile(const std::string& filepath);
		bool renameFile(const std::string& path, const std::string& oldname, const std::string& name);
		bool moveFile(const std::string& sourceFileName, const std::string& destFileName);
		long getFileSize(const std::string &filepath);
		std::string getWritablePath();
		void getDataFromFile(const std::string& filename, char** outBuffer, int& outSize);
		std::string getMD5(const unsigned char* buff,int size);
	};
}
#endif
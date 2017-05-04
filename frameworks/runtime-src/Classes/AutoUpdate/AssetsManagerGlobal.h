#ifndef AssetsManagerGlobal_H
#define AssetsManagerGlobal_H
#include <vector>
#include <mutex>
#include <functional>
namespace AutoUpdate
{
	namespace AssetsManagerGlobal
	{
		void refresh();
		void performFunctionInCocosThread(const std::function<void()> &function);
		void split(std::string& s, std::string& delim, std::vector< std::string >* ret);
		std::string format(const char* format, ...);
		std::string trim(const char* str);
	}
}
#endif
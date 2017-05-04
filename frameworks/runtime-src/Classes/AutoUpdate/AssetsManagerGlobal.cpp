#include "ParaEngine.h"
#include "AssetsManagerGlobal.h"
#include <stdarg.h>
#ifdef AUTOUPDATE_COCOS2DX
#include "cocos2d.h"
using namespace cocos2d;
#endif 

namespace AutoUpdate
{
	namespace AssetsManagerGlobal
	{
		std::vector<std::function<void()>> _functionsToPerform;
		std::mutex _performMutex;
		void refresh()
		{
			if (!_functionsToPerform.empty()) {
				_performMutex.lock();
				// fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
				auto temp = _functionsToPerform;
				_functionsToPerform.clear();
				_performMutex.unlock();
				for (const auto &function : temp) {
					function();
				}
			}
		}
		void performFunctionInCocosThread(const std::function<void()> &function)
		{
#ifdef AUTOUPDATE_COCOS2DX
			{
				Director *dir = Director::getInstance();
				Scheduler *sched = dir->getScheduler();
				sched->performFunctionInCocosThread(function);
			}
#else
			{
				_performMutex.lock();

				_functionsToPerform.push_back(function);

				_performMutex.unlock();
			}
#endif		
		}
		void split(std::string& s, std::string& delim, std::vector< std::string >* ret)
		{
			size_t last = 0;
			size_t index = s.find_first_of(delim, last);
			while (index != std::string::npos)
			{
				ret->push_back(s.substr(last, index - last));
				last = index + 1;
				index = s.find_first_of(delim, last);
			}
			if (index - last > 0)
			{
				ret->push_back(s.substr(last, index - last));
			}
		}
		std::string format(const char* format, ...)
		{
			#define CC_MAX_STRING_LENGTH (1024*100)

			std::string ret;

			va_list ap;
			va_start(ap, format);

			char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
			if (buf != nullptr)
			{
				vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
				ret = buf;
				free(buf);
			}
			va_end(ap);

			return ret;
		}
		std::string trim(const char* str)
		{
			const char *p = NULL, *start = NULL, *end = NULL;

			for (p = str; *p != '\0'; p++)
			{
				if (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n')
				{
					break;
				}
			}
			start = p;
			for (p = start + strlen(start) - 1; p >= start; p--)
			{
				if (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n')
				{
					break;
				}
			}
			end = p;
			return std::string(start, end - start + 1);
		}
	}
	
}
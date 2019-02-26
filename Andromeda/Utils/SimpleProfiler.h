#ifndef SimpleProfiler_H
#define SimpleProfiler_H

#include <Andromeda/System/Timer.h>

#include <string>
#include <map>

namespace Andromeda
{
	namespace Utils
	{

		class SimpleProfiler
		{
		private:

			static SimpleProfiler* _logger;

			std::map<std::string, float> _map;
			System::Timer* _timer;
			std::string _currentSection;

			SimpleProfiler();

		public:

			static SimpleProfiler* Instance();

			//log message 
			void BeginSection(std::string name);
			void EndSection();

			float GetSectionTime(std::string name);
		};

	}
}

#endif


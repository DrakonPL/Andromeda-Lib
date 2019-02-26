

#include <Andromeda/Utils/SimpleProfiler.h>

namespace Andromeda
{
	namespace Utils
	{
		SimpleProfiler* SimpleProfiler::_logger = 0;

		SimpleProfiler::SimpleProfiler()
		{
			_timer = new System::Timer();
		}

		SimpleProfiler* SimpleProfiler::Instance()
		{
			if (_logger == 0)
			{
				_logger = new SimpleProfiler();
			}

			return _logger;
		}

		//log message 
		void SimpleProfiler::BeginSection(std::string name)
		{
			_currentSection = name;
			_timer->GetDelta();
		}

		void SimpleProfiler::EndSection()
		{
			if (_map.find(_currentSection) == _map.end())
			{
				_map.insert(std::pair<std::string, float>(_currentSection, _timer->GetDelta()));
			}
			else
			{
				_map[_currentSection] = _timer->GetDelta();
			}
		}

		float SimpleProfiler::GetSectionTime(std::string name)
		{
			if (_map.find(name) == _map.end())
			{
				return 0.0f;
			}

			return _map[name];
		}
	}
}
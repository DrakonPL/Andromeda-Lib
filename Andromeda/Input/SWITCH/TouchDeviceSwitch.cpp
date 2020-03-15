#include <Andromeda/Input/SWITCH/TouchDeviceSwitch.h>

#include <switch.h>

namespace Andromeda
{
	namespace Input
	{
		TouchDeviceSwitch::TouchDeviceSwitch()
		{
			_touchCount = 0;
		}

		int TouchDeviceSwitch::GetTouchCount()
		{
			return hidTouchCount();
		}

		int TouchDeviceSwitch::GetTouchX(int touchNumber)
		{
			touchPosition touch;
			
			//Read the touch screen coordinates
            hidTouchRead(&touch, touchNumber);
			
			
			return touch.px;
		}

		int TouchDeviceSwitch::GetTouchY(int touchNumber)
		{
			touchPosition touch;
			
			//Read the touch screen coordinates
            hidTouchRead(&touch, touchNumber);
			
			
			return touch.py;
		}

	}
}

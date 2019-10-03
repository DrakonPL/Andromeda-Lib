#ifndef TouchDeviceSwitch_H
#define TouchDeviceSwitch_H

#include <Andromeda/Input/TouchDevice.h>

namespace Andromeda
{
	namespace Input
	{
		class InputManagerSwitch;

		class TouchDeviceSwitch : public TouchDevice
		{
			TouchDeviceSwitch();

		public:

			int GetTouchCount();

			int GetTouchX(int touch);
			int GetTouchY(int touch);

			friend class InputManagerSwitch;
		};
	}
}

#endif

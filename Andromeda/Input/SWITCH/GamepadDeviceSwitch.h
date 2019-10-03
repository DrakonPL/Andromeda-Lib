#ifndef GamepadDeviceSwitch_H
#define GamepadDeviceSwitch_H

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/GamepadDevice.h>


namespace Andromeda
{
	namespace Input
	{
		class InputManagerSwitch;

		class GamepadDeviceSwitch : public GamepadDevice
		{
		
			GamepadDeviceSwitch();

		public:

			int LeftAnalogX();
			int LeftAnalogY();

			int RightAnalogX();
			int RightAnalogY();

			bool KeyDown(Gamepad::Button button);
			bool KeyUp(Gamepad::Button button);

			friend class InputManagerSwitch;

		};
	}
}

#endif

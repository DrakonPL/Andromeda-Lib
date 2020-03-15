#ifndef GamepadDeviceSwitch_H
#define GamepadDeviceSwitch_H

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/GamepadDevice.h>

#include <switch.h>

namespace Andromeda
{
	namespace Input
	{
		class InputManagerSwitch;

		class GamepadDeviceSwitch : public GamepadDevice
		{
		
			GamepadDeviceSwitch();
			
			private:
			
			u64 _kDown;
			u64 _kUp;

		public:

			int LeftAnalogX();
			int LeftAnalogY();

			int RightAnalogX();
			int RightAnalogY();

			bool KeyDown(Gamepad::Button button);
			bool KeyUp(Gamepad::Button button);
			
			void UpdateKesy(u64 kDown,u64 kUp);

			friend class InputManagerSwitch;

		};
	}
}

#endif

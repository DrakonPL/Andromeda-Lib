#ifndef GamepadDevice_H
#define GamepadDevice_H

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/InputDevice.h>

namespace Andromeda
{
	namespace Input
	{
		class InputManager;

		class GamepadDevice : public InputDevice
		{
		protected:

			GamepadDevice(){}

		public:

			virtual float LeftAnalogX() = 0;
			virtual float LeftAnalogY() = 0;

			virtual float RightAnalogX() = 0;
			virtual float RightAnalogY() = 0;

			virtual bool KeyDown(Gamepad::Button button) = 0;
			virtual bool KeyUp(Gamepad::Button button) = 0;

			friend class InputManager;

		};
	}
}

#endif

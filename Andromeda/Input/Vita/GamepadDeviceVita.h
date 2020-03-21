#ifndef GamepadDeviceVita_H
#define GamepadDeviceVita_H

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/GamepadDevice.h>
#include <psp2/ctrl.h>

namespace Andromeda
{
	namespace Input
	{
		class InputManagerVita;

		class GamepadDeviceVita : public GamepadDevice
		{
			SceCtrlData* _pad;

			GamepadDeviceVita(SceCtrlData* pad);

		public:

			float LeftAnalogX();
			float LeftAnalogY();

			float RightAnalogX();
			float RightAnalogY();

			bool KeyDown(Gamepad::Button button);
			bool KeyUp(Gamepad::Button button);

			friend class InputManagerVita;

		};
	}
}

#endif

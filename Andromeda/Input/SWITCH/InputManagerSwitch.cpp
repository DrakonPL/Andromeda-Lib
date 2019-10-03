#include <Andromeda/Input/SWITCH/InputManagerSwitch.h>
#include <Andromeda/Input/SWITCH/GamepadDeviceSwitch.h>
#include <Andromeda/Input/SWITCH/TouchDeviceSwitch.h>

#include <switch.h>

namespace Andromeda
{
	namespace Input
	{
		int InputManagerSwitch::GetKayboardCount()
		{
			return 0;
		}

		int InputManagerSwitch::GetMouseCount()
		{
			return 0;
		}

		int InputManagerSwitch::GetGamepadCount()
		{
			return 1;
		}

		int InputManagerSwitch::GetTouchDeviceCount()
		{
			return 1;
		}

		void InputManagerSwitch::Update()
		{
			
		}

		KeyboardDevice* InputManagerSwitch::GetKeyboard(int deviceNumber)
		{
			return 0;
		}

		MouseDevice* InputManagerSwitch::GetMouse(int deviceNumber)
		{
			return 0;
		}

		GamepadDevice* InputManagerSwitch::GetGamepad(int deviceNumber)
		{
			return new GamepadDeviceSwitch();
		}

		TouchDevice* InputManagerSwitch::GetTouch(int deviceNumber)
		{
			return new TouchDeviceSwitch();
		}
	}
}

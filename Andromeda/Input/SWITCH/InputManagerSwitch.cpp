#include <Andromeda/Input/SWITCH/InputManagerSwitch.h>
#include <Andromeda/Input/SWITCH/GamepadDeviceSwitch.h>
#include <Andromeda/Input/SWITCH/TouchDeviceSwitch.h>

#include <switch.h>

namespace Andromeda
{
	namespace Input
	{
		InputManagerSwitch::InputManagerSwitch()
		{
			_gamepad = new GamepadDeviceSwitch(); 
		}
		
		
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
			//Scan all the inputs. This should be done once for each frame
			hidScanInput();
			
			//update data for pad 1
			//u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			//u64 kUp = hidKeysUp(CONTROLLER_P1_AUTO);
			
			u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
			
			//update Ggamepad device
			_gamepad->UpdateKesy(kHeld,kHeld);
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
			return _gamepad;
		}

		TouchDevice* InputManagerSwitch::GetTouch(int deviceNumber)
		{
			return new TouchDeviceSwitch();
		}
	}
}

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/SWITCH/GamepadDeviceSwitch.h>

#include <switch.h>

namespace Andromeda
{
	namespace Input
	{
		GamepadDeviceSwitch::GamepadDeviceSwitch()
		{
			
		}

		int GamepadDeviceSwitch::LeftAnalogX()
		{
			//JoystickPosition pos_left, pos_right;
			
			//Read the joysticks' position
			//hidJoystickRead(&pos_left, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);

		
			//pos_left.dx, pos_left.dy
			return 0;
		}
		int GamepadDeviceSwitch::LeftAnalogY()
		{
			return 0;
		}

		int GamepadDeviceSwitch::RightAnalogX()
		{
			//pos_right.dx, pos_right.dy
			return 0;
		}
		int GamepadDeviceSwitch::RightAnalogY()
		{
			return 0;
		}

		bool GamepadDeviceSwitch::KeyDown(Gamepad::Button button)
		{
			//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
			//u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
			
			//check states			
			switch(button)
			{
			case Gamepad::Left: 
				return (kDown & KEY_DLEFT);
				break;
			case Gamepad::Right: 
				return (kDown & KEY_DRIGHT);
				break;
			case Gamepad::Up: 
				return (kDown & KEY_DUP);
				break;
			case Gamepad::Down: 
				return (kDown & KEY_DDOWN);
				break;
			case Gamepad::Triangle: 
				return (kDown & KEY_X);
				break;
			case Gamepad::Square: 
				return (kDown & KEY_Y);
				break;
			case Gamepad::Circle: 
				return (kDown & KEY_A);
				break;
			case Gamepad::Cross: 
				return (kDown & KEY_B);
				break;
			case Gamepad::LTrigger:
				return (kDown & KEY_L);
				break;
			case Gamepad::RTrigger:
				return (kDown & KEY_R);
				break;
			case Gamepad::Start:
				return (kDown & KEY_MINUS);
				break;
			case Gamepad::Select:
				return (kDown & KEY_PLUS);
				break;
			}

			return false;
		}

		bool GamepadDeviceSwitch::KeyUp(Gamepad::Button button)
		{
			//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
			u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
			
			//check states			

			switch(button)
			{
			case Gamepad::Left: 
				return !(kDown & KEY_DLEFT);
				break;
			case Gamepad::Right: 
				return !(kDown & KEY_DRIGHT);
				break;
			case Gamepad::Up: 
				return !(kDown & KEY_DUP);
				break;
			case Gamepad::Down: 
				return !(kDown & KEY_DDOWN);
				break;
			case Gamepad::Triangle: 
				return !(kDown & KEY_X);
				break;
			case Gamepad::Square: 
				return !(kDown & KEY_Y);
				break;
			case Gamepad::Circle: 
				return !(kDown & KEY_A);
				break;
			case Gamepad::Cross: 
				return !(kDown & KEY_B);
				break;
			case Gamepad::LTrigger:
				return !(kDown & KEY_L);
				break;
			case Gamepad::RTrigger:
				return !(kDown & KEY_R);
				break;
			case Gamepad::Start:
				return !(kDown & KEY_MINUS);
				break;
			case Gamepad::Select:
				return !(kDown & KEY_PLUS);
				break;
			}

			return false;
		}

	}
}


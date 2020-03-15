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
		
		void GamepadDeviceSwitch::UpdateKesy(u64 kDown,u64 kUp)
		{
			_kDown = kDown;
			_kUp = kUp;
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
			//_kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			//u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
			
			//check states			
			switch(button)
			{
			case Gamepad::Left: 
				return (_kDown & KEY_DLEFT);
				break;
			case Gamepad::Right: 
				return (_kDown & KEY_DRIGHT);
				break;
			case Gamepad::Up: 
				return (_kDown & KEY_DUP);
				break;
			case Gamepad::Down: 
				return (_kDown & KEY_DDOWN);
				break;
			case Gamepad::Triangle: 
				return (_kDown & KEY_X);
				break;
			case Gamepad::Square: 
				return (_kDown & KEY_Y);
				break;
			case Gamepad::Circle: 
				return (_kDown & KEY_A);
				break;
			case Gamepad::Cross: 
				return (_kDown & KEY_B);
				break;
			case Gamepad::LTrigger:
				return (_kDown & KEY_L);
				break;
			case Gamepad::RTrigger:
				return (_kDown & KEY_R);
				break;
			case Gamepad::Select:
				return (_kDown & KEY_MINUS);
				break;
			case Gamepad::Start:
				return (_kDown & KEY_PLUS);
				break;

				break;
			}

			return false;
		}

		bool GamepadDeviceSwitch::KeyUp(Gamepad::Button button)
		{
			//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
			//u64 kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
			
			//check states			

			switch(button)
			{
			case Gamepad::Left: 
				return !(_kUp & KEY_DLEFT);
				break;
			case Gamepad::Right: 
				return !(_kUp & KEY_DRIGHT);
				break;
			case Gamepad::Up: 
				return !(_kUp & KEY_DUP);
				break;
			case Gamepad::Down: 
				return !(_kUp & KEY_DDOWN);
				break;
			case Gamepad::Triangle: 
				return !(_kUp & KEY_X);
				break;
			case Gamepad::Square: 
				return !(_kUp & KEY_Y);
				break;
			case Gamepad::Circle: 
				return !(_kUp & KEY_A);
				break;
			case Gamepad::Cross: 
				return !(_kUp & KEY_B);
				break;
			case Gamepad::LTrigger:
				return !(_kUp & KEY_L);
				break;
			case Gamepad::RTrigger:
				return !(_kUp & KEY_R);
				break;
			case Gamepad::Select:
				return !(_kUp & KEY_MINUS);
				break;
			case Gamepad::Start:
				return !(_kUp & KEY_PLUS);
				break;
			}

			return false;
		}

	}
}


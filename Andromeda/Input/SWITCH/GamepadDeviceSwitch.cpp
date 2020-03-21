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
		
		void GamepadDeviceSwitch::UpdateJoystick(JoystickPosition left,JoystickPosition right)
		{
			_leftJoy = left;
			_rightJoy = right;
		}

		float GamepadDeviceSwitch::LeftAnalogX()
		{
			return _leftJoy.dx / 32760.0f;
		}
		
		float GamepadDeviceSwitch::LeftAnalogY()
		{
			return _leftJoy.dy / 32760.0f;
		}

		float GamepadDeviceSwitch::RightAnalogX()
		{
			return _rightJoy.dx / 32760.0f;
		}
		
		float GamepadDeviceSwitch::RightAnalogY()
		{
			return _rightJoy.dy / 32760.0f;
		}

		bool GamepadDeviceSwitch::KeyDown(Gamepad::Button button)
		{
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
			}
			
			
			float toleranceLevel = 0.3f;

			//specials
			switch (button)
			{
			case Gamepad::LAnalogLeft:
				return LeftAnalogX() < toleranceLevel * -1.0f;
				break;
			case Gamepad::LAnalogRight:
				return LeftAnalogX() > toleranceLevel;
				break;
			case Gamepad::LAnalogUp:
				return LeftAnalogY() > toleranceLevel;
				break;
			case Gamepad::LAnalogDown:
				return LeftAnalogY() < toleranceLevel * -1.0f;
				break;
			case Gamepad::LAnalogPushed:
				return (_kDown & KEY_LSTICK);
				break;
			}

			switch (button)
			{
			case Gamepad::RAnalogLeft:
				return RightAnalogX() < toleranceLevel * -1.0f;
				break;
			case Gamepad::RAnalogRight:
				return RightAnalogX() > toleranceLevel;
				break;
			case Gamepad::RAnalogUp:
				return RightAnalogY() > toleranceLevel;
				break;
			case Gamepad::RAnalogDown:
				return RightAnalogY() < toleranceLevel * -1.0f;
				break;
			case Gamepad::RAnalogPushed:
				return (_kDown & KEY_RSTICK);
				break;
			}

			return false;
		}

		bool GamepadDeviceSwitch::KeyUp(Gamepad::Button button)
		{
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
			
			float toleranceLevel = 0.3f;

			//specials
			switch (button)
			{
			case Gamepad::LAnalogLeft:
				return !(LeftAnalogX() < toleranceLevel * -1.0f);
				break;
			case Gamepad::LAnalogRight:
				return !(LeftAnalogX() > toleranceLevel);
				break;
			case Gamepad::LAnalogUp:
				return !(LeftAnalogY() > toleranceLevel);
				break;
			case Gamepad::LAnalogDown:
				return !(LeftAnalogY() < toleranceLevel * -1.0f);
				break;
			case Gamepad::LAnalogPushed:
				return !(_kDown & KEY_LSTICK);
				break;
			}

			switch (button)
			{
			case Gamepad::RAnalogLeft:
				return !(RightAnalogX() < toleranceLevel * -1.0f);
				break;
			case Gamepad::RAnalogRight:
				return !(RightAnalogX() > toleranceLevel);
				break;
			case Gamepad::RAnalogUp:
				return !(RightAnalogY() > toleranceLevel);
				break;
			case Gamepad::RAnalogDown:
				return !(RightAnalogY() < toleranceLevel * -1.0f);
				break;
			case Gamepad::RAnalogPushed:
				return !(_kDown & KEY_RSTICK);
				break;
			}

			return false;
		}

	}
}


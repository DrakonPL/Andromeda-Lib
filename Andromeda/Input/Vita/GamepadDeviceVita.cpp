#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/Vita/GamepadDeviceVita.h>
#include <psp2/ctrl.h>

namespace Andromeda
{
	namespace Input
	{
		GamepadDeviceVita::GamepadDeviceVita(SceCtrlData* pad)
		{
			_pad = pad;
		}

		float GamepadDeviceVita::LeftAnalogX()
		{
			return (_pad->lx - 128) / 128.0f;
		}
		float GamepadDeviceVita::LeftAnalogY()
		{
			return (_pad->ly - 128) / 128.0f;
		}

		float GamepadDeviceVita::RightAnalogX()
		{
			return (_pad->rx - 128) / 128.0f;
		}
		
		float GamepadDeviceVita::RightAnalogY()
		{
			return (_pad->ry - 128) / 128.0f;
		}

		bool GamepadDeviceVita::KeyDown(Gamepad::Button button)
		{
			switch(button)
			{
			case Gamepad::Left: 
				return _pad->buttons & SCE_CTRL_LEFT;
				break;
			case Gamepad::Right: 
				return _pad->buttons & SCE_CTRL_RIGHT;
				break;
			case Gamepad::Up: 
				return _pad->buttons & SCE_CTRL_UP;
				break;
			case Gamepad::Down: 
				return _pad->buttons & SCE_CTRL_DOWN;
				break;
			case Gamepad::Triangle: 
				return _pad->buttons & SCE_CTRL_TRIANGLE;
				break;
			case Gamepad::Square: 
				return _pad->buttons & SCE_CTRL_SQUARE;
				break;
			case Gamepad::Circle: 
				return _pad->buttons & SCE_CTRL_CIRCLE;
				break;
			case Gamepad::Cross: 
				return _pad->buttons & SCE_CTRL_CROSS;
				break;
			case Gamepad::LTrigger:
				return _pad->buttons & SCE_CTRL_LTRIGGER;
				break;
			case Gamepad::RTrigger:
				return _pad->buttons & SCE_CTRL_RTRIGGER;
				break;
			case Gamepad::Select:
				return _pad->buttons & SCE_CTRL_SELECT;
				break;
			case Gamepad::Start:
				return _pad->buttons & SCE_CTRL_START;
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
			}

			return false;
		}

		bool GamepadDeviceVita::KeyUp(Gamepad::Button button)
		{
			switch (button)
			{
			case Gamepad::Left:
				return !(_pad->buttons & SCE_CTRL_LEFT);
				break;
			case Gamepad::Right:
				return !(_pad->buttons & SCE_CTRL_RIGHT);
				break;
			case Gamepad::Up:
				return !(_pad->buttons & SCE_CTRL_UP);
				break;
			case Gamepad::Down:
				return !(_pad->buttons & SCE_CTRL_DOWN);
				break;
			case Gamepad::Triangle:
				return !(_pad->buttons & SCE_CTRL_TRIANGLE);
				break;
			case Gamepad::Square:
				return !(_pad->buttons & SCE_CTRL_SQUARE);
				break;
			case Gamepad::Circle:
				return !(_pad->buttons & SCE_CTRL_CIRCLE);
				break;
			case Gamepad::Cross:
				return !(_pad->buttons & SCE_CTRL_CROSS);
				break;
			case Gamepad::LTrigger:
				return !(_pad->buttons & SCE_CTRL_LTRIGGER);
				break;
			case Gamepad::RTrigger:
				return !(_pad->buttons & SCE_CTRL_RTRIGGER);
				break;
			case Gamepad::Select:
				return !(_pad->buttons & SCE_CTRL_SELECT);
				break;
			case Gamepad::Start:
				return !(_pad->buttons & SCE_CTRL_START);
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
			}

			return false;
		}

	}
}


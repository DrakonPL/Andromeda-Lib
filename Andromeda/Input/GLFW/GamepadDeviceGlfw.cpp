

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/GLFW/GamepadDeviceGlfw.h>
#include <vector>

namespace Andromeda
{
	namespace Input
	{
		GamepadDeviceGlfw::GamepadDeviceGlfw(GLFWwindow* window, int number)
		{
			_window = window;
			_padNumber = number;
		}

		float GamepadDeviceGlfw::LeftAnalogX()
		{
			int count;
			const float* axes = glfwGetJoystickAxes(_padNumber, &count);

			if (count > 0)
			{
				return axes[0];
			}

			return 0;
		}

		float GamepadDeviceGlfw::LeftAnalogY()
		{
			int count;
			const float* axes = glfwGetJoystickAxes(_padNumber, &count);

			if (count > 1)
			{
				return axes[1];
			}

			return 0;
		}

		float GamepadDeviceGlfw::RightAnalogX()
		{
			int count;
			const float* axes = glfwGetJoystickAxes(_padNumber, &count);

			if (count > 2)
			{
				return axes[2];
			}

			return 0;
		}

		float GamepadDeviceGlfw::RightAnalogY()
		{
			int count;
			const float* axes = glfwGetJoystickAxes(_padNumber, &count);

			if (count > 3)
			{
				return axes[3];
			}

			return 0;
		}

		void GamepadDeviceGlfw::UpdateButtons(const unsigned char* buttons)
		{
			_buttons = buttons;
		}

		bool GamepadDeviceGlfw::KeyDown(Gamepad::Button button)
		{
			//int count;
			//const unsigned char* buttons = glfwGetJoystickButtons(_padNumber, &count);


			//printf("0:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d 10:%d 11:%d 12:%d 13:%d\n", _buttons[0], _buttons[1], _buttons[2], _buttons[3], _buttons[4], _buttons[5], _buttons[6], _buttons[7], _buttons[8], _buttons[9], _buttons[10], _buttons[11], _buttons[12], _buttons[13]);

			switch (button)
			{
			case Gamepad::Left:
				return _buttons[13] == GLFW_PRESS;
				break;
			case Gamepad::Right:
				return _buttons[11] == GLFW_PRESS;
				break;
			case Gamepad::Up:
				return _buttons[10] == GLFW_PRESS;
				break;
			case Gamepad::Down:
				return _buttons[12] == GLFW_PRESS;
				break;
			case Gamepad::Triangle:
				return _buttons[3] == GLFW_PRESS;
				break;
			case Gamepad::Square:
				return _buttons[2] == GLFW_PRESS;
				break;
			case Gamepad::Circle:
				return _buttons[1] == GLFW_PRESS;
				break;
			case Gamepad::Cross:
				return _buttons[0] == GLFW_PRESS;
				break;
			case Gamepad::LTrigger:
				return _buttons[4] == GLFW_PRESS;
				break;
			case Gamepad::RTrigger:
				return _buttons[5] == GLFW_PRESS;
				break;
			case Gamepad::Select:
				return _buttons[6] == GLFW_PRESS;
				break;
			case Gamepad::Start:
				return _buttons[7] == GLFW_PRESS;
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
				return _buttons[8];
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
				return _buttons[9];
				break;
			}

			return false;
		}

		bool GamepadDeviceGlfw::KeyUp(Gamepad::Button button)
		{
			//int count;
			//const unsigned char* buttons = glfwGetJoystickButtons(_padNumber, &count);

			switch (button)
			{
			case Gamepad::Left:
				return !_buttons[13];
				break;
			case Gamepad::Right:
				return !_buttons[11];
				break;
			case Gamepad::Up:
				return !_buttons[10];
				break;
			case Gamepad::Down:
				return !_buttons[12];
				break;
			case Gamepad::Triangle:
				return !_buttons[3];
				break;
			case Gamepad::Square:
				return !_buttons[2];
				break;
			case Gamepad::Circle:
				return !_buttons[1];
				break;
			case Gamepad::Cross:
				return !_buttons[0];
				break;
			case Gamepad::LTrigger:
				return !_buttons[4];
				break;
			case Gamepad::RTrigger:
				return !_buttons[5];
				break;
			case Gamepad::Select:
				return !_buttons[6];
				break;
			case Gamepad::Start:
				return !_buttons[7];
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
				return !_buttons[8];
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
				return !_buttons[9];
				break;
			}

			return false;
		}

	}
}


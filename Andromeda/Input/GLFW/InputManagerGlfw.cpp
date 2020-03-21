

#include <Andromeda/Input/GLFW/InputManagerGlfw.h>
#include <Andromeda/Input/GLFW/KeyboardDeviceGlfw.h>
#include <Andromeda/Input/GLFW/MouseDeviceGlfw.h>
#include <Andromeda/Input/GLFW/GamepadDeviceGlfw.h>

namespace Andromeda
{
	namespace Input
	{
		void InputManagerGlfw::SetGlfwWindow(GLFWwindow* window)
		{
			_window = window;

			_padCount = 0;
			_gamepad = nullptr;
		}

		int InputManagerGlfw::GetKayboardCount()
		{
			return 1;
		}

		int InputManagerGlfw::GetMouseCount()
		{
			return 1;
		}

		int InputManagerGlfw::GetGamepadCount()
		{
			//iterate on each pad in system
			int foundPads = 0;

			for (size_t i = 0; i < GLFW_JOYSTICK_LAST; i++)
			{	
				int present = glfwJoystickPresent(i);

				if (present)
				{
					foundPads++;
				}
			}

			_padCount = foundPads;

			return foundPads;
		}

		int InputManagerGlfw::GetTouchDeviceCount()
		{
			return 0;
		}

		void InputManagerGlfw::Update()
		{
            if (_padCount > 0 && _gamepad != nullptr)
            {
				int count;
				const unsigned char* buttons = glfwGetJoystickButtons(0, &count);

				_gamepad->UpdateButtons(buttons);
            }

		}

		KeyboardDevice* InputManagerGlfw::GetKeyboard(int deviceNumber)
		{
			return new KeyboardDeviceGlfw(_window);
		}

		MouseDevice* InputManagerGlfw::GetMouse(int deviceNumber)
		{
			return new MouseDeviceGlfw(_window);
		}

		GamepadDevice* InputManagerGlfw::GetGamepad(int deviceNumber)
		{
            if (_gamepad == nullptr)
            {
				_gamepad = new GamepadDeviceGlfw(_window, deviceNumber);
            }

			return _gamepad;
		}

		TouchDevice* InputManagerGlfw::GetTouch(int deviceNumber)
		{
			return 0;
		}
	}
}

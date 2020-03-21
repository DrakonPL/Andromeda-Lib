#ifndef GamepadDeviceVita_H
#define GamepadDeviceVita_H

#include <Andromeda/Input/InputEnums.h>
#include <Andromeda/Input/GamepadDevice.h>
#include <GLFW/glfw3.h>

namespace Andromeda
{
	namespace Input
	{
		class InputManagerGlfw;

		class GamepadDeviceGlfw : public GamepadDevice
		{
			GLFWwindow* _window;

			int _padNumber;

			const unsigned char* _buttons;

			GamepadDeviceGlfw(GLFWwindow* window,int number);

			void UpdateButtons(const unsigned char* buttons);

		public:

			float LeftAnalogX();
			float LeftAnalogY();

			float RightAnalogX();
			float RightAnalogY();

			bool KeyDown(Gamepad::Button button);
			bool KeyUp(Gamepad::Button button);

			friend class InputManagerGlfw;

		};
	}
}

#endif

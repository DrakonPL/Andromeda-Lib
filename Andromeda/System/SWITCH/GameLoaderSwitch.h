#ifndef GameLoaderSwitch_H
#define GameLoaderSwitch_H

#include <Andromeda/System/GameLoader.h>
#include <Andromeda/Graphics/RenderManager.h>

#include <switch.h>

#include <EGL/egl.h>    // EGL library
#include <EGL/eglext.h> // EGL extensions
#include <glad/glad.h>  // glad library (OpenGL loader)


namespace Andromeda
{
	namespace System
	{
		class GameLoaderSwitch : public GameLoader
		{
		private:

			Graphics::RenderManager* _renderManager;
			
		public:

			GameLoaderSwitch(GameManager* gameManager);

			void Run();
		};
	}
}

#endif
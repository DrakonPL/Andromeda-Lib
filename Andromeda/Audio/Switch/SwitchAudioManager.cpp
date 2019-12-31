#include <Andromeda/Audio/Switch/SwitchAudioManager.h>
#include <Andromeda/Audio/Switch/SwitchSound.h>

#include <switch.h>

// Include sdl2 headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace Andromeda
{
	namespace Audio
	{
		bool SwitchAudioManager::Init()
		{
			// Start SDL with audio support
    		SDL_Init(SDL_INIT_AUDIO);


			// Load support for the MP3 format
			Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

			// open 44.1KHz, signed 16bit, system byte order,
			//  stereo audio, using 4096 byte chunks
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

			return true;
		}

		Sound* SwitchAudioManager::GetSound(std::string name)
		{
			SwitchSound* sound = new SwitchSound(name);
			//sound->SetEngine(_engine);

			return sound;
		}		
	}
}

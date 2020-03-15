#include <Andromeda/Audio/Sdl/SdlAudioManager.h>
#include <Andromeda/Audio/Sdl/SdlSound.h>

// Include sdl2 headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace Andromeda
{
	namespace Audio
	{
		bool SdlAudioManager::Init()
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

		SdlAudioManager::~SdlAudioManager()
		{
			Mix_CloseAudio();
			Mix_Quit();
			SDL_Quit();
		}

		Sound* SdlAudioManager::GetSound(std::string name)
		{
			SdlSound* sound = new SdlSound(name);
			//sound->SetEngine(_engine);

			return sound;
		}		
	}
}

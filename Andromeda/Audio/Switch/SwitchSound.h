#ifndef SwitchSound_H
#define SwitchSound_H

#include <Andromeda/Audio/Sound.h>
#include <string>

// Include the main libnx system header, for Switch development
#include <switch.h>

// Include sdl2 headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace Andromeda
{
	namespace Audio
	{
		class SwitchAudioManager;

		class SwitchSound : public Sound
		{
		private:
	
			Mix_Music* _audio;
			Mix_Chunk* _sound;
			
			int _audioType;

		public:

			SwitchSound(std::string name);
			~SwitchSound();

			bool IsPlaying();

			bool LoadWav(std::string filePath, bool streaming);
			bool LoadOgg(std::string filePath);

			void UpdateVolume();

			void Play();
			void Stop();
			void Pause();

			friend class SwitchAudioManager;
		};
	}
}

#endif

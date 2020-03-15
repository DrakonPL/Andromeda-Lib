#ifndef SdlSound_H
#define SdlSound_H

#include <Andromeda/Audio/Sound.h>
#include <string>

// Include sdl2 headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace Andromeda
{
	namespace Audio
	{
		class SdlAudioManager;

		class SdlSound : public Sound
		{
		private:
	
			Mix_Music* _audio;
			Mix_Chunk* _sound;
			
			int _audioType;
			int _soundChannel;

		public:

			SdlSound(std::string name);
			~SdlSound();

			bool IsPlaying();

			bool LoadWav(std::string filePath, bool streaming);
			bool LoadOgg(std::string filePath);

			void UpdateVolume();

			void Play();
			void Stop();
			void Pause();

			friend class SdlAudioManager;
		};
	}
}

#endif

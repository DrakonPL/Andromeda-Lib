#ifndef SdlAudioManager_H
#define SdlAudioManager_H

#include <Andromeda/Audio/AudioManager.h>
#include <string>

namespace Andromeda
{
	namespace Audio
	{
		class SdlAudioManager : public AudioManager
		{
		private:

			//ISoundEngine* _engine;

		public:

			~SdlAudioManager();

			bool Init();
			Sound* GetSound(std::string name);
		};
	}
}

#endif
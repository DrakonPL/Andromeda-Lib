#ifndef SwitchAudioManager_H
#define SwitchAudioManager_H

#include <Andromeda/Audio/AudioManager.h>
#include <string>

namespace Andromeda
{
	namespace Audio
	{
		class SwitchAudioManager : public AudioManager
		{
		private:

			//ISoundEngine* _engine;

		public:

			bool Init();
			Sound* GetSound(std::string name);
		};
	}
}

#endif
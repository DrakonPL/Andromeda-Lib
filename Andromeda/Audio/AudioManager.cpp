#include <Andromeda/Audio/AudioManager.h>

#ifdef ANDROMEDA_PC
//#include <Andromeda/Audio/IrrKlang/IrrAudioManager.h>
#endif

#ifdef ANDROMEDA_PC
#include <Andromeda/Audio/Sdl/SdlAudioManager.h>
#endif


#ifdef ANDROMEDA_VITA
#include <Andromeda/Audio/Vita/VitaAudioManager.h>
#endif

#ifdef ANDROMEDA_SWITCH
#include <Andromeda/Audio/Sdl/SdlAudioManager.h>
#endif


namespace Andromeda
{
	namespace Audio
	{
		AudioManager* AudioManager::_audioManager = 0;

		AudioManager::AudioManager()
		{
			
		}

		AudioManager* AudioManager::Instance()
		{
			if (_audioManager == 0)
			{
				#ifdef ANDROMEDA_PC
				{
					_audioManager = new SdlAudioManager();
				}
				#endif

				#ifdef ANDROMEDA_VITA
				{
					_audioManager = new VitaAudioManager();
				}
				#endif
				
				#ifdef ANDROMEDA_SWITCH
				{
					_audioManager = new SdlAudioManager();
				}
				#endif
				
				_audioManager->Init();
			}

			return _audioManager;
		}
	}
}
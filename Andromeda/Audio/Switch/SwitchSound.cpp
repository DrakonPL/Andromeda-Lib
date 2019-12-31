#include <Andromeda/Audio/Switch/SwitchSound.h>
#include <Andromeda/FileSystem/FileManager.h>
#include <string>

namespace Andromeda
{
	namespace Audio
	{
		SwitchSound::SwitchSound(std::string name) : Sound(name)
		{
			_audio = nullptr;
			_sound = nullptr;
			
			_audioType = -1;
		}

		SwitchSound::~SwitchSound()
		{

		}

		bool SwitchSound::IsPlaying()
		{
			return false;
		}

		bool SwitchSound::LoadWav(std::string filePath, bool streaming)
		{			
			return true;
		}

		bool SwitchSound::LoadOgg(std::string filePath)
		{
			return true;
		}

		void SwitchSound::UpdateVolume()
		{

		}

		void SwitchSound::Play()
		{
		
		}

		void SwitchSound::Stop()
		{
		
		}

		void SwitchSound::Pause()
		{

		}
	}
}

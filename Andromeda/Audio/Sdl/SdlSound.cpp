#include <Andromeda/Audio/Sdl/SdlSound.h>
#include <Andromeda/FileSystem/FileManager.h>
#include <string>

namespace Andromeda
{
	namespace Audio
	{
		SdlSound::SdlSound(std::string name) : Sound(name)
		{
			_audio = nullptr;
			_sound = nullptr;
			
			_audioType = -1;
			_soundChannel = 0;
		}

		SdlSound::~SdlSound()
		{
			if (_audioType == -1)
			{
				return;
			}

			if (_audioType == 0)
			{
				if (_sound != nullptr)
				{
					Mix_FreeChunk(_sound);
				}
			}
			else if (_audioType == 1)
			{
				if (_audio != nullptr)
				{
					Mix_FreeMusic(_audio);
				}
			}
		}

		bool SdlSound::IsPlaying()
		{
			if (_audioType == -1)
			{
				return false;
			}

			if (_audioType == 0)
			{
				if (_sound != nullptr)
				{
					return Mix_Playing(_soundChannel);
				}
			}
			else if (_audioType == 1)
			{
				if (_audio != nullptr)
				{
					return Mix_PlayingMusic();
				}
			}

			return false;
		}

		bool SdlSound::LoadWav(std::string filePath, bool streaming)
		{
			_audioType = 0;

			//set proper location
			std::string loadName = FileSystem::FileManager::Instance()->GetMainDirPath() + filePath;

			//load sound
			_sound = Mix_LoadWAV(loadName.c_str());

			//check if it's loaded
            if (_sound == nullptr)
            {
				return false;
            }

			return true;
		}

		bool SdlSound::LoadOgg(std::string filePath)
		{
			_audioType = 1;

			//set proper location
			std::string loadName = FileSystem::FileManager::Instance()->GetMainDirPath() + filePath;

			_audio = Mix_LoadMUS(loadName.c_str());

			//check if it's loaded
			if (_audio == nullptr)
			{
				return false;
			}

			return true;
		}

		void SdlSound::UpdateVolume()
		{
            if (_audioType == -1)
            {
				return;
            }

			if (_audioType == 0)
			{
				if (_sound != nullptr)
				{
					Mix_Volume(_soundChannel, _volume * (float)128);
				}
			}
			else if (_audioType == 1)
			{
				if (_audio != nullptr)
				{
					Mix_VolumeMusic(_volume * (float)128);
				}
			}
		}

		void SdlSound::Play()
		{
			if (_audioType == -1)
			{
				return;
			}

			if (_audioType == 0)
			{
                if (_sound != nullptr)
                {
					int looped = _looped ? -1 : 0;
					_soundChannel = Mix_PlayChannel(-1, _sound, looped);
					Mix_Volume(_soundChannel, _volume * (float)128);
                }
			}
			else if (_audioType == 1)
			{
                if (_audio != nullptr)
                {
					int looped = _looped ? -1 : 0;
					Mix_PlayMusic(_audio, _looped);
					Mix_VolumeMusic(_volume * (float)128);
                }
			}
		}

		void SdlSound::Stop()
		{
			if (_audioType == -1)
			{
				return;
			}

			if (_audioType == 0)
			{
				if (_sound != nullptr)
				{
					Mix_HaltChannel(_soundChannel);
				}
			}
			else if (_audioType == 1)
			{
				if (_audio != nullptr)
				{
					Mix_HaltMusic();
				}
			}
		}

		void SdlSound::Pause()
		{
			if (_audioType == -1)
			{
				return;
			}

			if (_audioType == 0)
			{
				if (_sound != nullptr)
				{
					
				}
			}
			else if (_audioType == 1)
			{
				if (_audio != nullptr)
				{
					
				}
			}
		}
	}
}

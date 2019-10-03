#include <Andromeda/System/SWITCH/MemoryManagerSwitch.h>

namespace Andromeda
{
	namespace System
	{
		void MemoryManagerSwitch::AllocTexture(Graphics::Texture* texture)
		{
			int textureSize = texture->GetColorType() == Graphics::TextureColorType::Texture_RGBA ? 4 : 3;

			unsigned char* data = new unsigned char[texture->GetWidth() * texture->GetHeight() * textureSize];
			texture->SetImageData(data);
		}

		void MemoryManagerSwitch::FreeTexture(Graphics::Texture* texture)
		{
			if (texture != nullptr)
			{
				if (texture->GetImageData() != nullptr)
				{
					delete[] texture->GetImageData();
					texture->SetImageData(nullptr);
				}
			}			
		}
	}
}



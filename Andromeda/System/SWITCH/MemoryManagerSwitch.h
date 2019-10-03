#ifndef MemoryManagerSwitch_H
#define MemoryManagerSwitch_H

#include <Andromeda/System/MemoryManager.h>

namespace Andromeda
{
	namespace System
	{
		class MemoryManagerSwitch : public MemoryManager
		{
		public:

			void AllocTexture(Graphics::Texture* texture);
			void FreeTexture(Graphics::Texture* texture);
		};
	}
}

#endif
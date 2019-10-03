#ifndef Timer_H
#define Timer_H

#include <ctime>

#ifdef ANDROMEDA_SWITCH
#include <sys/time.h>
#endif


namespace Andromeda
{
	namespace System
	{
		class Timer
		{
		private:

#ifdef ANDROMEDA_SWITCH
            timeval lastTime;
#else
            clock_t lastTime;
#endif

			



			float msec;

		public:

			Timer();

			float GetDelta();
		};
	}
}

#endif
#pragma once

#ifndef Math_H
#define Math_H

namespace Andromeda
{
	namespace Math
	{
		class Math
		{
		public:

			static float Sign(float f);

            static float Max(float a, float b);

			static float Clamp(float value, float min, float max);

			static float Repeatf(float t, float length);

			static float DeltaAngle(float current, float target);

			static float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

			static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

            // Moves a value /current/ towards /target/.
			static float MoveTowards(float current, float target, float maxDelta);

            // Same as ::ref::MoveTowards but makes sure the values interpolate correctly when they wrap around 360 degrees.
			static float MoveTowardsAngle(float current, float target, float maxDelta);
			

		};
	}
}

#endif
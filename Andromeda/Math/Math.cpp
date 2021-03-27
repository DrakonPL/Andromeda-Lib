#include <Andromeda/Math/Math.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


namespace Andromeda
{
	namespace Math
	{
		float Math::Max(float a, float b) { return a > b ? a : b; }

		float Math::Sign(float f) { return f >= 0.F ? 1.F : -1.F; }

		float  Math::Clamp(float value, float min, float max)
		{
			if (value < min)
				value = min;
			else if (value > max)
				value = max;
			return value;
		}

		float  Math::Repeatf(float t, float length)
		{
			return Clamp(t - std::floorf(t / length) * length, 0.0f, length);
		}

		float  Math::DeltaAngle(float current, float target)
		{
			float delta = Repeatf((target - current), 360.0f);
			if (delta > 180.0F)
				delta -= 360.0F;
			return delta;
		}

		float  Math::SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
		{
			// Based on Game Programming Gems 4 Chapter 1.10
			smoothTime = Max(0.0001F, smoothTime);
			float omega = 2.0f / smoothTime;

			float x = omega * deltaTime;
			float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
			float change = current - target;
			float originalTo = target;

			// Clamp maximum speed
			float maxChange = maxSpeed * smoothTime;
			change = Clamp(change, -maxChange, maxChange);
			target = current - change;

			float temp = (currentVelocity + omega * change) * deltaTime;
			currentVelocity = (currentVelocity - omega * temp) * exp;
			float output = target + (change + temp) * exp;

			// Prevent overshooting
			if (originalTo - current > 0.0F == output > originalTo)
			{
				output = originalTo;
				currentVelocity = (output - originalTo) / deltaTime;
			}

			return output;
		}

		float  Math::SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
		{
			target = current + DeltaAngle(current, target);
			return SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
		}

		// Moves a value /current/ towards /target/.
		float  Math::MoveTowards(float current, float target, float maxDelta)
		{
			if (std::abs(target - current) <= maxDelta)
				return target;
			return current + Sign(target - current) * maxDelta;
		}

		// Same as ::ref::MoveTowards but makes sure the values interpolate correctly when they wrap around 360 degrees.
		float  Math::MoveTowardsAngle(float current, float target, float maxDelta)
		{
			float deltaAngle = DeltaAngle(current, target);
			if (-maxDelta < deltaAngle && deltaAngle < maxDelta)
				return target;
			target = current + deltaAngle;
			return MoveTowards(current, target, maxDelta);
		}

	}
}

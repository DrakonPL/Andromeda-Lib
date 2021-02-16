#ifndef _H_CROWD_
#define _H_CROWD_

#include <vector>
#include "AnimVec3.h"
#include "AnimVec2.h"
#include "AnimQuat.h"
#include "Clip.h"

#define CROWD_MAX_ACTORS 20

struct Crowd {
protected:
	std::vector<AnimVec3> mPositions;
	std::vector<AnimQuat> mRotations;
	std::vector<AnimVec3> mScales;
	std::vector<AnimIvec2> mFrames;
	std::vector<float> mTimes;

	std::vector<float> mCurrentPlayTimes;
	std::vector<float> mNextPlayTimes;
protected:
	float AdjustTime(float time, float start, float end, bool looping);
	void UpdatePlaybackTimes(float deltaTime, bool looping, float start, float end);
	void UpdateFrameIndices(float start, float duration, unsigned int texWidth);
	void UpdateInterpolationTimes(float start, float duration, unsigned int texWidth);
public:
	unsigned int Size();
	void Resize(unsigned int size);
	
	Transform GetActor(unsigned int index);
	void SetActor(unsigned int index, const Transform& t);

	void Update(float deltaTime, Clip& mClip, unsigned int texWidth);

	void RandomizeTimes(Clip& clip);
	void RandomizePositions(std::vector<AnimVec3>& existing, const AnimVec3& min, const AnimVec3& max, float radius);
};

#endif
#ifndef _H_SKELETON_
#define _H_SKELETON_

#include <vector>
#include "Pose.h"
#include "AnimMat4.h"
#include <string>
#include "DualQuaternion.h"

class Skeleton {
protected:
	Pose mRestPose;
	Pose mBindPose;
	std::vector<AnimMat4> mInvBindPose;
	std::vector<std::string> mJointNames;
protected:
	void UpdateInverseBindPose();
public:
	Skeleton();
	Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	void Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	Pose& GetBindPose();
	Pose& GetRestPose();
	std::vector<AnimMat4>& GetInvBindPose();
	void GetInvBindPose(std::vector<DualQuaternion>& outInvBndPose);

	std::vector<std::string>& GetJointNames();
	int GetJointNumber(std::string &jointName);
	std::string& GetJointName(unsigned int index);
};

#endif
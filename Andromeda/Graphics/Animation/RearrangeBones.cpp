#include "RearrangeBones.h"
#include <iostream>
#include <list>

BoneMap RearrangeSkeleton(Skeleton& skeleton) {
	Pose& restPose = skeleton.GetRestPose();
	Pose& bindPose = skeleton.GetBindPose();
	unsigned int size = restPose.Size();
	if (size == 0) { return BoneMap(); }

	std::vector<std::vector<int>> hierarchy(size);
	std::list<int> process;
	for (unsigned int i = 0; i < size; ++i) {
		int parent = restPose.GetParent(i);
		if (parent >= 0) {
			hierarchy[parent].push_back((int)i);
		}
		else {
			process.push_back((int)i);
		}
	}

	BoneMap mapForward;
	BoneMap mapBackward;
	int index = 0;
	while (process.size() > 0) {
		int current = *process.begin();
		process.pop_front();
		std::vector<int>& children = hierarchy[current];

		unsigned int numChildren = (unsigned int)children.size();
		for (unsigned int i = 0; i < numChildren; ++i) {
			process.push_back(children[i]);
		}

		mapForward[index] = current;
		mapBackward[current] = index;
		index += 1;
	}

#if _DEBUG
	if (mapForward.size() != size || mapBackward.size() != mapForward.size()) {
		std::cout << "Error, sorted into wrong size\n";
	}
#endif
	mapForward[-1] = -1;
	mapBackward[-1] = -1;

	Pose newRestPose(size);
	Pose newBindPose(size);
	std::vector<std::string> newNames(size);
	for (unsigned int i = 0; i < size; ++i) {
		int thisBone = mapForward[i];
		newRestPose.SetLocalTransform(i, restPose.GetLocalTransform(thisBone));
		newBindPose.SetLocalTransform(i, bindPose.GetLocalTransform(thisBone));

		int parent = mapBackward[bindPose.GetParent(thisBone)];
		newRestPose.SetParent(i, parent);
		newBindPose.SetParent(i, parent);

		newNames[i] = skeleton.GetJointName(thisBone);
	}

	skeleton.Set(newRestPose, newBindPose, newNames);
	return mapBackward;
}

void RearrangeClip(Clip& clip, BoneMap& boneMap) {
	unsigned int size = clip.Size();

	for (unsigned int i = 0; i < size; ++i) {
		int joint = (int)clip.GetIdAtIndex(i);
		unsigned int newJoint = (unsigned int)boneMap[joint];
		clip.SetIdAtIndex(i, newJoint);
	}
}

void RearrangeFastclip(FastClip& clip, BoneMap& boneMap) {
	unsigned int size = clip.Size();

	for (unsigned int i = 0; i < size; ++i) {
		int joint = (int)clip.GetIdAtIndex(i);
		unsigned int newJoint = (unsigned int)boneMap[joint];
		clip.SetIdAtIndex(i, newJoint);
	}
}

void RearrangeMesh(Andromeda::Graphics::AnimatedMesh& mesh, BoneMap& boneMap)
{
	std::vector<glm::ivec4>& joints = mesh.GetJoints();
	unsigned int size = (unsigned int)joints.size();

	for (unsigned int i = 0; i < size; ++i) {
		joints[i].x = boneMap[joints[i].x];
		joints[i].y = boneMap[joints[i].y];
		joints[i].z = boneMap[joints[i].z];
		joints[i].w = boneMap[joints[i].w];
	}
}
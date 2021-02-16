#include "FABRIKSolver.h"
#include <iostream>

FABRIKSolver::FABRIKSolver() {
	mNumSteps = 15;
	mThreshold = 0.00001f;
}

unsigned int FABRIKSolver::Size() {
	return (unsigned int)mIKChain.size();
}

void FABRIKSolver::Resize(unsigned int newSize) {
	mIKChain.resize(newSize);
	mWorldChain.resize(newSize);
	mLengths.resize(newSize);
}

Transform FABRIKSolver::GetLocalTransform(unsigned int index) {
	return mIKChain[index];
}

void FABRIKSolver::SetLocalTransform(unsigned int index, const Transform& t) {
	mIKChain[index] = t;
}

Transform FABRIKSolver::GetGlobalTransform(unsigned int index) {
	unsigned int size = (unsigned int)mIKChain.size();
	Transform world = mIKChain[index];
	for (int i = (int)index - 1; i >= 0; --i) {
		world = combine(mIKChain[i], world);
	}
	return world;
}

unsigned int FABRIKSolver::GetNumSteps() {
	return mNumSteps;
}

void FABRIKSolver::SetNumSteps(unsigned int numSteps) {
	mNumSteps = numSteps;
}

float FABRIKSolver::GetThreshold() {
	return mThreshold;
}

void FABRIKSolver::SetThreshold(float value) {
	mThreshold = value;
}

void FABRIKSolver::IKChainToWorld() {
	unsigned int size = Size();
	for (unsigned int i = 0; i < size; ++i) {
		Transform world = GetGlobalTransform(i);
		mWorldChain[i] = world.position;

		if (i >= 1) {
			AnimVec3 prev = mWorldChain[i - 1];
			mLengths[i] = len(world.position - prev);
		}
	}
	if (size > 0) {
		mLengths[0] = 0.0f;
	}
}

void FABRIKSolver::IterateBackward(const AnimVec3& goal) {
	int size = (int)Size();
	if (size > 0) {
		mWorldChain[size - 1] = goal;
	}

	for (int i = size - 2; i >= 0; --i) {
		AnimVec3 direction = normalized(mWorldChain[i] - mWorldChain[i + 1]);
		AnimVec3 offset = direction * mLengths[i + 1];
		mWorldChain[i] = mWorldChain[i + 1] + offset;
	}
}


void FABRIKSolver::IterateForward(const AnimVec3& base) {
	unsigned int size = Size();
	if (size > 0) {
		mWorldChain[0] = base;
	}

	for (unsigned int i = 1; i < size; ++i) {
		AnimVec3 direction = normalized(mWorldChain[i] - mWorldChain[i - 1]);
		AnimVec3 offset = direction * mLengths[i];
		mWorldChain[i] = mWorldChain[i - 1] + offset;
	}
}

void FABRIKSolver::WorldToIKChain() {
	unsigned int size = Size();
	if (size == 0) { return; }

	for (unsigned int i = 0; i < size - 1; ++i) {
		Transform world = GetGlobalTransform(i);
		Transform next = GetGlobalTransform(i + 1);
		AnimVec3 position = world.position;
		AnimQuat rotation = world.rotation;

		AnimVec3 toNext = next.position - position;
		toNext = inverse(rotation) * toNext;

		AnimVec3 toDesired = mWorldChain[i + 1] - position;
		toDesired = inverse(rotation) * toDesired;

		AnimQuat delta = fromTo(toNext, toDesired);
		mIKChain[i].rotation = delta * mIKChain[i].rotation;
	}
}

bool FABRIKSolver::Solve(const Transform& target) {
	return Solve(target.position);
}

bool FABRIKSolver::Solve(const AnimVec3& goal) {
	unsigned int size = Size();
	if (size == 0) { return false; }
	unsigned int last = size - 1;
	float thresholdSq = mThreshold * mThreshold;
	
	IKChainToWorld();
	AnimVec3 base = mWorldChain[0];

	for (unsigned int i = 0; i < mNumSteps; ++i) {
		AnimVec3 effector = mWorldChain[last];
		if (lenSq(goal - effector) < thresholdSq) {
			WorldToIKChain();
			return true;
		}

		IterateBackward(goal);
		IterateForward(base);
	}

	WorldToIKChain();
	AnimVec3 effector = GetGlobalTransform(last).position;
	if (lenSq(goal - effector) < thresholdSq) {
		return true;
	}

	return false;
}
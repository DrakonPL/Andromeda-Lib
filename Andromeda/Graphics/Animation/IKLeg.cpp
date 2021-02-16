#include "IKLeg.h"

IKLeg::IKLeg(Skeleton& skeleton, const std::string& hip, const std::string& knee, const std::string& ankle, const std::string& toe) {
	mSolver.Resize(3);

	mAnkleToGroundOffset = 0.0f;

	mHipIndex = mKneeIndex = mAnkleIndex = mToeIndex = 0;
	for (unsigned int i = 0, size = skeleton.GetRestPose().Size(); i < size; ++i) {
		std::string& jointName = skeleton.GetJointName(i);
		if (jointName == hip) {
			mHipIndex = i;
		}
		else if (jointName == knee) {
			mKneeIndex = i;
		}
		else if (jointName == ankle) {
			mAnkleIndex = i;
		}
		else if (jointName == toe) {
			mToeIndex = i;
		}
	}
}

IKLeg::IKLeg() {
	mAnkleToGroundOffset = 0.0f;
	mSolver.Resize(3);
}

IKLeg::IKLeg(const IKLeg& other) {

	mAnkleToGroundOffset = 0.0f;
	mSolver.Resize(3);


	*this = other;
}

IKLeg& IKLeg::operator=(const IKLeg& other) {
	if (this == &other) {
		return *this;
	}

	mSolver = other.mSolver;
	mAnkleToGroundOffset = other.mAnkleToGroundOffset;
	mHipIndex = other.mHipIndex;
	mKneeIndex = other.mKneeIndex;
	mAnkleIndex = other.mAnkleIndex;
	mToeIndex = other.mToeIndex;

	return *this;
}

IKLeg::~IKLeg() {

}

void IKLeg::SolveForLeg(const Transform& model, Pose& pose, const AnimVec3& ankleTargetPosition) {
	mSolver.SetLocalTransform(0, combine(model, pose.GetGlobalTransform(mHipIndex)));
	mSolver.SetLocalTransform(1, pose.GetLocalTransform(mKneeIndex));
	mSolver.SetLocalTransform(2, pose.GetLocalTransform(mAnkleIndex));
	mIKPose = pose;

	mSolver.Solve(ankleTargetPosition + AnimVec3(0, 1, 0) * mAnkleToGroundOffset);
	
	Transform rootWorld = combine(model, pose.GetGlobalTransform(pose.GetParent(mHipIndex)));
	mIKPose.SetLocalTransform(mHipIndex, combine(inverse(rootWorld), mSolver.GetLocalTransform(0)));
	mIKPose.SetLocalTransform(mKneeIndex, mSolver.GetLocalTransform(1));
	mIKPose.SetLocalTransform(mAnkleIndex, mSolver.GetLocalTransform(2));
}

Pose& IKLeg::GetAdjustedPose() {
	return mIKPose;
}

ScalarTrack& IKLeg::GetTrack() {
	return mPinTrack;
}

unsigned int IKLeg::Hip() {
	return mHipIndex;
}

unsigned int IKLeg::Knee() {
	return mKneeIndex;
}

unsigned int IKLeg::Ankle() {
	return mAnkleIndex;
}

unsigned int IKLeg::Toe() {
	return mToeIndex;
}

void IKLeg::Draw(const AnimMat4& mv, const AnimVec3& legColor) {

}

void IKLeg::SetAnkleOffset(float offset) {
	mAnkleToGroundOffset = offset;
}
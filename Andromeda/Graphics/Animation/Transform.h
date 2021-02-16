#ifndef _H_TRANSFORM_
#define _H_TRANSFORM_

#include "AnimVec3.h"
#include "AnimMat4.h"
#include "AnimQuat.h"

struct Transform {
	AnimVec3 position;
	AnimQuat rotation;
	AnimVec3 scale;
	Transform() :
		position(AnimVec3(0, 0, 0)),
		rotation(AnimQuat(0, 0, 0, 1)),
		scale(AnimVec3(1, 1, 1)) {}
	Transform(const AnimVec3& p, const AnimQuat& r, const AnimVec3& s) :
		position(p), rotation(r), scale(s) {}
}; // End of transform struct

Transform combine(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);
Transform mix(const Transform& a, const Transform& b, float t);
AnimMat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const AnimMat4& m);
AnimVec3 transformPoint(const Transform& a, const AnimVec3& b);
AnimVec3 transformVector(const Transform& a, const AnimVec3& b);

bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);

#endif
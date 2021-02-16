#ifndef _H_QUAT_
#define _H_QUAT_

#define QUAT_EPSILON 0.000001f
#define QUAT_DEG2RAD 0.0174533f
#define QUAT_RAD2DEG 57.2958f

#include "AnimVec3.h"
#include "AnimMat4.h"

struct AnimQuat {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		/*struct {
			AnimVec3 vector;
			float scalar;
		};*/
		float v[4];
	};

	inline AnimQuat() :
		x(0), y(0), z(0), w(1) { }
	inline AnimQuat(float _x, float _y, float _z, float _w) :
		x(_x), y(_y), z(_z), w(_w) {}
};

AnimQuat angleAxis(float angle, const AnimVec3& axis);
AnimQuat fromTo(const AnimVec3& from, const AnimVec3& to);
AnimVec3 getAxis(const AnimQuat& quat);
float getAngle(const AnimQuat& quat);
AnimQuat operator+(const AnimQuat& a, const AnimQuat& b);
AnimQuat operator-(const AnimQuat& a, const AnimQuat& b);
AnimQuat operator*(const AnimQuat& a, float b);
AnimQuat operator-(const AnimQuat& q);
bool operator==(const AnimQuat& left, const AnimQuat& right);
bool operator!=(const AnimQuat& a, const AnimQuat& b);
bool sameOrientation(const AnimQuat& left, const AnimQuat& right);
float dot(const AnimQuat& a, const AnimQuat& b);
float lenSq(const AnimQuat& q);
float len(const AnimQuat& q);
void normalize(AnimQuat& q);
AnimQuat normalized(const AnimQuat& q);
AnimQuat conjugate(const AnimQuat& q);
AnimQuat inverse(const AnimQuat& q);
AnimQuat operator*(const AnimQuat& Q1, const AnimQuat& Q2);
AnimVec3 operator*(const AnimQuat& q, const AnimVec3& v);
AnimQuat mix(const AnimQuat& from, const AnimQuat& to, float t);
AnimQuat nlerp(const AnimQuat& from, const AnimQuat& to, float t);
AnimQuat operator^(const AnimQuat& q, float f);
AnimQuat operator^(const AnimQuat& q, float f);
AnimQuat slerp(const AnimQuat& start, const AnimQuat& end, float t);
AnimQuat lookRotation(const AnimVec3& direcion, const AnimVec3& up);
AnimMat4 quatToMat4(const AnimQuat& q);
AnimQuat mat4ToQuat(const AnimMat4& m);

#endif
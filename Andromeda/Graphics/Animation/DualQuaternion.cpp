#include "DualQuaternion.h"
#include <cmath>

DualQuaternion operator+(const DualQuaternion& l, const DualQuaternion& r) {
	return DualQuaternion(real(l) + real(r), dual(l) + dual(r));
}

DualQuaternion operator*(const DualQuaternion& dq, float f) {
	return DualQuaternion(real(dq) * f, dual(dq) * f);
}

bool operator==(const DualQuaternion& l, const DualQuaternion& r) {
	return real(l) == real(r) && dual(l) == dual(r);
}

bool operator!=(const DualQuaternion& l, const DualQuaternion& r) {
	return real(l) != real(r) || dual(l) != dual(r);
}

// Remember, multiplication order is left to right. 
// This is the opposite of matrix and quaternion multiplication order
DualQuaternion operator*(const DualQuaternion& l, const DualQuaternion& r) {
	DualQuaternion lhs = normalized(l);
	DualQuaternion rhs = normalized(r);

	return DualQuaternion(real(lhs) * real(rhs), real(lhs) * dual(rhs) + dual(lhs) * real(rhs));
}

AnimQuat real(DualQuaternion quat)
{
	return AnimQuat(quat.v[0], quat.v[1], quat.v[2], quat.v[3]);
}

AnimQuat dual(DualQuaternion quat)
{
	return AnimQuat(quat.v[4], quat.v[5], quat.v[6], quat.v[7]);
}

void real(DualQuaternion quat, AnimQuat real)
{
	quat.v[0] = real.v[0];
	quat.v[1] = real.v[1];
	quat.v[2] = real.v[2];
	quat.v[3] = real.v[3];
}

void dual(DualQuaternion quat, AnimQuat dual)
{
	quat.v[4] = dual.v[0];
	quat.v[5] = dual.v[1];
	quat.v[6] = dual.v[2];
	quat.v[7] = dual.v[3];
}

float dot(const DualQuaternion& l, const DualQuaternion& r) {
	//return dot(l.real, r.real);
	return dot(real(l), real(r));
}

DualQuaternion conjugate(const DualQuaternion& dq) {
	return DualQuaternion(conjugate(real(dq)), conjugate(dual(dq)));
}

DualQuaternion normalized(const DualQuaternion& dq) {
	float magSq = dot(real(dq), real(dq));
	if (magSq < 0.000001f) {
		return DualQuaternion();
	}
	float invMag = 1.0f / sqrtf(magSq);

	return DualQuaternion(real(dq) * invMag, dual(dq) * invMag);
}

void normalize(DualQuaternion& dq) {
	float magSq = dot(real(dq), real(dq));
	if (magSq < 0.000001f) {
		return;
	}
	float invMag = 1.0f / sqrtf(magSq);

	real(dq,real(dq) * invMag);
	dual(dq, dual(dq) * invMag);
}

DualQuaternion transformToDualQuat(const Transform& t) {
	AnimQuat d(t.position.x, t.position.y, t.position.z, 0);

	AnimQuat qr = t.rotation;
	AnimQuat qd = qr * d * 0.5f;

	return DualQuaternion(qr, qd);
}

Transform dualQuatToTransform(const DualQuaternion& dq) {
	Transform result;

	result.rotation = real(dq);

	AnimQuat d = conjugate(real(dq)) * (dual(dq) * 2.0f);
	result.position = AnimVec3(d.x, d.y, d.z);

	return result;
}

AnimVec3 transformVector(const DualQuaternion& dq, const AnimVec3& v) {
	return real(dq) * v;
}

AnimVec3 transformPoint(const DualQuaternion& dq, const AnimVec3& v) {
	AnimQuat d = conjugate(real(dq)) * (dual(dq) * 2.0f);
	AnimVec3 t = AnimVec3(d.x, d.y, d.z);

	return real(dq) * v + t;
}
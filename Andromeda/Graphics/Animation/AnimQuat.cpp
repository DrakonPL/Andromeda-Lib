#include <cmath>
#include "AnimQuat.h"

AnimQuat angleAxis(float angle, const AnimVec3& axis) {
	AnimVec3 norm = normalized(axis);
	float s = sinf(angle * 0.5f);

	return AnimQuat(
		norm.x * s,
		norm.y * s,
		norm.z * s,
		cosf(angle * 0.5f)
	);
}

AnimQuat fromTo(const AnimVec3& from, const AnimVec3& to) {
	AnimVec3 f = normalized(from);
	AnimVec3 t = normalized(to);

	if (f == t) {
		return AnimQuat();
	}
	else if (f == t * -1.0f) {
		AnimVec3 ortho = AnimVec3(1, 0, 0);
		if (fabsf(f.y) < fabsf(f.x)) {
			ortho = AnimVec3(0, 1, 0);
		}
		if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
			ortho = AnimVec3(0, 0, 1);
		}

		AnimVec3 axis = normalized(cross(f, ortho));
		return AnimQuat(axis.x, axis.y, axis.z, 0);
	}

	AnimVec3 half = normalized(f + t);
	AnimVec3 axis = cross(f, half);

	return AnimQuat(
		axis.x,
		axis.y,
		axis.z,
		dot(f, half)
	);
}

AnimVec3 getAxis(const AnimQuat& quat) {
	return normalized(AnimVec3(quat.x, quat.y, quat.z));
}

float getAngle(const AnimQuat& quat) {
	return 2.0f * acosf(quat.w);
}

AnimQuat operator+(const AnimQuat& a, const AnimQuat& b) {
	return AnimQuat(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	);
}

AnimQuat operator-(const AnimQuat& a, const AnimQuat& b) {
	return AnimQuat(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	);
}

AnimQuat operator*(const AnimQuat& a, float b) {
	return AnimQuat(
		a.x * b,
		a.y * b,
		a.z * b,
		a.w * b
	);
}

AnimQuat operator-(const AnimQuat& q) {
	return AnimQuat(
		-q.x,
		-q.y,
		-q.z,
		-q.w
	);
}

bool operator==(const AnimQuat& left, const AnimQuat& right) {
	return (fabsf(left.x - right.x) <= QUAT_EPSILON && 
		    fabsf(left.y - right.y) <= QUAT_EPSILON && 
		    fabsf(left.z - right.z) <= QUAT_EPSILON && 
		    fabsf(left.w - left.w) <= QUAT_EPSILON);
}

bool operator!=(const AnimQuat& a, const AnimQuat& b) {
	return !(a == b);
}

bool sameOrientation(const AnimQuat& left, const AnimQuat& right) {
	return (fabsf(left.x - right.x) <= QUAT_EPSILON && fabsf(left.y - right.y) <= QUAT_EPSILON && 
		    fabsf(left.z - right.z) <= QUAT_EPSILON && fabsf(left.w - left.w) <= QUAT_EPSILON)
		|| (fabsf(left.x + right.x) <= QUAT_EPSILON && fabsf(left.y + right.y) <= QUAT_EPSILON && 
			fabsf(left.z + right.z) <= QUAT_EPSILON && fabsf(left.w + left.w) <= QUAT_EPSILON);
}

float dot(const AnimQuat& a, const AnimQuat& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float lenSq(const AnimQuat& q) {
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float len(const AnimQuat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < QUAT_EPSILON) {
		return 0.0f;
	}
	return sqrtf(lenSq);
}

void normalize(AnimQuat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < QUAT_EPSILON) {
		return;
	}
	float i_len = 1.0f / sqrtf(lenSq);

	q.x *= i_len;
	q.y *= i_len;
	q.z *= i_len;
	q.w *= i_len;
}

AnimQuat normalized(const AnimQuat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < QUAT_EPSILON) {
		return AnimQuat();
	}
	float i_len = 1.0f / sqrtf(lenSq);

	return AnimQuat(
		q.x * i_len,
		q.y * i_len,
		q.z * i_len,
		q.w * i_len
	);
}

AnimQuat conjugate(const AnimQuat& q) {
	return AnimQuat(
		-q.x,
		-q.y,
		-q.z,
		q.w
	);
}

AnimQuat inverse(const AnimQuat& q) {
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq < QUAT_EPSILON) {
		return AnimQuat();
	}
	float recip = 1.0f / lenSq;

	// conjugate / norm
	return AnimQuat(
		-q.x * recip,
		-q.y * recip,
		-q.z * recip,
		 q.w * recip
	);
}

#if 1
AnimQuat operator*(const AnimQuat& Q1, const AnimQuat& Q2) {
	return AnimQuat(
		Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y + Q2.w * Q1.x,
		-Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x + Q2.w * Q1.y,
		Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w + Q2.w * Q1.z,
		-Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z + Q2.w * Q1.w
	);
}
#else
AnimQuat operator*(const AnimQuat& Q1, const AnimQuat& Q2) {
	AnimQuat result;
	result.scalar = Q2.scalar * Q1.scalar - dot(Q2.vector, Q1.vector);
	result.vector = (Q1.vector * Q2.scalar) + (Q2.vector * Q1.scalar) + cross(Q2.vector, Q1.vector);
	return result;
}
#endif

AnimVec3 operator*(const AnimQuat& q, const AnimVec3& v) {

	AnimVec3 vector(q.x, q.y, q.z);

	return vector * 2.0f * dot(vector, v) +
		v * (q.w * q.w - dot(vector, vector)) +
		cross(vector, v) * 2.0f * q.w;
}

AnimQuat mix(const AnimQuat& from, const AnimQuat& to, float t) {
	return from * (1.0f - t) + to * t;
}

AnimQuat nlerp(const AnimQuat& from, const AnimQuat& to, float t) {
	return normalized(from + (to - from) * t);
}

AnimQuat operator^(const AnimQuat& q, float f) {
	float angle = 2.0f * acosf(q.w);
	AnimVec3 vector(q.x, q.y, q.z);
	AnimVec3 axis = normalized(vector);

	float halfCos = cosf(f * angle * 0.5f);
	float halfSin = sinf(f * angle * 0.5f);

	return AnimQuat(
		axis.x * halfSin,
		axis.y * halfSin,
		axis.z * halfSin,
		halfCos
	);
}

AnimQuat slerp(const AnimQuat& start, const AnimQuat& end, float t) {
	if (fabsf(dot(start, end)) > 1.0f - QUAT_EPSILON) {
		return nlerp(start, end, t);
	}

	return normalized(((inverse(start) * end) ^ t) * start);
}

AnimQuat lookRotation(const AnimVec3& direcion, const AnimVec3& up) {
	// Find orthonormal basis vectors
	AnimVec3 f = normalized(direcion);
	AnimVec3 u = normalized(up);
	AnimVec3 r = cross(u, f);
	u = cross(f, r);

	// From world forward to object forward
	AnimQuat f2d = fromTo(AnimVec3(0, 0, 1), f);

	// what direction is the new object up?
	AnimVec3 objectUp = f2d * AnimVec3(0, 1, 0);
	// From object up to desired up
	AnimQuat u2u = fromTo(objectUp, u);

	// Rotate to forward direction first, then twist to correct up
	AnimQuat result = f2d * u2u;
	// Don’t forget to normalize the result
	return normalized(result);
}

AnimMat4 quatToMat4(const AnimQuat& q) {
	AnimVec3 r = q * AnimVec3(1, 0, 0);
	AnimVec3 u = q * AnimVec3(0, 1, 0);
	AnimVec3 f = q * AnimVec3(0, 0, 1);

	return AnimMat4(
		r.x, r.y, r.z, 0,
		u.x, u.y, u.z, 0,
		f.x, f.y, f.z, 0,
		0, 0, 0, 1
	);
}

AnimQuat mat4ToQuat(const AnimMat4& m) {
	AnimVec3 up = normalized(AnimVec3(m.yx, m.yy, m.yz));
	AnimVec3 forward = normalized(AnimVec3(m.zx, m.zy, m.zz));
	AnimVec3 right = cross(up, forward);
	up = cross(forward, right);

	return lookRotation(forward, up);
}
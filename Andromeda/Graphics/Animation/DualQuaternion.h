#ifndef _H_DUALQUATERNION_
#define _H_DUALQUATERNION_

#include "AnimQuat.h"
#include "Transform.h"

struct DualQuaternion {
	union {
		/*struct {
			AnimQuat real;
			AnimQuat dual;
		};*/
		float v[8];
	};
	inline DualQuaternion()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		v[3] = 1;

		v[4] = 0;
		v[5] = 0;
		v[6] = 0;
		v[7] = 0;
	}
	inline DualQuaternion(const AnimQuat& r, const AnimQuat& d)
	{
		v[0] = r.v[0];
		v[1] = r.v[1];
		v[2] = r.v[2];
		v[3] = r.v[3];

		v[4] = d.v[0];
		v[5] = d.v[1];
		v[6] = d.v[2];
		v[7] = d.v[3];
	}
};

AnimQuat real(DualQuaternion quat);
AnimQuat dual(DualQuaternion quat);

void real(DualQuaternion quat,AnimQuat real);
void dual(DualQuaternion quat,AnimQuat dual);

DualQuaternion operator+(const DualQuaternion& l, const DualQuaternion& r);
DualQuaternion operator*(const DualQuaternion& dq, float f);
// Multiplication order is left to right
// Left to right is the OPPOSITE of matrices and quaternions
DualQuaternion operator*(const DualQuaternion& l, const DualQuaternion& r);
bool operator==(const DualQuaternion& l, const DualQuaternion& r);
bool operator!=(const DualQuaternion& l, const DualQuaternion& r);
float dot(const DualQuaternion& l, const DualQuaternion& r);
DualQuaternion conjugate(const DualQuaternion& dq);
DualQuaternion normalized(const DualQuaternion& dq);
void normalize(DualQuaternion& dq);
DualQuaternion transformToDualQuat(const Transform& t);
Transform dualQuatToTransform(const DualQuaternion& dq);
AnimVec3 transformVector(const DualQuaternion& dq, const AnimVec3& v);
AnimVec3 transformPoint(const DualQuaternion& dq, const AnimVec3& v);

#endif
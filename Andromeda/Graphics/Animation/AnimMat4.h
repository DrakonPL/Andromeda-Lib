#ifndef _H_MAT4_
#define _H_MAT4_

#include "AnimVec3.h"
#include "AnimVec4.h"

#include <glm/glm.hpp>

#define MAT4_EPSILON 0.000001f

struct AnimMat4 {
	union {
		float v[16];
		/*struct{
			AnimVec4 right;
			AnimVec4 up;
			AnimVec4 forward;
			AnimVec4 position;
		};*/
		struct {
			//            row 1     row 2     row 3     row 4
			/* column 1 */float xx; float xy; float xz; float xw;
			/* column 2 */float yx; float yy; float yz; float yw;
			/* column 3 */float zx; float zy; float zz; float zw;
			/* column 4 */float tx; float ty; float tz; float tw;
		};
		struct {
			float c0r0; float c0r1; float c0r2; float c0r3;
			float c1r0; float c1r1; float c1r2; float c1r3;
			float c2r0; float c2r1; float c2r2; float c2r3;
			float c3r0; float c3r1; float c3r2; float c3r3;
		};
		struct {
			float r0c0; float r1c0; float r2c0; float r3c0;
			float r0c1; float r1c1; float r2c1; float r3c1;
			float r0c2; float r1c2; float r2c2; float r3c2;
			float r0c3; float r1c3; float r2c3; float r3c3;
		};
	};
	inline AnimMat4() :
		xx(1), xy(0), xz(0), xw(0),
		yx(0), yy(1), yz(0), yw(0),
		zx(0), zy(0), zz(1), zw(0),
		tx(0), ty(0), tz(0), tw(1) {}

	inline AnimMat4(float* fv) :
		xx(fv[0]), xy(fv[1]), xz(fv[2]), xw(fv[3]),
		yx(fv[4]), yy(fv[5]), yz(fv[6]), yw(fv[7]),
		zx(fv[8]), zy(fv[9]), zz(fv[10]), zw(fv[11]),
		tx(fv[12]), ty(fv[13]), tz(fv[14]), tw(fv[15]) { }

	inline AnimMat4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) :
		xx(_00), xy(_01), xz(_02), xw(_03),
		yx(_10), yy(_11), yz(_12), yw(_13),
		zx(_20), zy(_21), zz(_22), zw(_23),
		tx(_30), ty(_31), tz(_32), tw(_33) { }
}; // end AnimMat4 struct

bool operator==(const AnimMat4& a, const AnimMat4& b);
bool operator!=(const AnimMat4& a, const AnimMat4& b);
AnimMat4 operator+(const AnimMat4& a, const AnimMat4& b);
AnimMat4 operator*(const AnimMat4& m, float f);
AnimMat4 operator*(const AnimMat4& a, const AnimMat4& b);
AnimVec4 operator*(const AnimMat4& m, const AnimVec4& v);
AnimVec3 transformVector(const AnimMat4& m, const AnimVec3& v);
glm::vec3 transformVector(const AnimMat4& m, const glm::vec3& v);
AnimVec3 transformPoint(const AnimMat4& m, const AnimVec3& v);
glm::vec3 transformPoint(const AnimMat4& m, const glm::vec3& v);
AnimVec3 transformPoint(const AnimMat4& m, const AnimVec3& v, float& w);
void transpose(AnimMat4& m);
AnimMat4 transposed(const AnimMat4& m);
float determinant(const AnimMat4& m);
AnimMat4 adjugate(const AnimMat4& m);
AnimMat4 inverse(const AnimMat4& m);
void invert(AnimMat4& m);
AnimMat4 frustum(float l, float r, float b, float t, float n, float f);
AnimMat4 perspective(float fov, float aspect, float znear, float zfar);
AnimMat4 ortho(float l, float r, float b, float t, float n, float f);
AnimMat4 lookAt(const AnimVec3& position, const AnimVec3& target, const AnimVec3& up);

#endif
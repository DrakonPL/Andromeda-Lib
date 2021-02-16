#ifndef _H_INTERSECTIONS_
#define _H_INTERSECTIONS_

#include "AnimVec3.h"
//#include "AnimMesh.h"
#include <cmath>
#include <vector>

struct Ray {
	AnimVec3 origin;
	AnimVec3 direction;

	inline Ray() : direction(AnimVec3(0, -1, 0)) { }
	inline Ray(const AnimVec3& o) : origin(o), direction(AnimVec3(0, -1, 0)) { }
	inline Ray(const AnimVec3& o, const AnimVec3& d) : origin(o), direction(d) { }
};

struct Triangle {
	AnimVec3 v0;
	AnimVec3 v1;
	AnimVec3 v2;
	AnimVec3 normal;

	inline Triangle() { }
	inline Triangle(const AnimVec3& _v0, const AnimVec3& _v1, const AnimVec3& _v2) : v0(_v0), v1(_v1), v2(_v2) {
		normal = normalized(cross(v1 - v0, v2 - v0));
	}
};

bool RaycastTriangle(const Ray& ray, const Triangle& triangle, AnimVec3& hitPoint);
//std::vector<Triangle> MeshToTriangles(AnimMesh& mesh);
//std::vector<Triangle> MeshesToTriangles(std::vector<AnimMesh>& mesh);

#endif
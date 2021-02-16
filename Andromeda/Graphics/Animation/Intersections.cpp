#include "Intersections.h"

bool RaycastTriangle(const Ray& ray, const Triangle& triangle, AnimVec3& hitPoint) {
	const float EPSILON = 0.0000001f;
	AnimVec3 vertex0 = triangle.v0;
	AnimVec3 vertex1 = triangle.v1;
	AnimVec3 vertex2 = triangle.v2;
	AnimVec3 edge1, edge2, h, s, q;
	AnimVec3 rayVector = ray.direction;
	AnimVec3 rayOrigin = ray.origin;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = cross(rayVector, edge2);
	a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	f = 1.0f / a;
	s = rayOrigin - vertex0;
	u = f * dot(s, h);
	if (u < 0.0f || u > 1.0f)
		return false;
	q = cross(s, edge1);
	v = f * dot(rayVector, q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		hitPoint = rayOrigin + rayVector * t;
		return true;
	}
	
	return false;
}

/*std::vector<Triangle> MeshToTriangles(AnimMesh& mesh) {
	std::vector<Triangle> result;
	std::vector<AnimVec3> vertices = mesh.GetPosition();
	std::vector<unsigned int> indices = mesh.GetIndices();
	if (indices.size() == 0) {
		unsigned int numVertices = (unsigned int)vertices.size();
		for (unsigned int i = 0; i < numVertices; i += 3) {
			result.push_back(Triangle(
				vertices[i + 0],
				vertices[i + 1],
				vertices[i + 2]
			));
		}
	}
	else {
		unsigned int numIndices = (unsigned int)indices.size();
		for (unsigned int i = 0; i < numIndices; i += 3) {
			result.push_back(Triangle(
				vertices[indices[i + 0]],
				vertices[indices[i + 1]],
				vertices[indices[i + 2]]
			));
		}
	}
	return result;
}

std::vector<Triangle> MeshesToTriangles(std::vector<AnimMesh>& meshes) {
	std::vector<Triangle> result;
	unsigned int numMeshes = (unsigned int)meshes.size();
	for (unsigned int j = 0; j < numMeshes; ++j) {
		AnimMesh& mesh = meshes[j];
		std::vector<AnimVec3> vertices = mesh.GetPosition();
		std::vector<unsigned int> indices = mesh.GetIndices();
		if (indices.size() == 0) {
			unsigned int numVertices = (unsigned int)vertices.size();
			for (unsigned int i = 0; i < numVertices; i += 3) {
				result.push_back(Triangle(
					vertices[i + 0],
					vertices[i + 1],
					vertices[i + 2]
				));
			}
		}
		else {
			unsigned int numIndices = (unsigned int)indices.size();
			for (unsigned int i = 0; i < numIndices; i += 3) {
				result.push_back(Triangle(
					vertices[indices[i + 0]],
					vertices[indices[i + 1]],
					vertices[indices[i + 2]]
				));
			}
		}
	}
	return result;
}*/
#ifndef _H_GLTFLOADER_
#define _H_GLTFLOADER_

#include "cgltf.h"
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"


#include <vector>
#include <string>

#include "AnimatedMesh.h"
#include <Andromeda/Graphics/Models/ModelMaterial.h>

cgltf_data* LoadGLTFFile(const char* path);
std::vector<std::string> LoadJointNames(cgltf_data* data);
Pose LoadRestPose(cgltf_data* data);
Pose LoadBindPose(cgltf_data* data);
Skeleton LoadSkeleton(cgltf_data* data);
std::vector<Clip> LoadAnimationClips(cgltf_data* data);

std::vector<Andromeda::Graphics::AnimatedMesh> LoadAnimationMeshes(cgltf_data* data);//ModelMaterial
std::vector<Andromeda::Graphics::AnimatedMesh> LoadStaticMeshes(cgltf_data* data);//ModelMaterial
std::vector<Andromeda::Graphics::ModelMaterial> LoadMaterials(cgltf_data* data);

void FreeGLTFFile(cgltf_data* handle);

#endif
#ifndef _H_REARRANGEBONES_
#define _H_REARRANGEBONES_

#include <map>

#include "AnimatedMesh.h"
#include "Skeleton.h"
#include "Clip.h"

typedef std::map<int, int> BoneMap;

BoneMap RearrangeSkeleton(Skeleton& skeleton);
void RearrangeMesh(Andromeda::Graphics::AnimatedMesh& mesh, BoneMap& boneMap);
void RearrangeClip(Clip& clip, BoneMap& boneMap);
void RearrangeFastclip(FastClip& clip, BoneMap& boneMap);

#endif
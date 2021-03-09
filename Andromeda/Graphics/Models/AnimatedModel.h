#pragma once

#ifndef AnimatedModel_H
#define AnimatedModel_H

#include <Andromeda/Graphics/Animation/AnimatedMesh.h>
#include "Andromeda/Graphics/Animation/Clip.h"
#include "Andromeda/Graphics/Animation/Skeleton.h"
#include "Andromeda/Graphics/Animation/SkinningType.h"


#include <vector>

namespace Andromeda
{
	namespace Graphics
	{
		class AnimatedModel
		{
		private:
			std::vector<AnimatedMesh> _meshes;

			Skeleton mSkeleton;
			Pose mCurrentPose;

			std::vector<Clip> mClips;
			std::vector<AnimMat4> mPosePalette;

			SkinningType mSkinType;

			unsigned int mCurrentClip;
			float mPlaybackTime;

		private:

			Shader* _shader;

		public:

			AnimatedModel();
			~AnimatedModel();

			void LoadAnimatedModel(std::string modelFile);
			void LoadStaticModel(std::string modelFile);
			void LoadAllModels(std::string modelFile);

			void SetSkinningType(SkinningType skinning);

			void SetShader(Shader* shader);

			void Update(float dt);

			void Draw();
		};
	}
}

#endif

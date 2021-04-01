#pragma once

#ifndef AnimatedModel_H
#define AnimatedModel_H

#include <Andromeda/Graphics/Animation/AnimatedMesh.h>
#include "Andromeda/Graphics/Animation/Clip.h"
#include "Andromeda/Graphics/Animation/Skeleton.h"
#include "Andromeda/Graphics/Animation/SkinningType.h"


#include <vector>

#include "Andromeda/Graphics/Animation/CrossFadeController.h"
#include "Andromeda/Graphics/Animation/RearrangeBones.h"

namespace Andromeda
{
	namespace Graphics
	{
		class AnimatedModel
		{
		private:
			std::vector<AnimatedMesh> _meshes;

			Skeleton _skeleton;
			Pose _currentPose;

			BoneMap boneMap;

			std::vector<Clip> _clips;
			std::vector<std::string> _clipNames;
			int _clipsCount;

			std::vector<AnimMat4> _posePalette;
			std::vector<AnimMat4> _invBindPose;

			SkinningType _skinningType;

			unsigned int _currentClip;
			float _playbackTime;

		private:

			//blending and cross controller
			CrossFadeController _crossController;

		private:

			Shader* _shader;

			//transformation
			glm::mat4 _modelMatrix;

			glm::vec3 _position;
			glm::vec3 _rotation;
			glm::vec3 _scale;

		private:

			int _connectedBone;

		private:

			std::vector<AnimatedModel*> _attachedModels;

		private:

			void AttachMeshes(int boneIndex, glm::vec3 pos, glm::vec3 rot);

		public:

			AnimatedModel();
			~AnimatedModel();

			//load models
			void LoadSkinnedModel(std::string modelFile, SkinningType skinning);
			void LoadStaticModel(std::string modelFile);

			void LoadOnly(std::string modelFile);
			void AttachModel(AnimatedModel* model, std::string boneName, glm::vec3 pos, glm::vec3 rot);

			void SetShader(Shader* shader);
			Shader* GetShader();

			//transformations
			void SetPosition(glm::vec3 position);
			void SetRotation(glm::vec3 rotation);
			void SetScale(glm::vec3 scale);

			glm::vec3 GetPosition();
			glm::vec3 GetRotation();
			glm::vec3 GetScale();

			glm::mat4 GetModelMatrix();

			std::vector<AnimMat4> GetPosePalette();
			void SetPosePalette(std::vector<AnimMat4> posePalette);

			std::vector<AnimMat4> GetInvBindPose();
			void SetInvBindPose(std::vector<AnimMat4> invBindPose);

			int GetBoneIndex(std::string boneName);

			//clips
			int GetClipsCount();
			std::vector<std::string> GetClipsNames();

			int GetCurrentClip();
			void SetCurrentClip(int clip);
			void SetCurrentClip(std::string clip);

			float GetClipDuration(std::string clip);

			void PlayOnce(int clip);
			void PlayOnce(std::string clip);

			void FadeToClip(int clip, float time);
			void FadeToClip(std::string clip, float time);

			//update and draw 
			void Update(float dt);
			void Draw();


        };
	}
}

#endif

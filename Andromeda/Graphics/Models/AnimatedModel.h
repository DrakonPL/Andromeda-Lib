#pragma once

#ifndef AnimatedModel_H
#define AnimatedModel_H

#include <Andromeda/Graphics/Animation/AnimatedMesh.h>
#include "Andromeda/Graphics/Animation/Clip.h"
#include "Andromeda/Graphics/Animation/Skeleton.h"
#include "Andromeda/Graphics/Animation/SkinningType.h"


#include <vector>

#include "Andromeda/Graphics/Animation/CrossFadeController.h"

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

			std::vector<Clip> _clips;
			std::vector<std::string> _clipNames;
			int _clipsCount;

			std::vector<AnimMat4> _posePalette;

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



		public:

			AnimatedModel();
			~AnimatedModel();

			//load models
			void LoadAnimatedModel(std::string modelFile);
			void LoadStaticModel(std::string modelFile);
			void LoadAllModels(std::string modelFile);

			void SetSkinningType(SkinningType skinning);

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

			//clips
			int GetClipsCount();
			std::vector<std::string> GetClipsNames();

			int GetCurrentClip();
			void SetCurrentClip(int clip);
			void SetCurrentClip(std::string clip);


			void FadeToClip(int clip, float time);
			void FadeToClip(std::string clip, float time);

			//update and draw 
			void Update(float dt);

			void Draw();
		};
	}
}

#endif

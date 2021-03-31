#pragma once

#ifndef AnimatedMesh_H
#define AnimatedMesh_H

#include <Andromeda/Graphics/RenderManager.h>
#include <Andromeda/Graphics/GraphicsEnums.h>
#include <Andromeda/Graphics/Models/ModelMaterial.h>

#include <vector>

#include "Pose.h"
#include "Skeleton.h"
#include "SkinningType.h"


namespace Andromeda
{
	namespace Graphics
	{
		class AnimatedMesh
		{
		private:

			VertexType VertexTyp;
			VertexArrayObject* mesh_ = nullptr;

			int MaterialId;
			ModelMaterial* material_ = nullptr;

			SkinningType mSkinType_;

		protected:
			std::vector<glm::vec3> position_;
			std::vector<glm::vec3> normals_;
			std::vector<glm::vec2> textCoords_;
			std::vector<glm::vec4> weights_;
			std::vector<glm::ivec4> joints_;
			std::vector<unsigned int> indices_;

			std::vector<glm::vec3> mSkinnedPosition;
			std::vector<glm::vec3> mSkinnedNormal;
			std::vector<AnimMat4> mPosePalette;

		public:

			AnimatedMesh();
			~AnimatedMesh();


			std::vector<glm::vec3>& GetPositions();
			std::vector<glm::vec3>& GetNormals();
			std::vector<glm::vec2>& GetTextCoords();
			std::vector<glm::vec4>& GetWeights();
			std::vector<glm::ivec4>& GetJoints();
			std::vector<unsigned int>& GetIndices();

			void Setmaterial(ModelMaterial* material);
			ModelMaterial* GetMaterial();

			void CPUSkin(Skeleton& skeleton, Pose& pose);
			void CPUSkin(std::vector<AnimMat4>& animated);

			void UpdateSkinning();

			void CreateMesh(SkinningType mSkinType);
			void AttachMesh(SkinningType mSkinType,int boneNumber, glm::vec3 pos, glm::vec3 rot);
			void Draw();


		};
	}
}

#endif 
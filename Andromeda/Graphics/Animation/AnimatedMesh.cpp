#include "AnimatedMesh.h"


#include "SkinningType.h"
#include "Andromeda/Graphics/VertexTypes.h"

namespace Andromeda
{
	namespace Graphics
	{
		AnimatedMesh::AnimatedMesh()
		{
			 mesh_ = nullptr;
			 material_ = nullptr;
		}

		AnimatedMesh::~AnimatedMesh()
		{
			/*if (mesh_ != nullptr)
			{
				delete mesh_;
			}

			if (material_ != nullptr)
			{
				delete material_;
			}*/
		}

        void AnimatedMesh::Clean()
        {
			position_.clear();
			normals_.clear();
			textCoords_.clear();
			weights_.clear();
			joints_.clear();
			indices_.clear();
        }

        std::vector<glm::vec3>& AnimatedMesh::GetPositions()
		{
			return position_;
		}

		std::vector<glm::vec3>& AnimatedMesh::GetNormals()
		{
			return normals_;
		}

		std::vector<glm::vec2>& AnimatedMesh::GetTextCoords()
		{
			return textCoords_;
		}

		std::vector<glm::vec4>& AnimatedMesh::GetWeights()
		{
			return weights_;
		}

		std::vector<glm::ivec4>& AnimatedMesh::GetJoints()
		{
			return joints_;
		}

		std::vector<unsigned int>& AnimatedMesh::GetIndices()
		{
			return indices_;
		}

		void AnimatedMesh::CreateMesh(SkinningType mSkinType)
		{
            if (mesh_ != nullptr)
            {
				return;
            }

			mSkinType_ = mSkinType;

			if(mSkinType_ == SkinningType::GPU)
			    mesh_ = RenderManager::Instance()->CreateVertexArrayObject(NormalTextureWeighJoint, DynamicDraw);
			else
				mesh_ = RenderManager::Instance()->CreateVertexArrayObject(TextureNormal, DynamicDraw);

			//create vertices
			mesh_->CreateVertices(position_.size());

			//get vertices
			if (mSkinType_ == SkinningType::GPU)
			{
				NormalTextureWeighJointVertex* _simpleData = static_cast<NormalTextureWeighJointVertex*>(mesh_->GetVertices());

				for (size_t v = 0; v < position_.size(); v++)
				{
					_simpleData[v].Position = position_[v];
					_simpleData[v].Normal = normals_[v];
					_simpleData[v].TexCoords = textCoords_[v];
					_simpleData[v].Weights = weights_[v];

					_simpleData[v].wx = joints_[v].x;
					_simpleData[v].wy = joints_[v].y;
					_simpleData[v].wz = joints_[v].z;
					_simpleData[v].ww = joints_[v].w;
				}
			}
		    else
			{
				TextureNormalVertex* _simpleData = static_cast<TextureNormalVertex*>(mesh_->GetVertices());

				for (size_t v = 0; v < position_.size(); v++)
				{
					_simpleData[v].x = position_[v].x;
					_simpleData[v].y = position_[v].y;
					_simpleData[v].z = position_[v].z;

					_simpleData[v].nx = normals_[v].x;
					_simpleData[v].ny = normals_[v].y;
					_simpleData[v].nz = normals_[v].z;

					_simpleData[v].u = textCoords_[v].x;
					_simpleData[v].v = textCoords_[v].y;
				}
			}
			

			//create indices
			mesh_->CreateIndices(indices_.size());

			//get indices
			unsigned short* _indices = static_cast<unsigned short*>(mesh_->GetIndices());

			//set data
			for (unsigned int in = 0; in < indices_.size(); in++)
			{
				_indices[in] = indices_[in];
			}

			//generate buffer object
            if (mSkinType_ != SkinningType::CPU)
            {
				mesh_->Generate();
				Clean();
            }else
            {
				mesh_->Generate(false);
            }
			
		}

		void AnimatedMesh::AttachMesh(SkinningType mSkinType, int boneNumber, glm::vec3 pos, glm::vec3 rot)
		{
			if (mesh_ != nullptr)
			{
				return;
			}

			mSkinType_ = mSkinType;

			mesh_ = RenderManager::Instance()->CreateVertexArrayObject(NormalTextureWeighJoint, DynamicDraw);

			//create vertices
			mesh_->CreateVertices(position_.size());

			glm::mat4 rotationMat(1); // Creates a identity matrix
			rotationMat = glm::rotate(rotationMat, rot.x, glm::vec3(1.0, 0.0, 0.0));
			rotationMat = glm::rotate(rotationMat, rot.y, glm::vec3(0.0, 1.0, 0.0));
			rotationMat = glm::rotate(rotationMat, rot.z, glm::vec3(0.0, 0.0, 1.0));

			//get vertices
			NormalTextureWeighJointVertex* _simpleData = static_cast<NormalTextureWeighJointVertex*>(mesh_->GetVertices());

			for (size_t v = 0; v < position_.size(); v++)
			{
				_simpleData[v].Position = glm::vec3(rotationMat * glm::vec4(position_[v] + pos, 1.0));
				_simpleData[v].Normal = normals_[v];
				_simpleData[v].TexCoords = textCoords_[v];
				_simpleData[v].Weights = glm::vec4(1.0f, 0, 0, 0);

				_simpleData[v].wx = boneNumber;
				_simpleData[v].wy = boneNumber;
				_simpleData[v].wz = boneNumber;
				_simpleData[v].ww = boneNumber;
			}


			//create indices
			mesh_->CreateIndices(indices_.size());

			//get indices
			unsigned short* _indices = static_cast<unsigned short*>(mesh_->GetIndices());

			//set data
			for (unsigned int in = 0; in < indices_.size(); in++)
			{
				_indices[in] = indices_[in];
			}

			//generate buffer object
			if (mSkinType_ != SkinningType::CPU)
			{
				mesh_->Generate();
			}
			else
			{
				mesh_->Generate(false);
			}
		}

		void AnimatedMesh::UpdateSkinning()
		{
			if (mesh_ == nullptr)
			{
				return;
			}

            if (mSkinType_ == SkinningType::GPU)
            {
				//NormalTextureWeighJointVertex
				NormalTextureWeighJointVertex* _simpleData = static_cast<NormalTextureWeighJointVertex*>(mesh_->GetVertices());

				for (size_t v = 0; v < position_.size(); v++)
				{
					_simpleData[v].Position = mSkinnedPosition[v];
					_simpleData[v].Normal = mSkinnedNormal[v];
					_simpleData[v].TexCoords = textCoords_[v];
					_simpleData[v].Weights = weights_[v];

					_simpleData[v].wx = joints_[v].x;
					_simpleData[v].wy = joints_[v].y;
					_simpleData[v].wz = joints_[v].z;
					_simpleData[v].ww = joints_[v].w;
				}

				mesh_->UpdateVertices(_simpleData, position_.size(), false);
            }else
            {
				//ColorNormalVertex
				TextureNormalVertex* _simpleData = static_cast<TextureNormalVertex*>(mesh_->GetVertices());

				for (size_t v = 0; v < position_.size(); v++)
				{
					_simpleData[v].x = mSkinnedPosition[v].x;
					_simpleData[v].y = mSkinnedPosition[v].y;
					_simpleData[v].z = mSkinnedPosition[v].z;

					_simpleData[v].nx = mSkinnedNormal[v].x;
					_simpleData[v].ny = mSkinnedNormal[v].y;
					_simpleData[v].nz = mSkinnedNormal[v].z;

					_simpleData[v].u = textCoords_[v].x;
					_simpleData[v].v = textCoords_[v].y;
				}

				mesh_->UpdateVertices(_simpleData, position_.size(), false);
            }			
		}


		void AnimatedMesh::Setmaterial(ModelMaterial* material)
		{
			material_ = material;
		}

		ModelMaterial* AnimatedMesh::GetMaterial()
		{
			return material_;
		}

		void AnimatedMesh::CPUSkin(Skeleton& skeleton, Pose& pose)
		{
			unsigned int numVerts = (unsigned int)position_.size();
			if (numVerts == 0) { return; }

			mSkinnedPosition.resize(numVerts);
			mSkinnedNormal.resize(numVerts);

			pose.GetMatrixPalette(mPosePalette);
			std::vector<AnimMat4> invPosePalette = skeleton.GetInvBindPose();

			for (unsigned int i = 0; i < numVerts; ++i) {
				glm::ivec4& j = joints_[i];
				glm::vec4& w = weights_[i];

				// This might look a bit scary, but all we're doing here
				// is avoiding multiplying matries together if the weight
				// would be 0. The way the books text lists it, all matrices
				// are always multiplied together, which we can easily avoid.
				AnimMat4 m0, m1, m2, m3;
				if (w.x > 0.00001f) {
					m0 = (mPosePalette[j.x] * invPosePalette[j.x]) * w.x;
				}
				else {
					m0.xx = m0.yy = m0.zz = m0.tw = 0.0f;
				}
				if (w.y > 0.00001f) {
					m1 = (mPosePalette[j.y] * invPosePalette[j.y]) * w.y;
				}
				else {
					m1.xx = m1.yy = m1.zz = m1.tw = 0.0f;
				}
				if (w.z > 0.00001f) {
					m2 = (mPosePalette[j.z] * invPosePalette[j.z]) * w.z;
				}
				else {
					m2.xx = m2.yy = m2.zz = m2.tw = 0.0f;
				}
				if (w.w > 0.00001f) {
					m3 = (mPosePalette[j.w] * invPosePalette[j.w]) * w.w;
				}
				else {
					m3.xx = m3.yy = m3.zz = m3.tw = 0.0f;
				}

				AnimMat4 skin = m0 + m1 + m2 + m3;

				mSkinnedPosition[i] = transformPoint(skin, position_[i]);
				mSkinnedNormal[i] = transformVector(skin, normals_[i]);
			}

			UpdateSkinning();
		}

		void AnimatedMesh::CPUSkin(std::vector<AnimMat4>& animatedPose) {
			unsigned int numVerts = (unsigned int)position_.size();
			if (numVerts == 0) { return; }

			mSkinnedPosition.resize(numVerts);
			mSkinnedNormal.resize(numVerts);

			for (unsigned int i = 0; i < numVerts; ++i) {
				glm::ivec4& j = joints_[i];
				glm::vec4& w = weights_[i];

				glm::vec3 p0 = transformPoint(animatedPose[j.x], position_[i]);
				glm::vec3 p1 = transformPoint(animatedPose[j.y], position_[i]);
				glm::vec3 p2 = transformPoint(animatedPose[j.z], position_[i]);
				glm::vec3 p3 = transformPoint(animatedPose[j.w], position_[i]);
				mSkinnedPosition[i] = p0 * w.x + p1 * w.y + p2 * w.z + p3 * w.w;

				glm::vec3 n0 = transformVector(animatedPose[j.x], normals_[i]);
				glm::vec3 n1 = transformVector(animatedPose[j.y], normals_[i]);
				glm::vec3 n2 = transformVector(animatedPose[j.z], normals_[i]);
				glm::vec3 n3 = transformVector(animatedPose[j.w], normals_[i]);
				mSkinnedNormal[i] = n0 * w.x + n1 * w.y + n2 * w.z + n3 * w.w;
			}

			//mPosAttrib->Set(mSkinnedPosition);
			//mNormAttrib->Set(mSkinnedNormal);

			UpdateSkinning();
		}

		void AnimatedMesh::Draw()
		{
			mesh_->Draw();
		}
	}
}

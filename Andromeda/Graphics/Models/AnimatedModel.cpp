#include "Andromeda/Graphics/Models/AnimatedModel.h"
#include "Andromeda/FileSystem/FileManager.h"

#include "Andromeda/Graphics/Animation/GLTFLoader.h"
#include "Andromeda/Graphics/Animation/RearrangeBones.h"

namespace Andromeda
{
	namespace Graphics
	{
		AnimatedModel::AnimatedModel()
		{
			mSkinType = SkinningType::None;
		}

		AnimatedModel::~AnimatedModel()
		{
			_meshes.clear();
			mClips.clear();
		}		

		void AnimatedModel::LoadAnimatedModel(std::string modelFile)
		{
			std::string file = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + modelFile;
			cgltf_data* gltf = LoadGLTFFile(file.c_str());

			if (gltf)
			{
				_meshes = LoadAnimationMeshes(gltf);
				mSkeleton = LoadSkeleton(gltf);
				mClips = LoadAnimationClips(gltf);
				FreeGLTFFile(gltf);

				BoneMap bones = RearrangeSkeleton(mSkeleton);
				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i) {
					RearrangeMesh(_meshes[i], bones);
				}
				for (unsigned int i = 0, size = (unsigned int)mClips.size(); i < size; ++i) {
					RearrangeClip(mClips[i], bones);
				}

				//update meshes
				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i)
				{
					_meshes[i].CreateMesh(mSkinType);
				}
			}			
		}

		void AnimatedModel::LoadStaticModel(std::string modelFile)
		{
			std::string file = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + modelFile;
			cgltf_data* gltf = LoadGLTFFile(file.c_str());

			if (gltf)
			{
				_meshes = LoadStaticMeshes(gltf);
				FreeGLTFFile(gltf);

				//update meshes
				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i)
				{
					_meshes[i].CreateMesh(SkinningType::None);
				}
			}
		}

		void AnimatedModel::LoadAllModels(std::string modelFile)
		{
			std::string file = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + modelFile;
		}

		void AnimatedModel::SetSkinningType(SkinningType skinning)
		{
			mSkinType = skinning;
		}

		void AnimatedModel::SetShader(Shader* shader)
		{
			_shader = shader;
		}

		void AnimatedModel::Update(float dt)
		{
			if (mSkinType == SkinningType::CPU)
			{
				mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + dt);

				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i)
				{
					_meshes[i].CPUSkin(mSkeleton, mCurrentPose);
				}
			}

			if (mSkinType == SkinningType::CPUGPU)
			{
				mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + dt);
				mCurrentPose.GetMatrixPalette(mPosePalette);

				std::vector<AnimMat4>& invBindPose = mSkeleton.GetInvBindPose();
				for (unsigned int i = 0, size = (unsigned int)mPosePalette.size(); i < size; ++i) {
					mPosePalette[i] = mPosePalette[i] * invBindPose[i];
				}
			}


			if (mSkinType == SkinningType::GPU)
			{
				mPlaybackTime = mClips[mCurrentClip].Sample(mCurrentPose, mPlaybackTime + dt);
				mCurrentPose.GetMatrixPalette(mPosePalette);
			}
		}

		void AnimatedModel::Draw()
		{
			for (unsigned int i = 0, size = _meshes.size(); i < size; ++i)
			{
				glm::vec3 myColor = _meshes[i].GetMaterial()->GetColor(MaterialColorDiffuse);

				_shader->SetUniform(FragmentShader, "myColor", myColor);

				// material properties
				//_shader_gpu->SetUniform(FragmentShader, "matambient", matambient);
				//_shader_gpu->SetUniform(FragmentShader, "matdiffuse", myColor);
				//_shader_gpu->SetUniform(FragmentShader, "matspecular", matspecular); // specular lighting doesn't have full effect on this object's material
				//_shader_gpu->SetUniform(FragmentShader, "matshininess", 32.0f);

				_meshes[i].Draw();
			}
		}
	}
}

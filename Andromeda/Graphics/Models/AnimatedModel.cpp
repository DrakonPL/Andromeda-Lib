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
			_skinningType = SkinningType::None;
		}

		AnimatedModel::~AnimatedModel()
		{
			_meshes.clear();
			_clips.clear();
		}		

		void AnimatedModel::LoadAnimatedModel(std::string modelFile)
		{
			std::string file = Andromeda::FileSystem::FileManager::Instance()->GetMainDirPath() + modelFile;
			cgltf_data* gltf = LoadGLTFFile(file.c_str());

			if (gltf)
			{
				_meshes = LoadAnimationMeshes(gltf);
				_skeleton = LoadSkeleton(gltf);
				_clips = LoadAnimationClips(gltf);
				FreeGLTFFile(gltf);

				BoneMap bones = RearrangeSkeleton(_skeleton);
				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i) {
					RearrangeMesh(_meshes[i], bones);
				}
				for (unsigned int i = 0, size = (unsigned int)_clips.size(); i < size; ++i) {
					RearrangeClip(_clips[i], bones);
				}

				//update meshes
				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i)
				{
					_meshes[i].CreateMesh(_skinningType);
				}

				_currentClip = 0;
				_currentPose = _skeleton.GetRestPose();

				_clipsCount = _clips.size();

				for (unsigned int i = 0; i < _clipsCount ;++i)
				{
					_clipNames.push_back(_clips[i].GetName());
				}


				_crossController.SetSkeleton(_skeleton);
				//_crossController.Play(&_clips[_currentClip]);
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
			_skinningType = skinning;
		}

		void AnimatedModel::SetShader(Shader* shader)
		{
			_shader = shader;
		}

		Shader* AnimatedModel::GetShader()
		{
			return _shader;
		}

		//transformations
		void AnimatedModel::SetPosition(glm::vec3 position)
		{
			_position = position;
		}

		void AnimatedModel::SetRotation(glm::vec3 rotation)
		{
			_rotation = rotation;
		}

		void AnimatedModel::SetScale(glm::vec3 scale)
		{
			_scale = scale;
		}

		glm::vec3 AnimatedModel::GetPosition()
		{
			return _position;
		}

		glm::vec3 AnimatedModel::GetRotation()
		{
			return _rotation;
		}

		glm::vec3 AnimatedModel::GetScale()
		{
			return _scale;
		}

		glm::mat4 AnimatedModel::GetModelMatrix()
		{
			_modelMatrix = glm::mat4(1.0);

			//translation
			_modelMatrix = glm::translate(_modelMatrix, _position);

			//rotate
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			//scale
			_modelMatrix = glm::scale(_modelMatrix, _scale);

			return _modelMatrix;
		}

		int AnimatedModel::GetClipsCount()
		{
			return _clipsCount;
		}

		std::vector<std::string> AnimatedModel::GetClipsNames()
		{
			return _clipNames;
		}

		int AnimatedModel::GetCurrentClip()
		{
			return _currentClip;
		}

		void AnimatedModel::SetCurrentClip(int clip)
		{
			_currentClip = clip;
			_crossController.Play(&_clips[_currentClip]);
		}

		void AnimatedModel::SetCurrentClip(std::string clip)
		{
            for (int i = 0;i < _clipsCount;i++)
            {
                if (_clipNames[i] == clip)
                {
					_currentClip = i;

					_crossController.Play(&_clips[_currentClip]);
					return;
                }
            }
		}

		void AnimatedModel::FadeToClip(int clip, float time)
		{
			_currentClip = clip;
			_crossController.FadeTo(&_clips[_currentClip], time);
		}

		void AnimatedModel::FadeToClip(std::string clip, float time)
		{
			for (int i = 0; i < _clipsCount; i++)
			{
				if (_clipNames[i] == clip)
				{
					_currentClip = i;

					_crossController.FadeTo(&_clips[_currentClip],time);
					return;
				}
			}
		}

		void AnimatedModel::Update(float dt)
		{
			_crossController.Update(dt);

			if (_skinningType == SkinningType::CPU)
			{
				//_playbackTime = _clips[_currentClip].Sample(_currentPose, _playbackTime + dt);

				for (unsigned int i = 0, size = (unsigned int)_meshes.size(); i < size; ++i)
				{
					_meshes[i].CPUSkin(_skeleton, _crossController.GetCurrentPose());
				}
			}

			if (_skinningType == SkinningType::CPUGPU)
			{
				//_playbackTime = _clips[_currentClip].Sample(_currentPose, _playbackTime + dt);
				_crossController.GetCurrentPose().GetMatrixPalette(_posePalette);

				std::vector<AnimMat4>& invBindPose = _skeleton.GetInvBindPose();
				for (unsigned int i = 0, size = (unsigned int)_posePalette.size(); i < size; ++i) {
					_posePalette[i] = _posePalette[i] * invBindPose[i];
				}
			}

			if (_skinningType == SkinningType::GPU)
			{
				//_playbackTime = _clips[_currentClip].Sample(_currentPose, _playbackTime + dt);
				_crossController.GetCurrentPose().GetMatrixPalette(_posePalette);
			}
		}

		void AnimatedModel::Draw()
		{
			if (_skinningType == SkinningType::GPU)
			{
				_shader->Set(VertexShader, "pose", _posePalette);
				_shader->Set(VertexShader, "invBindPose", _skeleton.GetInvBindPose());
			}

			if (_skinningType == SkinningType::CPUGPU)
			{
				_shader->Set(VertexShader, "pose", _posePalette);
			}

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

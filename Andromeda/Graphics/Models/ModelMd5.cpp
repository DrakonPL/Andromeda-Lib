

#include <Andromeda/Graphics/Models/ModelMd5.h>
#include <Andromeda/Graphics/Models/ModelMd5Utils.h>
#include <Andromeda/FileSystem/FileManager.h>
#include <Andromeda/Utils/Logger.h>

#include <fstream> 

namespace Andromeda
{
	namespace Graphics
	{
		ModelMd5::ModelMd5()
			: m_iMD5Version(-1)
			, m_iNumJoints(0)
			, m_iNumMeshes(0)
			, m_bHasAnimation(false)
			, m_LocalToWorldMatrix(1)
			, m_WorldToLoacalMatrix(1)
			, m_VertexSkinning(VS_CPU)
		{

		}

		ModelMd5::~ModelMd5()
		{
			MeshList::iterator iter = m_Meshes.begin();
			while (iter != m_Meshes.end())
			{
				DestroyMesh(*iter);
				++iter;
			}
			m_Meshes.clear();
		}

		void ModelMd5::DestroyMesh(Mesh& mesh)
		{
			mesh.m_PositionBuffer.clear();
			mesh.m_Tex2DBuffer.clear();
			mesh.m_BoneIndex.clear();
			mesh.m_BoneWeights.clear();

			//delete vertex array object
			delete mesh._mesh;

			//remove texture
			if (mesh.m_Material.Image != NULL)
			{
				TextureManager::Instance()->Remove(mesh.m_Material.Image);
			}
		}

		void ModelMd5::SetVertexSkinning(VertexSkinning skinning)
		{
			m_VertexSkinning = skinning;
		}

		ModelMd5::VertexSkinning ModelMd5::GetVertexSkinning() const
		{
			return m_VertexSkinning;
		}

		void ModelMd5::SetWorldTransform(const glm::mat4x4 world)
		{
			m_LocalToWorldMatrix = world;
			m_WorldToLoacalMatrix = glm::inverse(m_LocalToWorldMatrix);
		}

		glm::mat4x4 ModelMd5::GetWorldTransform() const
		{
			return m_LocalToWorldMatrix;
		}

		glm::mat4x4 ModelMd5::GetInverseWorldTransform() const
		{
			return m_WorldToLoacalMatrix;
		}

		bool ModelMd5::LoadModel(const std::string &filename)
		{
			std::string filePath = filename;

			// store the parent path used for loading images relative to this file.
			std::string parent_path = "";// filePath.parent_path();

			std::string param;
			std::string junk;   // Read junk from the file

			std::string fileLocation = FileSystem::FileManager::Instance()->GetMainDirPath() + filename;

			if (!FileSystem::FileManager::Instance()->FileExists(filename))
			{
				Utils::Logger::Instance()->Log("[ModelMd5::LoadModel]File: %s not exists\n", fileLocation.c_str());
				return false;
			}

			Utils::Logger::Instance()->Log("[ModelMd5::LoadModel]File: %s\n",fileLocation.c_str());

			std::fstream file(fileLocation);
			int fileLength = ModelMd5Utils::GetFileLength(file);

			Utils::Logger::Instance()->Log("[ModelMd5::LoadModel]File size: %s\n", fileLength);
			assert(fileLength > 0);

			m_Joints.clear();
			m_Meshes.clear();

			file >> param;

			while (!file.eof())
			{
				if (param == "MD5Version")
				{
					file >> m_iMD5Version;
					assert(m_iMD5Version == 10);
				}
				else if (param == "commandline")
				{
					ModelMd5Utils::IgnoreLine(file, fileLength); // Ignore the contents of the line
				}
				else if (param == "numJoints")
				{
					file >> m_iNumJoints;
					m_Joints.reserve(m_iNumJoints);
					m_AnimatedBones.assign(m_iNumJoints, glm::mat4x4(1.0f));
				}
				else if (param == "numMeshes")
				{
					file >> m_iNumMeshes;
					m_Meshes.reserve(m_iNumMeshes);
				}
				else if (param == "joints")
				{
					Joint joint;
					file >> junk; // Read the '{' character
					for (int i = 0; i < m_iNumJoints; ++i)
					{
						file >> joint.m_Name >> joint.m_ParentID >> junk
							>> joint.m_Pos.x >> joint.m_Pos.y >> joint.m_Pos.z >> junk >> junk
							>> joint.m_Orient.x >> joint.m_Orient.y >> joint.m_Orient.z >> junk;

						ModelMd5Utils::RemoveQuotes(joint.m_Name);
						ModelMd5Utils::ComputeQuatW(joint.m_Orient);

						m_Joints.push_back(joint);
						// Ignore everything else on the line up to the end-of-line character.
						ModelMd5Utils::IgnoreLine(file, fileLength);
					}
					file >> junk; // Read the '}' character

					BuildBindPose(m_Joints);
				}
				else if (param == "mesh")
				{
					Mesh mesh;
					int numVerts, numTris, numWeights;

					file >> junk; // Read the '{' character
					file >> param;
					while (param != "}")  // Read until we get to the '}' character
					{
						if (param == "shader")
						{
							file >> mesh.m_Shader;
							ModelMd5Utils::RemoveQuotes(mesh.m_Shader);

							std::string pathName(filename);
							size_t found = pathName.find_last_of("/");
							pathName = pathName.substr(0, found) + "/" + mesh.m_Shader + ".png";

							mesh.m_Material.Image = TextureManager::Instance()->LoadFromFile(pathName);

							file.ignore(fileLength, '\n'); // Ignore everything else on the line
						}
						else if (param == "numverts")
						{
							file >> numVerts;               // Read in the vertices
							ModelMd5Utils::IgnoreLine(file, fileLength);
							for (int i = 0; i < numVerts; ++i)
							{
								Vertex vert;

								file >> junk >> junk >> junk                    // vert vertIndex (
									>> vert.m_Tex0.x >> vert.m_Tex0.y >> junk  //  s t )
									>> vert.m_StartWeight >> vert.m_WeightCount;

								ModelMd5Utils::IgnoreLine(file, fileLength);

								mesh.m_Verts.push_back(vert);
								mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
							}
						}
						else if (param == "numtris")
						{
							file >> numTris;
							ModelMd5Utils::IgnoreLine(file, fileLength);
							for (int i = 0; i < numTris; ++i)
							{
								Triangle tri;
								file >> junk >> junk >> tri.m_Indices[0] >> tri.m_Indices[1] >> tri.m_Indices[2];

								ModelMd5Utils::IgnoreLine(file, fileLength);

								mesh.m_Tris.push_back(tri);
								mesh.m_IndexBuffer.push_back((int)tri.m_Indices[0]);
								mesh.m_IndexBuffer.push_back((int)tri.m_Indices[1]);
								mesh.m_IndexBuffer.push_back((int)tri.m_Indices[2]);
							}
						}
						else if (param == "numweights")
						{
							file >> numWeights;
							ModelMd5Utils::IgnoreLine(file, fileLength);
							for (int i = 0; i < numWeights; ++i)
							{
								Weight weight;
								file >> junk >> junk >> weight.m_JointID >> weight.m_Bias >> junk
									>> weight.m_Pos.x >> weight.m_Pos.y >> weight.m_Pos.z >> junk;

								ModelMd5Utils::IgnoreLine(file, fileLength);
								mesh.m_Weights.push_back(weight);
							}
						}
						else
						{
							ModelMd5Utils::IgnoreLine(file, fileLength);
						}

						file >> param;
					}

					PrepareMesh(mesh);
					PrepareNormals(mesh);
					CreateVertexBuffers(mesh);

					m_Meshes.push_back(mesh);
				}

				file >> param;
			}

			assert(m_Joints.size() == m_iNumJoints);
			assert(m_Meshes.size() == m_iNumMeshes);

			return true;
		}

		bool ModelMd5::LoadAnim(const std::string& filename)
		{
			if (m_Animation.LoadAnimation(filename))
			{
				// Check to make sure the animation is appropriate for this model
				m_bHasAnimation = CheckAnimation(m_Animation);
			}

			return m_bHasAnimation;
		}

		bool ModelMd5::CheckAnimation(const ModelMd5Animation& animation) const
		{
			if (m_iNumJoints != animation.GetNumJoints())
			{
				return false;
			}

			// Check to make sure the joints match up
			for (unsigned int i = 0; i < m_Joints.size(); ++i)
			{
				const Joint& meshJoint = m_Joints[i];
				const ModelMd5Animation::JointInfo& animJoint = animation.GetJointInfo(i);

				if (meshJoint.m_Name != animJoint.m_Name ||
					meshJoint.m_ParentID != animJoint.m_ParentID)
				{
					return false;
				}
			}

			return true;
		}

		void ModelMd5::BuildBindPose(const JointList& joints)
		{
			m_BindPose.clear();
			m_InverseBindPose.clear();

			JointList::const_iterator iter = joints.begin();
			while (iter != joints.end())
			{
				const Joint& joint = (*iter);
				glm::mat4x4 boneTranslation = glm::translate(joint.m_Pos);
				glm::mat4x4 boneRotation = glm::toMat4(joint.m_Orient);

				glm::mat4x4 boneMatrix = boneTranslation * boneRotation;

				glm::mat4x4 inverseBoneMatrix = glm::inverse(boneMatrix);

				m_BindPose.push_back(boneMatrix);
				m_InverseBindPose.push_back(inverseBoneMatrix);

				++iter;
			}
		}

		// Compute the position of the vertices in object local space
		// in the skeleton's bind pose
		bool ModelMd5::PrepareMesh(Mesh& mesh)
		{
			mesh.m_PositionBuffer.clear();
			mesh.m_Tex2DBuffer.clear();
			mesh.m_BoneIndex.clear();
			mesh.m_BoneWeights.clear();

			// Compute vertex positions
			for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i)
			{
				glm::vec3 finalPos(0);
				Vertex& vert = mesh.m_Verts[i];

				vert.m_Pos = glm::vec3(0);
				vert.m_Normal = glm::vec3(0);
				vert.m_BoneWeights[0] = vert.m_BoneWeights[1] = vert.m_BoneWeights[2] = vert.m_BoneWeights[3] = vert.m_BoneWeights[4] = 0.0f;// = int[5];// glm::vec4(0);
				vert.m_BoneIndices[0] = vert.m_BoneIndices[1] = vert.m_BoneIndices[2] = vert.m_BoneIndices[3] = vert.m_BoneIndices[4] = 0;// = glm::vec4(0);

				// Sum the position of the weights
				for (int j = 0; j < vert.m_WeightCount; ++j)
				{
					Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
					Joint& joint = m_Joints[weight.m_JointID];

					// Convert the weight position from Joint local space to object space
					glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;

					vert.m_Pos += (joint.m_Pos + rotPos) * weight.m_Bias;
					vert.m_BoneIndices[j] = (float)weight.m_JointID;
					vert.m_BoneWeights[j] = weight.m_Bias;
				}

				mesh.m_PositionBuffer.push_back(vert.m_Pos);
				mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
				//mesh.m_BoneIndex.push_back(vert.m_BoneIndices);
				//mesh.m_BoneWeights.push_back(vert.m_BoneWeights);
			}

			bool update = true;

			if (mesh._mesh == 0)
			{
				mesh._mesh = RenderManager::Instance()->CreateVertexArrayObject(TextureColor, DynamicDraw);

				//vertices
				mesh._mesh->CreateVertices(mesh.m_Verts.size());

				//indices
				//mesh.m_IndexBuffer
				mesh._mesh->CreateIndices(mesh.m_IndexBuffer.size());

				update = false;
			}

			TextureColorVertex* _simpleData;

			if (update)
			{
				_simpleData = new TextureColorVertex[mesh.m_Verts.size()];
			}
			else
			{
				_simpleData = static_cast<TextureColorVertex*>(mesh._mesh->GetVertices());
			}

			if (update)
			{
				for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i)
				{
					Vertex& vert = mesh.m_Verts[i];
					glm::vec3& pos = mesh.m_PositionBuffer[i];

					_simpleData[i].x = pos.x;
					_simpleData[i].y = pos.y;
					_simpleData[i].z = pos.z;

					_simpleData[i].u = vert.m_Tex0.x;
					_simpleData[i].v = vert.m_Tex0.y;

					_simpleData[i].r = _simpleData->g = _simpleData->b = 1.0f;
				}
			}
			else
			{
				for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i)
				{
					Vertex& vert = mesh.m_Verts[i];

					_simpleData[i].x = vert.m_Pos.x;
					_simpleData[i].y = vert.m_Pos.y;
					_simpleData[i].z = vert.m_Pos.z;

					_simpleData[i].u = vert.m_Tex0.x;
					_simpleData[i].v = vert.m_Tex0.y;

					_simpleData[i].r = _simpleData->g = _simpleData->b = 1.0f;
				}
			}

			if (!update)
			{
				unsigned short* _indices = static_cast<unsigned short*>(mesh._mesh->GetIndices());
				for (unsigned int i = 0; i < mesh.m_IndexBuffer.size(); ++i)
				{
					_indices[i] = mesh.m_IndexBuffer[i];
				}

				//generate buffer object
				mesh._mesh->Generate();
			}
			else
			{
				mesh._mesh->UpdateVertices(_simpleData, mesh.m_Verts.size());
			}


			return true;
		}

		// Compute the vertex normals in the Mesh's bind pose
		bool ModelMd5::PrepareNormals(Mesh& mesh)
		{
			mesh.m_NormalBuffer.clear();

			// Loop through all triangles and calculate the normal of each triangle
			for (unsigned int i = 0; i < mesh.m_Tris.size(); ++i)
			{
				glm::vec3 v0 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[0]].m_Pos;
				glm::vec3 v1 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[1]].m_Pos;
				glm::vec3 v2 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[2]].m_Pos;

				glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

				mesh.m_Verts[mesh.m_Tris[i].m_Indices[0]].m_Normal += normal;
				mesh.m_Verts[mesh.m_Tris[i].m_Indices[1]].m_Normal += normal;
				mesh.m_Verts[mesh.m_Tris[i].m_Indices[2]].m_Normal += normal;
			}

			// Now normalize all the normals
			for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i)
			{
				Vertex& vert = mesh.m_Verts[i];

				vert.m_Normal = glm::normalize(vert.m_Normal);
				mesh.m_NormalBuffer.push_back(vert.m_Normal);
			}

			return true;
		}

		void  ModelMd5::AddJointRotation(std::string jointName, glm::quat rotation)
		{
			for (size_t i = 0; i < m_Joints.size(); i++)
			{
				Joint* joint = &m_Joints[i];

				if (joint->m_Name == jointName)
				{
					joint->m_Orient = rotation;
				}
			}

			for (unsigned int i = 0; i < m_Meshes.size(); ++i)
			{
				PrepareMesh(m_Meshes[i]);
			}
		}

		bool ModelMd5::PrepareMesh(Mesh& mesh, const std::vector<glm::mat4x4>& skel)
		{
			for (unsigned int v = 0; v < mesh.m_Verts.size(); ++v)
			{
				const Vertex& vert = mesh.m_Verts[v];
				glm::vec3& pos = mesh.m_PositionBuffer[v];
				glm::vec3& normal = mesh.m_NormalBuffer[v];

				pos = glm::vec3(0);
				normal = glm::vec3(0);

				for (int j = 0; j < vert.m_WeightCount; ++j)
				{
					const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
					const glm::mat4x4 boneMatrix = skel[weight.m_JointID];

					pos += glm::vec3((boneMatrix * glm::vec4(vert.m_Pos, 1.0f)) * weight.m_Bias);
					normal += glm::vec3((boneMatrix * glm::vec4(vert.m_Normal, 0.0f)) * weight.m_Bias);
				}
			}

			bool update = true;

			if (mesh._mesh == 0)
			{
				mesh._mesh = RenderManager::Instance()->CreateVertexArrayObject(TextureColor, DynamicDraw);

				//vertices
				mesh._mesh->CreateVertices(mesh.m_Verts.size());

				//indices
				//mesh.m_IndexBuffer
				mesh._mesh->CreateIndices(mesh.m_IndexBuffer.size());

				update = false;
			}

			TextureColorVertex* _simpleData;

			if (update)
			{
				_simpleData = new TextureColorVertex[mesh.m_Verts.size()];
			}
			else
			{
				_simpleData = static_cast<TextureColorVertex*>(mesh._mesh->GetVertices());
			}

			if (update)
			{
				for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i)
				{
					Vertex& vert = mesh.m_Verts[i];
					glm::vec3& pos = mesh.m_PositionBuffer[i];

					_simpleData[i].x = pos.x;
					_simpleData[i].y = pos.y;
					_simpleData[i].z = pos.z;

					_simpleData[i].u = vert.m_Tex0.x;
					_simpleData[i].v = vert.m_Tex0.y;

					_simpleData[i].r = _simpleData->g = _simpleData->b = 1.0f;
				}
			}
			else
			{
				for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i)
				{
					Vertex& vert = mesh.m_Verts[i];

					_simpleData[i].x = vert.m_Pos.x;
					_simpleData[i].y = vert.m_Pos.y;
					_simpleData[i].z = vert.m_Pos.z;

					_simpleData[i].u = vert.m_Tex0.x;
					_simpleData[i].v = vert.m_Tex0.y;

					_simpleData[i].r = _simpleData->g = _simpleData->b = 1.0f;
				}
			}

			if (!update)
			{
				unsigned short* _indices = static_cast<unsigned short*>(mesh._mesh->GetIndices());
				for (unsigned int i = 0; i < mesh.m_IndexBuffer.size(); ++i)
				{
					_indices[i] = mesh.m_IndexBuffer[i];
				}

				//generate buffer object
				mesh._mesh->Generate();
			}
			else
			{
				mesh._mesh->UpdateVertices(_simpleData, mesh.m_Verts.size());
			}

			return true;
		}

		bool ModelMd5::CreateVertexBuffers(Mesh& mesh)
		{
			return true;
		}

		void ModelMd5::Update(float fDeltaTime)
		{
			if (m_bHasAnimation)
			{
				m_Animation.Update(fDeltaTime);

				const MatrixList& animatedSkeleton = m_Animation.GetSkeletonMatrixList();
				// Multiply the animated skeleton joints by the inverse of the bind pose.
				for (int i = 0; i < m_iNumJoints; ++i)
				{
					m_AnimatedBones[i] = animatedSkeleton[i] * m_InverseBindPose[i];
				}

				for (unsigned int i = 0; i < m_Meshes.size(); ++i)
				{
					// NOTE: This only needs to be done for CPU skinning, but if I want to render the
					// animated normals, I have to update the mesh on the CPU.
					PrepareMesh(m_Meshes[i], m_AnimatedBones);
				}
			}
			else
			{
				// No animation.. Just use identity matrix for each bone.
				m_AnimatedBones.assign(m_iNumJoints, glm::mat4x4(1.0));
			}			
		}

		void ModelMd5::Render()
		{
			// Render the meshes
			for (unsigned int i = 0; i < m_Meshes.size(); ++i)
			{
				RenderMesh(m_Meshes[i]);
			}

			//m_Animation.Render();
		}

		void ModelMd5::RenderCPU(const Mesh& mesh)
		{
			if (mesh.m_Material.Image != NULL)
			{
				RenderManager::Instance()->UseTexture(mesh.m_Material.Image);

				mesh._mesh->Draw();
			}
		}

		void ModelMd5::RenderGPU(const Mesh& mesh)
		{
			
		}

		void ModelMd5::RenderMesh(const Mesh& mesh)
		{
			switch (m_VertexSkinning)
			{
			case VS_CPU:
			{
				RenderCPU(mesh);
			}
			break;
			case VS_GPU:
			{
				RenderGPU(mesh);
			}
			break;
			}
		}

		void ModelMd5::RenderNormals(const Mesh& mesh)
		{

		}

		void ModelMd5::RenderSkeleton(const JointList& joints)
		{
			
		}		
	}
}
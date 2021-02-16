

#include <Andromeda/Graphics/GXM/ShaderGxm.h>
#include <Andromeda/Graphics/VertexTypes.h>
#include <Andromeda/FileSystem/FileManager.h>
#include <Andromeda/Utils/Logger.h>

namespace Andromeda
{
	namespace Graphics
	{
		ShaderGxm::ShaderGxm() : Shader()
		{
			
		}

		ShaderGxm::~ShaderGxm()
		{
			sceGxmShaderPatcherReleaseFragmentProgram(_shaderPatcher, _fragmentProgram);
			sceGxmShaderPatcherReleaseVertexProgram(_shaderPatcher, _vertexProgram);

			sceGxmShaderPatcherUnregisterProgram(_shaderPatcher, _fragmentProgramId);
			sceGxmShaderPatcherUnregisterProgram(_shaderPatcher, _vertexProgramId);
			
			delete _gxmVertexProgram;
			delete _gxmFragmentProgram;
		}

		bool ShaderGxm::LoadFromFile(std::string vertexFile, std::string fragmentFile, VertexType vertexType)
		{
			_vertexType = vertexType;
			
			Utils::Logger::Instance()->Log("LoadShaderFile(vertexFile,VertexShader);\n");

			//load vertex shader
			LoadShaderFile(vertexFile,VertexShader);
			
			Utils::Logger::Instance()->Log("LoadShaderFile(vertexFile,FragmentShader);\n");

			//load fragment shader
			LoadShaderFile(fragmentFile,FragmentShader);
			
			Utils::Logger::Instance()->Log("LinkShader;\n");

			//link program
			LinkShader();
			
			return true;
		}

		bool ShaderGxm::LoadFromMemory(std::string vertexShader, std::string fragmentShader, VertexType vertexType)
		{
			_vertexType = vertexType;
			
			return true;
		}

		unsigned int ShaderGxm::LoadShaderFile(std::string fileName, ShaderType shaderType)
		{
			Utils::Logger::Instance()->Log("Test1 %s\n",fileName.c_str());
			
			FileSystem::BaseFile* file = FileSystem::FileManager::Instance()->GetFile(fileName);

			if (!file->Exist())
			{
				Utils::Logger::Instance()->Log("Can't open %s\n", fileName.c_str());
				return -1;
			}				

			file->Open(FileSystem::Read, FileSystem::Binary);
			
			Utils::Logger::Instance()->Log("Test2\n");

			int size = 0;

			if (shaderType == VertexShader)
			{
				_gxmVertexProgram = (SceGxmProgram*)file->GetData(size);
			}
			else
			{
				_gxmFragmentProgram = (SceGxmProgram*)file->GetData(size);
			}

			file->Close();
			delete file;
			
			Utils::Logger::Instance()->Log("Test3\n");

			if(shaderType == VertexShader)
			{
				Utils::Logger::Instance()->Log("Test4\n");
				// check the shaders
				sceGxmProgramCheck(_gxmVertexProgram);
				
				Utils::Logger::Instance()->Log("Test5\n");

				// register programs with the patcher
				sceGxmShaderPatcherRegisterProgram(_shaderPatcher, _gxmVertexProgram, &_vertexProgramId);
				
				Utils::Logger::Instance()->Log("Test6\n");
			}else
			{
				Utils::Logger::Instance()->Log("Test4\n");
				// check the shaders
				sceGxmProgramCheck(_gxmFragmentProgram);
				
				Utils::Logger::Instance()->Log("Test5\n");

				// register programs with the patcher
				sceGxmShaderPatcherRegisterProgram(_shaderPatcher, _gxmFragmentProgram, &_fragmentProgramId);
				
				Utils::Logger::Instance()->Log("Test6\n");
			}

			return 0;
		}

		unsigned int ShaderGxm::LinkShader()
		{
			Utils::Logger::Instance()->Log("LinkShader - start\n");
			
			SceGxmVertexAttribute *vertexArrtibs;
			SceGxmVertexStream *vertexstream;
			int paramsCount = 0;

			switch (_vertexType)
			{
			case Andromeda::Graphics::Simple:
			{
				Utils::Logger::Instance()->Log("LinkShader - Simple\n");
				
				paramsCount = 1;
				
				vertexArrtibs = new SceGxmVertexAttribute[paramsCount];
				vertexstream = new SceGxmVertexStream[1];

				//get attribute index
				const SceGxmProgramParameter *paramClearPositionAttribute = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iPosition");

				//create clear vertex format
				vertexArrtibs[0].streamIndex = 0;
				vertexArrtibs[0].offset = 0;
				vertexArrtibs[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[0].componentCount = 3;
				vertexArrtibs[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramClearPositionAttribute);

			
				vertexstream[0].stride = sizeof(SimpleVertex);
				vertexstream[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
			}
				break;
			case Andromeda::Graphics::Color:
			{
				Utils::Logger::Instance()->Log("LinkShader - Color\n");
				
				paramsCount = 2;

				vertexArrtibs = new SceGxmVertexAttribute[paramsCount];
				vertexstream = new SceGxmVertexStream[1];

				//get attribute index
				const SceGxmProgramParameter *positionParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iPosition");
				const SceGxmProgramParameter *colorParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iColor");

				//position param
				vertexArrtibs[0].streamIndex = 0;
				vertexArrtibs[0].offset = 0;
				vertexArrtibs[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[0].componentCount = 3;
				vertexArrtibs[0].regIndex = sceGxmProgramParameterGetResourceIndex(positionParam);

				//color param
				vertexArrtibs[1].streamIndex = 0;
				vertexArrtibs[1].offset = 12;
				vertexArrtibs[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[1].componentCount = 3;
				vertexArrtibs[1].regIndex = sceGxmProgramParameterGetResourceIndex(colorParam);


				vertexstream[0].stride = sizeof(ColorVertex);
				vertexstream[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
			}
				break;
			case Andromeda::Graphics::Textured:
			{
				Utils::Logger::Instance()->Log("LinkShader - Textured\n");
				
				paramsCount = 2;

				vertexArrtibs = new SceGxmVertexAttribute[paramsCount];
				vertexstream = new SceGxmVertexStream[1];

				//get attribute index
				const SceGxmProgramParameter *positionParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iPosition");
				const SceGxmProgramParameter *textureParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iTexCoord");

				//position param
				vertexArrtibs[0].streamIndex = 0;
				vertexArrtibs[0].offset = 0;
				vertexArrtibs[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[0].componentCount = 3;
				vertexArrtibs[0].regIndex = sceGxmProgramParameterGetResourceIndex(positionParam);

				//texture param
				vertexArrtibs[1].streamIndex = 0;
				vertexArrtibs[1].offset = 12;
				vertexArrtibs[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[1].componentCount = 2;
				vertexArrtibs[1].regIndex = sceGxmProgramParameterGetResourceIndex(textureParam);

				vertexstream[0].stride = sizeof(TextureVertex);
				vertexstream[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
			}
				break;
			case Andromeda::Graphics::TextureNormal:
			{
				Utils::Logger::Instance()->Log("LinkShader - TextureNormal\n");
				
				paramsCount = 3;

				vertexArrtibs = new SceGxmVertexAttribute[paramsCount];
				vertexstream = new SceGxmVertexStream[1];

				//get attribute index
				const SceGxmProgramParameter *positionParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iPosition");
				const SceGxmProgramParameter *colorParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iNormal");
				const SceGxmProgramParameter *textureParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iTexCoord");

				//position param
				vertexArrtibs[0].streamIndex = 0;
				vertexArrtibs[0].offset = 0;
				vertexArrtibs[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[0].componentCount = 3;
				vertexArrtibs[0].regIndex = sceGxmProgramParameterGetResourceIndex(positionParam);

				//color param
				vertexArrtibs[1].streamIndex = 0;
				vertexArrtibs[1].offset = 12;
				vertexArrtibs[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[1].componentCount = 3;
				vertexArrtibs[1].regIndex = sceGxmProgramParameterGetResourceIndex(colorParam);

				vertexArrtibs[2].streamIndex = 0;
				vertexArrtibs[2].offset = 24;
				vertexArrtibs[2].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[2].componentCount = 2;
				vertexArrtibs[2].regIndex = sceGxmProgramParameterGetResourceIndex(textureParam);


				vertexstream[0].stride = sizeof(TextureNormalVertex);
				vertexstream[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
			}
				break;
			case Andromeda::Graphics::TextureColor:
			{
				Utils::Logger::Instance()->Log("LinkShader - TextureColor\n");
				
				paramsCount = 3;

				vertexArrtibs = new SceGxmVertexAttribute[paramsCount];
				vertexstream = new SceGxmVertexStream[1];

				//get attribute index
				const SceGxmProgramParameter *positionParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iPosition");
				const SceGxmProgramParameter *colorParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iColor");
				const SceGxmProgramParameter *textureParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iTexCoord");

				//position param
				vertexArrtibs[0].streamIndex = 0;
				vertexArrtibs[0].offset = 0;
				vertexArrtibs[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[0].componentCount = 3;
				vertexArrtibs[0].regIndex = sceGxmProgramParameterGetResourceIndex(positionParam);

				//color param
				vertexArrtibs[1].streamIndex = 0;
				vertexArrtibs[1].offset = 12;
				vertexArrtibs[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[1].componentCount = 3;
				vertexArrtibs[1].regIndex = sceGxmProgramParameterGetResourceIndex(colorParam);

				//textCoord
				vertexArrtibs[2].streamIndex = 0;
				vertexArrtibs[2].offset = 24;
				vertexArrtibs[2].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[2].componentCount = 2;
				vertexArrtibs[2].regIndex = sceGxmProgramParameterGetResourceIndex(textureParam);


				vertexstream[0].stride = sizeof(TextureColorVertex);
				vertexstream[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
			}
			break;
			case Andromeda::Graphics::ColorNormal:
			{

			}
			break;
			case Andromeda::Graphics::TextureColorNormal:
			{

			}
			break;
			case Andromeda::Graphics::NormalTextureWeighJoint:
			{
				Utils::Logger::Instance()->Log("LinkShader - NormalTextureWeighJoint\n");
				
				paramsCount = 5;

				vertexArrtibs = new SceGxmVertexAttribute[paramsCount];
				vertexstream = new SceGxmVertexStream[1];

				//get attribute index
				const SceGxmProgramParameter *positionParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iPosition");
				const SceGxmProgramParameter *normalParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iNormal");
				const SceGxmProgramParameter *textureParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iTexCoord");
				const SceGxmProgramParameter *weightParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iWeight");
				const SceGxmProgramParameter *jointParam = sceGxmProgramFindParameterByName(_gxmVertexProgram, "iJoint");

				//position param - 12
				vertexArrtibs[0].streamIndex = 0;
				vertexArrtibs[0].offset = 0;
				vertexArrtibs[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[0].componentCount = 3;
				vertexArrtibs[0].regIndex = sceGxmProgramParameterGetResourceIndex(positionParam);
				
				
				//color param - 12
				vertexArrtibs[1].streamIndex = 0;
				vertexArrtibs[1].offset = 12;
				vertexArrtibs[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[1].componentCount = 3;
				vertexArrtibs[1].regIndex = sceGxmProgramParameterGetResourceIndex(normalParam);

				//textCoord - 8
				vertexArrtibs[2].streamIndex = 0;
				vertexArrtibs[2].offset = 24;
				vertexArrtibs[2].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[2].componentCount = 2;
				vertexArrtibs[2].regIndex = sceGxmProgramParameterGetResourceIndex(textureParam);
				
				//weight - 16
				vertexArrtibs[3].streamIndex = 0;
				vertexArrtibs[3].offset = 32;
				vertexArrtibs[3].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
				vertexArrtibs[3].componentCount = 4;
				vertexArrtibs[3].regIndex = sceGxmProgramParameterGetResourceIndex(weightParam);
				
				//joint - 16
				vertexArrtibs[4].streamIndex = 0;
				vertexArrtibs[4].offset = 48;
				vertexArrtibs[4].format = SCE_GXM_ATTRIBUTE_FORMAT_U16;
				vertexArrtibs[4].componentCount = 4;
				vertexArrtibs[4].regIndex = sceGxmProgramParameterGetResourceIndex(jointParam);


				vertexstream[0].stride = sizeof(NormalTextureWeighJointVertex);
				vertexstream[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
			}
				break;
			default:
				break;
			}

			SceGxmBlendInfo	blendInfo;
			blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
			blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
			blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
			blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_SRC_ALPHA;
			blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;


			// create clear programs
			sceGxmShaderPatcherCreateVertexProgram(
				_shaderPatcher,
				_vertexProgramId,
				vertexArrtibs,
				paramsCount,
				vertexstream,
				1,
				&_vertexProgram);

			sceGxmShaderPatcherCreateFragmentProgram(
				_shaderPatcher,
				_fragmentProgramId,
				SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
				SCE_GXM_MULTISAMPLE_NONE,
				&blendInfo,
				_gxmVertexProgram,
				&_fragmentProgram);
				
			Utils::Logger::Instance()->Log("LinkShader - end\n");
				
			return 0;
		}

		void ShaderGxm::SetShaderPatcher(SceGxmShaderPatcher* shaderPatcher)
		{
			_shaderPatcher = shaderPatcher;
		}

		void ShaderGxm::SetGxmContext(SceGxmContext* gxmContext)
		{
			_gxmContext = gxmContext;
		}

		void ShaderGxm::Bind()
		{
			sceGxmSetVertexProgram(_gxmContext, _vertexProgram);
			sceGxmSetFragmentProgram(_gxmContext, _fragmentProgram);
		}		

		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, int val)
		{

		}

		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, float val)
		{
			void *uniform_buffer;
			const SceGxmProgramParameter* param;

			if (shaderType == FragmentShader)
			{
				param = GetUniform(_fragmentProgramId, name);
				sceGxmReserveFragmentDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			else
			{
				param = GetUniform(_vertexProgramId, name);
				sceGxmReserveVertexDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}

			sceGxmSetUniformDataF(uniform_buffer, param, 0, 1, &val);
		}

		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, glm::vec3& val)
		{
			void *uniform_buffer;
			const SceGxmProgramParameter* param;

			if (shaderType == FragmentShader)
			{
				param = GetUniform(_fragmentProgramId, name);
				sceGxmReserveFragmentDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			else
			{
				param = GetUniform(_vertexProgramId, name);
				sceGxmReserveVertexDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}

			sceGxmSetUniformDataF(uniform_buffer, param, 0, 3, glm::value_ptr(val));
		}

		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, glm::vec4& val)
		{
			void *uniform_buffer;
			const SceGxmProgramParameter* param;

			if (shaderType == FragmentShader)
			{
				param = GetUniform(_fragmentProgramId, name);
				sceGxmReserveFragmentDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			else
			{
				param = GetUniform(_vertexProgramId, name);
				sceGxmReserveVertexDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}

			sceGxmSetUniformDataF(uniform_buffer, param, 0, 4, glm::value_ptr(val));
		}

		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, glm::mat3& val)
		{
			void *uniform_buffer;
			const SceGxmProgramParameter* param;

			if (shaderType == FragmentShader)
			{
				param = GetUniform(_fragmentProgramId, name);
				sceGxmReserveFragmentDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			else
			{
				param = GetUniform(_vertexProgramId, name);
				sceGxmReserveVertexDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}

			sceGxmSetUniformDataF(uniform_buffer, param, 0, 9, glm::value_ptr(val));
		}

		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, glm::mat4& val)
		{
			void *uniform_buffer;
			const SceGxmProgramParameter* param;

			if (shaderType == FragmentShader)
			{
				param = GetUniform(_fragmentProgramId, name);
				sceGxmReserveFragmentDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			else
			{
				param = GetUniform(_vertexProgramId, name);
				sceGxmReserveVertexDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}

			sceGxmSetUniformDataF(uniform_buffer, param, 0, 16, glm::value_ptr(val));
		}
		
		void ShaderGxm::SetUniform(ShaderType shaderType, std::string name, AnimMat4* inputArray, unsigned int arrayLength)
		{
			void *uniform_buffer;
			const SceGxmProgramParameter* param;
			
			if (shaderType == FragmentShader)
			{
				param = GetUniform(_fragmentProgramId, name);
				sceGxmReserveFragmentDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			else
			{
				param = GetUniform(_vertexProgramId, name);
				sceGxmReserveVertexDefaultUniformBuffer(_gxmContext, &uniform_buffer);
			}
			
			sceGxmSetUniformDataF(uniform_buffer, param, 0, 16 * arrayLength, (float*)&inputArray[0]);
		}
		
		void ShaderGxm::Set(ShaderType shaderType, std::string name, std::vector<AnimMat4>& value)
		{
			SetUniform(shaderType, name, &value[0], (unsigned int)value.size());
		}

		const SceGxmProgramParameter * ShaderGxm::GetUniform(SceGxmShaderPatcherId programId, std::string paramName)
		{
			const SceGxmProgram			*program;
			const SceGxmProgramParameter	*param;
			int		ret;

			program = sceGxmShaderPatcherGetProgramFromId(programId);

			param = sceGxmProgramFindParameterByName(program, paramName.c_str());
			if (!param)
			{
				Utils::Logger::Instance()->Log("Error: GetUniform - no param %s\n", paramName.c_str());
				return 0;
			}

			ret = sceGxmProgramParameterGetCategory(param);
			if (ret != SCE_GXM_PARAMETER_CATEGORY_UNIFORM)
			{
				Utils::Logger::Instance()->Log("Error: GetUniform - wrong category %s\n", paramName.c_str());
				return 0;
			}				

			return param;
		}
	}
}


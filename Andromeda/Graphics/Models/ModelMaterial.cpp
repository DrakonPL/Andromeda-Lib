#include "Andromeda/Graphics/Models/ModelMaterial.h"
#include "Andromeda/Graphics/TextureManager.h"

namespace Andromeda
{
	namespace Graphics
	{
		ModelMaterial::ModelMaterial()
		{
			_ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
			_diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
			_specularColor = glm::vec3(0.0f, 0.0f, 0.0f);

			//textures
			_diffuseTexture = "";
			_specularTexture = "";
			_bumpTexture = "";

			_haveColor = false;
			_haveTexture = false;

			Image = nullptr;
		}

		ModelMaterial::~ModelMaterial()
		{

		}

		bool ModelMaterial::HaveColor()
		{
			return _haveColor;
		}

		bool ModelMaterial::HaveTexture()
		{
			return _haveTexture;
		}

		void ModelMaterial::SetColor(MaterialColorType type, glm::vec3 color)
		{
			switch (type)
			{
			case MaterialColorAmbient:
				_ambientColor = color;
				break;
			case MaterialColorDiffuse:
				_diffuseColor = color;
				_haveColor = true;
				break;
			case MaterialColorSpecular:
				_specularColor = color;
				break;
			}
		}

		void ModelMaterial::SetTextureName(MaterialTextureType type, std::string fileName)
		{
			switch (type)
			{
			case MaterialTextureDiffuse:
				_diffuseTexture = fileName;
				break;
			case MaterialTextureSpecular:
				_specularTexture = fileName;
				_haveTexture = true;
				break;
			case MaterialTextureBump:
				_bumpTexture = fileName;
				break;
			}
		}

		glm::vec3 ModelMaterial::GetColor(MaterialColorType type)
		{
			switch (type)
			{
			case MaterialColorAmbient:
				return _ambientColor;
				break;
			case MaterialColorDiffuse:
				return _diffuseColor;
				break;
			case MaterialColorSpecular:
				return _specularColor;
				break;
			}

			return glm::vec3(0.0f, 0.0f, 0.0f);
		}

		std::string ModelMaterial::GetTextureName(MaterialTextureType type)
		{
			switch (type)
			{
			case MaterialTextureDiffuse:
				return _diffuseTexture;
				break;
			case MaterialTextureSpecular:
				return _specularTexture;
				break;
			case MaterialTextureBump:
				return _bumpTexture;
				break;
			}

			return "";
		}
	}
}

#include <Andromeda/Graphics/Models/ModelMd5Utils.h>

#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream

namespace Andromeda
{
	namespace Graphics
	{
		// Remove the quotes from a string
		void ModelMd5Utils::RemoveQuotes(std::string& str)
		{
			size_t n;
			while ((n = str.find('\"')) != std::string::npos) str.erase(n, 1);
		}

		// Get's the size of the file in bytes.
		int ModelMd5Utils::GetFileLength(std::istream& file)
		{
			int pos = file.tellg();
			file.seekg(0, std::ios::end);
			int length = file.tellg();
			// Restore the position of the get pointer
			file.seekg(pos);

			return length;
		}

		void ModelMd5Utils::IgnoreLine(std::istream& file, int length)
		{
			file.ignore(length, '\n');
		}

		// Computes the W component of the quaternion based on the X, Y, and Z components.
		// This method assumes the quaternion is of unit length.
		void ModelMd5Utils::ComputeQuatW(glm::quat& quat)
		{
			float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);
			if (t < 0.0f)
			{
				quat.w = 0.0f;
			}
			else
			{
				quat.w = -sqrtf(t);
			}
		}
	}
}
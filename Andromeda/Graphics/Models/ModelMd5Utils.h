#ifndef ModelMd5Utils_H
#define ModelMd5Utils_H

#include <string>
#include <glm/gtc/quaternion.hpp>

namespace Andromeda
{
	namespace Graphics
	{
		class ModelMd5Utils
		{
		public:

			// Remove the quotes from a string
			static void RemoveQuotes(std::string& str);

			// Get's the size of the file in bytes.
			static int GetFileLength(std::istream& file);

			static void IgnoreLine(std::istream& file, int length);

			// Computes the W component of the quaternion based on the X, Y, and Z components.
			// This method assumes the quaternion is of unit length.
			static void ComputeQuatW(glm::quat& quat);
		};
	}
}



#endif
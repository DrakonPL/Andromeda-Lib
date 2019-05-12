#ifndef StdioDirectory_H
#define StdioDirectory_H

#include <Andromeda/FileSystem/BaseDirectory.h>
#include <dirent.h>

namespace Andromeda
{
	namespace FileSystem
	{
		class FileManager;

		class StdioDirectory : public BaseDirectory
		{
		private:

			DIR* _directory;

		protected:

			StdioDirectory();
			StdioDirectory(std::string dirName);

		public:

			bool Open() override;
			void Close() override;

			std::vector<std::string> GetFiles() override;
			std::vector<std::string> GetFiles(std::string extension) override;

			friend class FileManager;
		};
	}
}

#endif

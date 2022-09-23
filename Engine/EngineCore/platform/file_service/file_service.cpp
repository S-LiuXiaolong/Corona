#include "platform/file_service/file_service.h"

using namespace std;

namespace Diligent
{

	std::vector<std::filesystem::path> Diligent::CFileSystem::getFiles(const std::filesystem::path& directory)
	{
		vector<filesystem::path> files;
		for (auto const& directory_entry : filesystem::recursive_directory_iterator{ directory })
		{
			if (directory_entry.is_regular_file())
			{
				files.push_back(directory_entry);
			}
		}
		return files;
	}

}


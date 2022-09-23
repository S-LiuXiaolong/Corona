#pragma once

#include <filesystem>
#include <vector>

namespace Diligent
{
	class CFileSystem
	{
	public:
		std::vector<std::filesystem::path> getFiles(const std::filesystem::path& directory);
	};
}
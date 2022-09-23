#pragma once

#include <memory>
#include <string>

namespace Diligent
{
	class CFileSystem;
	class CWindowSystem;
	class CRenderSystem;

	class RuntimeGlobalContext
	{
	public:
		void StartSystems(const std::string& config_file_path);
		void shutdownSystems();

	public:
		std::shared_ptr<CFileSystem> m_file_system;
		std::shared_ptr<CWindowSystem> m_window_system;
		std::shared_ptr<CRenderSystem> m_render_system;
	};

	extern RuntimeGlobalContext g_runtime_global_context;
}
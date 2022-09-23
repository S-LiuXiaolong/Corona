#pragma once

#include <memory>
#include <string>

namespace Diligent
{
	class FileSystem;
	class WindowSystem;
	class RenderSystem;

	class RuntimeGlobalContext
	{
	public:
		void StartSystems(const std::string& config_file_path);
		void shutdownSystems();

	public:
		std::shared_ptr<FileSystem> m_file_system;
		std::shared_ptr<WindowSystem> m_window_system;
		std::shared_ptr<RenderSystem> m_render_system;
	};

	extern RuntimeGlobalContext g_runtime_global_context;
}
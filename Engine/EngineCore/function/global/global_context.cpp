#include "function/global/global_context.h"

#include "engine.h"

#include "function/platform/file_service/file_service.h"
#include "function/render/window_system.h"
#include "function/render/render_system.h"

namespace Diligent
{
	RuntimeGlobalContext g_runtime_global_context;

	void RuntimeGlobalContext::StartSystems(const std::string& config_file_path)
	{
		m_file_system = std::make_shared<FileSystem>();

		m_window_system = std::make_shared<WindowSystem>();
		WindowCreateInfo window_create_info;
		m_window_system->initialize(window_create_info);

		m_render_system = std::make_shared<RenderSystem>();
		m_render_system->initialize();
	}

	void RuntimeGlobalContext::shutdownSystems()
	{
		m_render_system->reset();
	}

}
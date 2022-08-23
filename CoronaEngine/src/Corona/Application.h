#pragma once

#include "Core.h"

namespace Corona {

	class CORONA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defines in CLIENT
	Application* CreateApplication();

}



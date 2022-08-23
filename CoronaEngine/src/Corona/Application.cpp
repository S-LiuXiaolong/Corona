#include "Application.h"
#include <stdio.h>

namespace Corona {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (true)
		{
			printf("Hello!\n");
		}
	}

}
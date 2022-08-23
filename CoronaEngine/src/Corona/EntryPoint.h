#pragma once

#ifdef CR_PLATFORM_WINDOWS

extern Corona::Application* Corona::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Corona::CreateApplication();
	app->Run();
	delete app;
}

#endif

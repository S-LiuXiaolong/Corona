#pragma once

#ifdef CR_PLATFORM_WINDOWS

//int main(int argc, char** argv)
//{
//	auto app = Corona::CreateApplication();
//	app->Run();
//	delete app;
//}

#include "Corona/Sandbox.h"
#include <windows.h>

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//try
	//{
	//	Sandbox theApp(hInstance);
	//	if (!theApp.Initialize())
	//		return 0;

	//	return theApp.Run();
	//}
	//catch (DxException& e)
	//{
	//	MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
	//	return 0;
	//}
	Corona::Log::Init();
	Corona::Sandbox theApp(hInstance);
	if (!theApp.Initialize())
		return 0;

	return theApp.Run();
}

#endif

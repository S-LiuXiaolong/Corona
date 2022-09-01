#pragma once

#ifdef CR_PLATFORM_WINDOWS

#include "GameApp.h"

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

	GameApp* app = new GameApp();
	GameCore::RunApplication(*app, L"CrossGate", hInstance, nCmdShow);
	delete app;
	return 0;
}

#endif

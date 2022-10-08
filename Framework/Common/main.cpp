#include <stdio.h>
#include <chrono>
#include <thread>
#include "BaseApplication.h"

using namespace Corona;

int main(int argc, char** argv)
{
	int ret;

    g_pApp->SetCommandLineParameters(argc, argv);

	if ((ret = g_pApp->Initialize()) != 0) {
		printf("App Initialize failed, will exit now.");
		return ret;
	}

	// if ((ret = g_pApp->LoadScene()) != 0) {
	// 	printf("Load Scene failed, will exit now.");
	// 	return ret;
	// }

	while (!g_pApp->IsQuit()) {
        g_pMemoryManager->Tick();
        g_pAssetLoader->Tick();
        g_pSceneManager->Tick();
        g_pGraphicsManager->Tick();
		g_pApp->Tick();
	}

	g_pApp->Finalize();

	return 0;
}
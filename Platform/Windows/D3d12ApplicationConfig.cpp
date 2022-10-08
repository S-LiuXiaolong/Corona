#include "D3d12Application.h"
#include "D3d/D3d12GraphicsManager.h"
#include <tchar.h>

namespace Corona
{
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 960, 540, _T("Corona Engine (Windows)"));
	IApplication* g_pApp                = static_cast<IApplication*>(new D3d12Application(config));
    GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new D3d12GraphicsManager);
    MemoryManager*   g_pMemoryManager   = static_cast<MemoryManager*>(new MemoryManager);
    AssetLoader*     g_pAssetLoader     = static_cast<AssetLoader*>(new AssetLoader);
    SceneManager*    g_pSceneManager    = static_cast<SceneManager*>(new SceneManager);
}


#include "WindowsApplication.h"
#include "D3d/D3d12GraphicsManager.h"
#include "MemoryManager.h"
#include <tchar.h>

namespace Corona
{
    GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 960, 540, _T("Game Engine From Scratch (Windows)"));
	IApplication* g_pApp                = static_cast<IApplication*>(new WindowsApplication(config));
    GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new D3d12GraphicsManager);
    MemoryManager*   g_pMemoryManager   = static_cast<MemoryManager*>(new MemoryManager);

}


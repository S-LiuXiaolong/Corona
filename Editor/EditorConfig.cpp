#include "GfxConfiguration.h"
#include "EditorLogic.h"
#include "PhysicsManager.h"

namespace Corona 
{
    GfxConfiguration config(8, 8, 8, 8, 24, 8, 0, 1920, 1080, "Corona Editor");
    IGameLogic*       g_pGameLogic       = static_cast<IGameLogic*>(new EditorLogic);
    // IPhysicsManager*  g_pPhysicsManager  = static_cast<IPhysicsManager*>(new PhysicsManager);
}
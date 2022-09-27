#include "BaseApplication.h"

namespace Corona
{
    GfxConfiguration cfg;
    BaseApplication g_App(cfg);
    IApplication *g_pApp = &g_App;
}
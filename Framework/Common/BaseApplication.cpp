#include "BaseApplication.h"

namespace Corona
{
    bool BaseApplication::m_bQuit = false;

    BaseApplication::BaseApplication(GfxConfiguration& cfg)
        :m_Config(cfg)
    {
    }

    // Parse command line, read configuration, initialize all submodules
    int BaseApplication::Initialize()
    {
        int result = 0;

        std::cout << m_Config;

        return result;
    }

    // Finalize all submodules and clean up all runtime temporary files.
    void BaseApplication::Finalize()
    {
    }

    // One cycle of the main loop
    void BaseApplication::Tick()
    {
    }

    bool BaseApplication::IsQuit()
    {
        return m_bQuit;
    }
}

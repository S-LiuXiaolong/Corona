#pragma once
#include "IApplication.h"
// #include "GfxConfiguration.h"
#include "GraphicsManager.h"
#include "MemoryManager.h"
#include "AssetLoader.h"
#include "SceneManager.h"
#include "InputManager.h"

namespace Corona
{
    class BaseApplication : implements IApplication
    {
    public:
        BaseApplication(GfxConfiguration& cfg);
        virtual int Initialize();
        virtual void Finalize();
        // One cycle of the main loop
        virtual void Tick();
        
        virtual bool IsQuit();

        inline GfxConfiguration& GetConfiguration() { return m_Config; };

        virtual void SetCommandLineParameters(int argc, char** argv);

        virtual int LoadScene();

        virtual void OnDraw() {};
    
    protected:
        // Flag is need quit the main loop of the application
        static bool        m_bQuit;
        GfxConfiguration   m_Config;
        int                m_nArgC;
        char**             m_ppArgV;

    private:
        // hide the default construct to enforce a configuration
        BaseApplication() {};
    };
}
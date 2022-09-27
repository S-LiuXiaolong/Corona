#pragma once
#include "IApplication.h"
#include "GfxConfiguration.h"

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
    
    protected:
        // Flag is need quit the main loop of the application
        static bool m_bQuit;
        GfxConfiguration m_Config;

    private:
        // hide the default construct to enforce a configuration
        BaseApplication(){};
    };
}
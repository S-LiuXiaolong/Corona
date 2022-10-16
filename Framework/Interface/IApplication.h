#pragma once
#include "IRuntimeModule.h"
#include "GfxConfiguration.h"

namespace Corona
{
    Interface IApplication : implements IRuntimeModule
    {
    public:
        virtual int Initialize() = 0;
        virtual void Finalize() = 0;
        // One cycle of the main loop
        virtual void Tick() = 0;

        virtual void SetCommandLineParameters(int argc, char** argv) = 0;
        virtual int  GetCommandLineArgumentsCount() const = 0;
        virtual const char* GetCommandLineArgument(int index) const = 0;

        virtual bool IsQuit() = 0;

        virtual int LoadScene() = 0;

        virtual void OnDraw() = 0;

        virtual GfxConfiguration& GetConfiguration() = 0;
    };

    extern IApplication* g_pApp;
}
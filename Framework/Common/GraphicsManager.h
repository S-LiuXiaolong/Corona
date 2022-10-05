#pragma once
#include "IRuntimeModule.h"

namespace Corona
{
    class GraphicsManager : implements IRuntimeModule
    {
    public:
        virtual ~GraphicsManager() {};

        virtual int Initialize();
	    virtual void Finalize();

	    virtual void Tick();
    };

    extern GraphicsManager* g_pGraphicsManager;
}
#pragma once

#include "Interface.h"

namespace Corona
{
    Interface IRuntimeModule
    {
    public:
        virtual ~IRuntimeModule() {};

        virtual int Initialize() = 0;
        virtual void Finalize() = 0;

        virtual void Tick() = 0;
        
#ifdef _DEBUG
	    virtual void DrawDebugInfo() {};
#endif
    };
}
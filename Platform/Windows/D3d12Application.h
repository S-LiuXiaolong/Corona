#pragma once
#include "WindowsApplication.h"

namespace Corona {
    class D3d12Application : public WindowsApplication 
    {
        public:
            using WindowsApplication::WindowsApplication;
            void Tick();
    };
}

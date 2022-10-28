#pragma once
#include <iostream>
#include "Interface.h"
#include "GfxStructures.h"

namespace Corona {
    Interface IDrawPass
    {
    public:
        IDrawPass() = default;
        virtual ~IDrawPass() {};

        virtual void Draw(Frame& frame) = 0;
    };
}

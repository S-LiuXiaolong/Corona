#pragma once 
#include "IDrawPass.h"

namespace Corona
{
    class ForwardRenderPass : implements IDrawPass
    {
    public:
        ~ForwardRenderPass() = default;
        void Draw(Frame& frame) final;
    };
}
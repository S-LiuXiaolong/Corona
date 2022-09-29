#pragma once
#include "Interface.h"
#include "Image.h"
#include "Buffer.h"

namespace Corona
{
    Interface ImageParser
    {
    public:
        virtual Image Parse(const Buffer& buf) = 0;
    };
}
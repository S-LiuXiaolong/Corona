#pragma once
#include "Interface.h"
#include "Image.h"
#include "Buffer.h"

namespace Corona
{
    Interface ImageParser
    {
    public:
        virtual ~ImageParser() = default;
        virtual Image Parse(Buffer & buf) = 0;
    };
}
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "crossguid/guid.hpp"
#include "Image.h"

namespace Corona
{
    namespace details
    {
        constexpr int32_t i32(const char* s, int32_t v)
        {
            return *s ? i32(s + 1, v * 256 + *s) : v;
        }
    }

    constexpr int32_t operator "" _i32(const char* s, size_t)
    {
        return details::i32(s, 0);
    }

    using namespace xg;
    class BaseSceceObject
    {
    protected:
        Guid m_Guid;
        SceneObjectType m_Type;
    };
}
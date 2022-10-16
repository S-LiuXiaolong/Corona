#pragma once
#include "Ability.h"

namespace Corona {
    template<typename T>
    Ability Animatable
    {
        typedef const T ParamType;
        virtual void Update(ParamType param) = 0;
    };
}

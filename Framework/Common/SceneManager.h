#pragma once
#include "geommath.h"
#include "IRuntimeModule.h"
#include "SceneNode.h"

namespace Corona
{
    class SceneManager : implements IRuntimeModule
    {
    public:
        virtual ~SceneManager();

        virtual int Initialize();
        virtual void Finalize();

        virtual void Tick();

    protected:
        SceneEmptyNode m_RootNode;
    };
}
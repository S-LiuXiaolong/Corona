#pragma once
#include <memory>
#include <string>
#include "Interface.h"
#include "SceneNode.h"

namespace Corona
{
    Interface SceneParser
    {
    public:
        virtual std::unique_ptr<BaseSceneNode> Parse(const std::string& buf) = 0;
    };
}
#pragma once
#include <memory>
#include <string>
#include "Interface.h"
#include "SceneNode.h"
#include "AssetLoader.h"
#include "Scene.h"

namespace Corona
{
    Interface SceneParser
    {
    public:
        virtual std::shared_ptr<Scene> Parse(const std::string& FileName) = 0;
    };
}
#pragma once
#include <memory>
#include <string>
#include "Interface.h"
#include "glTFSceneObject.h"

namespace Corona
{
    Interface SceneParser
    {
    public:
        virtual std::unique_ptr<Model> LoadFromFile(const std::string& buf) {};
    };
}
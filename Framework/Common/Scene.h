#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "SceneNode.h"

namespace Corona
{
    class Scene 
    {
    public:
        Scene(std::string scene_name) : name(scene_name) {};
        ~Scene() = default;

    public:
        std::string name;
        std::unique_ptr<Model> pModel;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectCamera>>      Cameras;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMesh>>    Geometries;
        // std::unordered_map<std::string, std::shared_ptr<SceneObjectLight>>       Lights;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMaterial>>    Materials;

        const std::shared_ptr<SceneObjectCamera> GetCamera(const std::string& key) const;
        const std::shared_ptr<SceneObjectCamera> GetFirstCamera() const;
        const std::shared_ptr<SceneObjectCamera> GetNextCamera() const;

        const std::shared_ptr<SceneObjectMesh> GetGeometry(const std::string& key) const;
        const std::shared_ptr<SceneObjectMesh> GetFirstGeometry() const;
        const std::shared_ptr<SceneObjectMesh> GetNextGeometry() const;
        
        // const std::shared_ptr<SceneObjectLight> GetLight(std::string key) const;
        // const std::shared_ptr<SceneObjectLight> GetFirstLight() const;
        // const std::shared_ptr<SceneObjectLight> GetNextLight() const;

        const std::shared_ptr<SceneObjectMaterial> GetMaterial(std::string key) const;
        const std::shared_ptr<SceneObjectMaterial> GetFirstMaterial() const;
        const std::shared_ptr<SceneObjectMaterial> GetNextMaterial() const;

    };
}
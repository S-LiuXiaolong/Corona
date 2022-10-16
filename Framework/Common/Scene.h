#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "SceneNode.h"

namespace Corona
{
    class Scene
    {
    private:
        std::shared_ptr<SceneObjectMaterial> m_pDefaultMaterial;

    public:
        std::string name;
        std::vector<std::weak_ptr<BaseSceneNode>> RootNodes;
        std::unordered_map<std::string, std::shared_ptr<BaseSceneNode>> LUT_Name_LinearNodes;

        std::unordered_map<std::string, std::shared_ptr<SceneObjectCamera>> Cameras;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectLight>> Lights;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMaterial>> Materials;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMesh>> Geometries;

        // TODO: why weak_ptr here ?
        std::unordered_multimap<std::string, std::weak_ptr<SceneCameraNode>> CameraNodes;
        std::unordered_multimap<std::string, std::weak_ptr<SceneLightNode>> LightNodes;
        std::unordered_multimap<std::string, std::weak_ptr<SceneGeometryNode>> GeometryNodes;
        // std::unordered_map<std::string, std::weak_ptr<SceneBoneNode>>               BoneNodes;

        // std::vector<std::weak_ptr<BaseSceneNode>> AnimatableNodes;

    public:
        Scene()
        {
            m_pDefaultMaterial = std::make_shared<SceneObjectMaterial>("default");
        }

        Scene(const std::string &scene_name) : name(scene_name)
        {
        }

        ~Scene() = default;

        const std::shared_ptr<SceneObjectCamera> GetCamera(const std::string &key) const;
        const std::shared_ptr<SceneCameraNode> GetFirstCameraNode() const;

        const std::shared_ptr<SceneObjectLight> GetLight(const std::string &key) const;
        const std::shared_ptr<SceneLightNode> GetFirstLightNode() const;

        const std::shared_ptr<SceneObjectMesh> GetGeometry(const std::string &key) const;
        const std::shared_ptr<SceneGeometryNode> GetFirstGeometryNode() const;

        const std::shared_ptr<SceneObjectMaterial> GetMaterial(const std::string &key) const;
        const std::shared_ptr<SceneObjectMaterial> GetFirstMaterial() const;

        void LoadResource(void);
    };
}
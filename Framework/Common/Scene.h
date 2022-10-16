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
        std::shared_ptr<BaseSceneNode> SceneGraph;

        std::unordered_map<std::string, std::shared_ptr<SceneObjectCamera>> Cameras;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectLight>> Lights;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMaterial>> Materials;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMesh>> Geometries;

        std::unordered_multimap<std::string, std::weak_ptr<SceneCameraNode>> CameraNodes;
        std::unordered_multimap<std::string, std::weak_ptr<SceneLightNode>> LightNodes;
        std::unordered_multimap<std::string, std::weak_ptr<SceneGeometryNode>> GeometryNodes;
        // std::unordered_map<std::string, std::weak_ptr<SceneBoneNode>>               BoneNodes;

        std::vector<std::weak_ptr<BaseSceneNode>> AnimatableNodes;

        std::unordered_map<std::string, std::weak_ptr<SceneGeometryNode>> LUT_Name_GeometryNode;

    public:
        Scene()
        {
            m_pDefaultMaterial = std::make_shared<SceneObjectMaterial>("default");
        }

        Scene(const std::string &scene_name) : name(scene_name), SceneGraph(new BaseSceneNode(scene_name))
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
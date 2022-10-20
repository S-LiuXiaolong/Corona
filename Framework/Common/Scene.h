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
        std::vector<std::weak_ptr<SceneNode>> RootNodes;
        std::unordered_map<std::string, std::shared_ptr<SceneNode>> LUT_Name_LinearNodes;

        // the strings here are hash values formed by crossguid
        std::unordered_map<std::string, std::shared_ptr<SceneObjectCamera>> Cameras;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectLight>> Lights;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMaterial>> Materials;
        std::unordered_map<std::string, std::shared_ptr<SceneObjectMesh>> Geometries;

        // For binding meshes and materials
        std::vector<std::weak_ptr<SceneObjectMaterial>> LinearMaterials;

        // TODO: why weak_ptr here ?
        std::unordered_map<std::string, std::weak_ptr<SceneCameraNode>> CameraNodes;
        std::unordered_map<std::string, std::weak_ptr<SceneNode>> LightNodes;
        std::unordered_map<std::string, std::weak_ptr<SceneNode>> GeometryNodes;
        // std::unordered_map<std::string, std::weak_ptr<SceneBoneNode>>               BoneNodes;

        // std::vector<std::weak_ptr<BaseSceneNode>> AnimatableNodes;

    public:
        Scene() {};
        Scene(const std::string &scene_name) : name(scene_name)
        {
        }

        ~Scene() = default;

        const std::shared_ptr<SceneObjectCamera> GetCamera(const std::string &key) const;
        const std::shared_ptr<SceneNode> GetFirstCameraNode() const;

        const std::shared_ptr<SceneObjectLight> GetLight(const std::string &key) const;
        const std::shared_ptr<SceneNode> GetFirstLightNode() const;

        const std::shared_ptr<SceneObjectMesh> GetGeometry(const std::string &key) const;
        const std::shared_ptr<SceneNode> GetFirstGeometryNode() const;

        const std::shared_ptr<SceneObjectMaterial> GetMaterial(const std::string &key) const;
        const std::shared_ptr<SceneObjectMaterial> GetFirstMaterial() const;

        // void LoadResource(void);
    };
}
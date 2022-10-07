#include "Scene.h"

namespace Corona
{
    const std::shared_ptr<SceneObjectCamera> Scene::GetCamera(const std::string& key) const
    {
        auto i = Cameras.find(key);
        if (i == Cameras.end())
            return nullptr;
        else
            return i->second;
    }

    // const std::shared_ptr<SceneObjectLight> Scene::GetLight(const std::string& key) const
    // {
    //     auto i = Lights.find(key);
    //     if (i == Lights.end())
    //         return nullptr;
    //     else
    //         return i->second;
    // }

    const std::shared_ptr<SceneObjectMesh> Scene::GetGeometry(const std::string& key) const
    {
        auto i = Geometries.find(key);
        if (i == Geometries.end())
            return nullptr;
        else
            return i->second;
    }

    // const std::shared_ptr<SceneObjectMaterial> Scene::GetMaterial(const std::string& key) const
    // {
    //     auto i = Materials.find(key);
    //     if (i == Materials.end())
    //         return m_pDefaultMaterial;
    //     else
    //         return i->second;
    // }

    // const std::shared_ptr<SceneObjectMaterial> Scene::GetFirstMaterial() const
    // {
    //     return (Materials.empty()? nullptr : Materials.cbegin()->second);
    // }

    // const std::shared_ptr<SceneObjectMaterial> Scene::GetNextMaterial() const
    // {
    //     static thread_local auto _it = Materials.cbegin();
    //     if (_it == Materials.cend()) return nullptr;
    //     return ((++_it == Materials.cend()) ? nullptr : _it->second);
    // }

    const std::shared_ptr<SceneObjectMesh> Scene::GetFirstGeometry() const
    {
        return (Geometries.empty()? 
                nullptr 
                : Geometries.cbegin()->second);
    }

    const std::shared_ptr<SceneObjectMesh> Scene::GetNextGeometry() const
    {
        static thread_local auto _it = Geometries.cbegin();
        if (_it == Geometries.cend()) return nullptr;
        return ((++_it == Geometries.cend()) ? 
                nullptr 
                : _it->second);
    }

    // const std::shared_ptr<SceneLightNode> Scene::GetFirstLightNode() const
    // {
    //     return (LightNodes.empty()? 
    //             nullptr 
    //             : LightNodes.cbegin()->second);
    // }

    // const std::shared_ptr<SceneLightNode> Scene::GetNextLightNode() const
    // {
    //     static thread_local auto _it = LightNodes.cbegin();
    //     if (_it == LightNodes.cend()) return nullptr;
    //     return ((++_it == LightNodes.cend()) ? 
    //             nullptr 
    //             : _it->second);
    // }

    const std::shared_ptr<SceneObjectCamera> Scene::GetFirstCamera() const
    {
        return (Cameras.empty()? 
                nullptr 
                : Cameras.cbegin()->second);
    }

    const std::shared_ptr<SceneObjectCamera> Scene::GetNextCamera() const
    {
        static thread_local auto _it = Cameras.cbegin();
        if (_it == Cameras.cend()) return nullptr;
        return ((++_it == Cameras.cend()) ? 
                nullptr 
                : _it->second);
    }

    // void Scene::LoadResource()
    // {
    //     for (auto material : Materials)
    //     {
    //         material.second->LoadTextures();
    //     }
    // }

}


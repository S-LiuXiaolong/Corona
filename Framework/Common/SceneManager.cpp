#include "SceneManager.h"
#include "AssetLoader.h"
#include "GLTF.h"

namespace Corona
{
    SceneManager::~SceneManager()
    {
    }

    int SceneManager::Initialize()
    {
        int result = 0;

        m_pScene = std::make_shared<Scene>();

        return result;
    }

    void SceneManager::Finalize()
    {
    }

    void SceneManager::Tick()
    {
        if (m_bDirtyFlag)
        {
            // m_bDirtyFlag = !(m_bRenderingQueued && m_bPhysicalSimulationQueued && m_bAnimationQueued);
            m_bDirtyFlag = !(m_bRenderingQueued);
        }
    }

    int SceneManager::LoadScene(std::string scene_file_name)
    {
        // now we only has gltf scene parser, call it directly
        if(LoadGltfScene(scene_file_name))
        {
            // m_pScene->LoadResource();
            m_bDirtyFlag = true;
            m_bRenderingQueued = false;
            m_bPhysicalSimulationQueued = false;
            return 0;
        }
        return 0;
    }

    void SceneManager::ResetScene()
    {
        m_bDirtyFlag = true;
    }

    bool SceneManager::LoadGltfScene(std::string gltf_scene_file_name)
    {
        m_pScene->name = gltf_scene_file_name;

        GltfParser gltf_parser;
        m_pScene = gltf_parser.Parse(gltf_scene_file_name);

        if (!m_pScene) {
            return false;
        }

        return true;
    }

    Scene& SceneManager::GetSceneForRendering()
    {
        // TODO: we should perform CPU scene crop at here
        return *m_pScene;
    }

    // Scene& SceneManager::GetSceneForPhysicalSimulation()
    // {
    //     // TODO: we should perform CPU scene crop at here
    //     return *m_pScene;
    // }

    bool SceneManager::IsSceneChanged()
    {
        return m_bDirtyFlag;
    }

    void SceneManager::NotifySceneIsRenderingQueued()
    {
        m_bRenderingQueued = true;
    }

    void SceneManager::NotifySceneIsPhysicalSimulationQueued()
    {
        m_bPhysicalSimulationQueued = true;
    }

    void SceneManager::NotifySceneIsAnimationQueued()
    {
        m_bAnimationQueued = true;
    }

    // std::weak_ptr<BaseSceneNode> SceneManager::GetRootNode()
    // {
    //     return m_pScene->SceneGraph;
    // }

    // std::weak_ptr<SceneGeometryNode> SceneManager::GetSceneNode(std::string name)
    // {
    //     auto it = m_pScene->LUT_Name_LinearNodes.find(name);
    //     if (it != m_pScene->LUT_Name_LinearNodes.end())
    //         return it->second;
    //     else
    //         return std::weak_ptr<SceneGeometryNode>();
    // }

    // std::weak_ptr<SceneObjectMesh> SceneManager::GetSceneGeometryObject(std::string key)
    // {
    //     return m_pScene->Geometries.find(key)->second;
    // }

}
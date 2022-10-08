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
        return result;
    }

    void SceneManager::Finalize()
    {
    }

    void SceneManager::Tick()
    {
    }

    int SceneManager::LoadScene(std::string scene_file_name)
    {
        // now we only has gltf scene parser, call it directly
        LoadGltfScene(scene_file_name);
        return 0;
    }

    const Scene& SceneManager::GetSceneForRendering()
    {
        return *m_pScene;
    }

    bool SceneManager::IsSceneChanged()
    {
        return m_bDirtyFlag;
    }

    void SceneManager::LoadGltfScene(std::string gltf_scene_file_name)
    {
        std::string gltf_text = g_pAssetLoader->GetFilePath(gltf_scene_file_name.c_str());

        m_pScene = std::make_unique<Scene>(gltf_text);
        GltfParser gltf_parser;
        gltf_parser.Parse(m_pScene);
    }
}
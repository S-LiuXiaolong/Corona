#pragma once
#include "geommath.h"
#include "IRuntimeModule.h"
#include "SceneParser.h"

namespace Corona
{
    class SceneManager : implements IRuntimeModule
    {
    public:
        virtual ~SceneManager();

        virtual int Initialize();
        virtual void Finalize();

        virtual void Tick();

        int LoadScene(std::string scene_file_name);

        const Scene& GetSceneForRendering();

        bool IsSceneChanged();

    protected:
        void LoadGltfScene(std::string gltf_scene_file_name);

    protected:
        std::unique_ptr<Scene> m_pScene;
        bool m_bDirtyFlag = false;
    };

    extern SceneManager* g_pSceneManager;
}
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

        void LoadScene(std::string scene_file_name);

        const Scene& GetSceneForRendering();

    protected:
        void LoadGltfScene(std::string gltf_scene_file_name);

    protected:
        std::unique_ptr<Scene> m_pScene;
    };

    extern SceneManager* g_pSceneManager;
}
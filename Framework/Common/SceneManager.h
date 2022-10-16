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
        void NotifySceneIsRenderingQueued();
        void NotifySceneIsPhysicalSimulationQueued();
        void NotifySceneIsAnimationQueued();

        const Scene& GetSceneForRendering();
        // const Scene& GetSceneForPhysicalSimulation();

        void ResetScene();

        // std::weak_ptr<BaseSceneNode> GetRootNode();
        // std::weak_ptr<SceneGeometryNode> GetSceneNode(std::string name);
        // std::weak_ptr<SceneObjectMesh> GetSceneGeometryObject(std::string key);

    protected:
        bool LoadGltfScene(std::string gltf_scene_file_name);

    protected:
        std::shared_ptr<Scene> m_pScene;
        bool m_bRenderingQueued = false;
        bool m_bPhysicalSimulationQueued = false;
        bool m_bAnimationQueued = false;
        bool m_bDirtyFlag = false;
    };

    extern SceneManager* g_pSceneManager;
}
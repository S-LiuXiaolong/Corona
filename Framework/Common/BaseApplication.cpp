#include "BaseApplication.h"

using namespace std;

namespace Corona
{
    bool BaseApplication::m_bQuit = false;

    BaseApplication::BaseApplication(GfxConfiguration &cfg)
        : m_Config(cfg)
    {
    }

    // Parse command line, read configuration, initialize all submodules
    int BaseApplication::Initialize()
    {
        int ret = 0;

        cout << m_Config;

        if ((ret = g_pMemoryManager->Initialize()) != 0)
        {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        if ((ret = g_pAssetLoader->Initialize()) != 0)
        {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        if ((ret = g_pSceneManager->Initialize()) != 0)
        {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        if ((ret = g_pGraphicsManager->Initialize()) != 0)
        {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        if ((ret = g_pInputManager->Initialize()) != 0)
        {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        if ((ret = g_pPhysicsManager->Initialize()) != 0)
        {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        if ((ret = g_pAnimationManager->Initialize()) != 0)
        {
            cerr << "Failed. err =" << ret;
            return ret;
        }

        if ((ret = g_pGameLogic->Initialize()) != 0)
        {
            cerr << "Failed. err =" << ret;
            return ret;
        }

#ifdef DEBUG
        if ((ret = g_pDebugManager->Initialize()) != 0)
        {
            cerr << "Failed. err =" << ret;
            return ret;
        }
#endif

        return ret;
    }

    // Finalize all submodules and clean up all runtime temporary files.
    void BaseApplication::Finalize()
    {
#ifdef _DEBUG
        g_pDebugManager->Finalize();
#endif
        g_pGameLogic->Finalize();
        g_pAnimationManager->Finalize();
        g_pInputManager->Finalize();
        g_pGraphicsManager->Finalize();
        g_pPhysicsManager->Finalize();
        g_pSceneManager->Finalize();
        g_pAssetLoader->Finalize();
        g_pMemoryManager->Finalize();
    }

    // One cycle of the main loop
    void BaseApplication::Tick()
    {
        g_pMemoryManager->Tick();
        g_pAssetLoader->Tick();
        g_pSceneManager->Tick();
        g_pInputManager->Tick();
        g_pPhysicsManager->Tick();
        g_pAnimationManager->Tick();
        g_pGraphicsManager->Tick();
        g_pGameLogic->Tick();
#ifdef _DEBUG
        g_pDebugManager->Tick();
#endif
    }

    bool BaseApplication::IsQuit()
    {
        return m_bQuit;
    }

    void BaseApplication::SetCommandLineParameters(int argc, char** argv)
    {
        m_nArgC = argc;
        m_ppArgV = argv;
    }

    int  BaseApplication::GetCommandLineArgumentsCount() const
    {
        return m_nArgC;
    }

    const char* BaseApplication::GetCommandLineArgument(int index) const
    {
        assert(index < m_nArgC);
        return m_ppArgV[index];
    }

    // int BaseApplication::LoadScene()
    // {
    //     int ret;

    //     // string scene_file_name = "Scene/Suzanne/Suzanne.gltf";
    //     string scene_file_name = "Scene/ABeautifulGame/ABeautifulGame.gltf";
    //     // string scene_file_name = "Scene/DamagedHelmet/DamagedHelmet.gltf";
    //     // string scene_file_name = "Scene/Box.glb";
    //     if (m_nArgC > 1)
    //     {
    //         scene_file_name = m_ppArgV[1];
    //     }

    //     cerr << "Load Scene(" << scene_file_name << "): ";
    //     if ((ret = g_pSceneManager->LoadScene(scene_file_name)) != 0)
    //     {
    //         cerr << "Failed. err = " << ret;
    //         return ret;
    //     }

    //     cerr << "Success";

    //     return 0;
    // }
}

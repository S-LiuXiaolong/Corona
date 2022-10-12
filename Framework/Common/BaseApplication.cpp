#include "BaseApplication.h"

using namespace std;

namespace Corona
{
    bool BaseApplication::m_bQuit = false;

    BaseApplication::BaseApplication(GfxConfiguration& cfg)
        :m_Config(cfg)
    {
    }

    // Parse command line, read configuration, initialize all submodules
    int BaseApplication::Initialize()
    {
        int ret = 0;

        cout << m_Config;

        cerr << "Initialize Memory Manager: ";
        if ((ret = g_pMemoryManager->Initialize()) != 0) {
            cerr << "Failed. err = " << ret;
            return ret;
        }
        cerr << "Success" << endl;

        cerr << "Initialize Asset Loader: ";
        if ((ret = g_pAssetLoader->Initialize()) != 0) {
            cerr << "Failed. err = " << ret;
            return ret;
        }
        cerr << "Success" << endl;

        cerr << "Initialize Scene Manager: ";
        if ((ret = g_pSceneManager->Initialize()) != 0) {
            cerr << "Failed. err = " << ret;
            return ret;
        }
        cerr << "Success" << endl;

        if ((ret = LoadScene()) != 0) {
            printf("Load Scene failed, will exit now.");
            return ret;
        }

        cerr << "Initialize Graphics Manager: ";
        if ((ret = g_pGraphicsManager->Initialize()) != 0) {
            cerr << "Failed. err = " << ret;
            return ret;
        }
        cerr << "Success" << endl;

        return ret;
    }

    // Finalize all submodules and clean up all runtime temporary files.
    void BaseApplication::Finalize()
    {
        g_pGraphicsManager->Finalize();
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
        g_pGraphicsManager->Tick();
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

    int BaseApplication::LoadScene()
    {
        int ret;

        // string scene_file_name = "Scene/Suzanne/Suzanne.gltf";
        // string scene_file_name = "Scene/BoomBoxWithAxes/BoomBoxWithAxes.gltf";
        string scene_file_name = "Scene/DamagedHelmet/DamagedHelmet.gltf";
        if (m_nArgC > 1) {
            scene_file_name = m_ppArgV[1];
        }

        cerr << "Load Scene(" << scene_file_name << "): ";
        if ((ret = g_pSceneManager->LoadScene(scene_file_name)) != 0) {
            cerr << "Failed. err = " << ret;
            return ret;
        }

        cerr << "Success";

        return 0;
    }
}

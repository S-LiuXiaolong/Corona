#include <iostream>
#include <string>
#include "AssetLoader.h"
#include "MemoryManager.h"
#include "GLTF.h"

using namespace std;
using namespace Corona;

namespace Corona 
{
    MemoryManager* g_pMemoryManager = new MemoryManager();
    AssetLoader* g_pAssetLoader = new AssetLoader();
}

int main(int , char** )
{
    g_pMemoryManager->Initialize();

    // string shader_pgm = asset_loader.SyncOpenAndReadTextFileToString("Shaders/illum.hs");
    // cout << shader_pgm;
    // string imgPath = g_pAssetLoader->GetFilePath("Scene/BoomBoxWithAxes/BoomBoxWithAxes_emissive.jpg");
    string imgPath = "Scene/FlightHelmet/FlightHelmet_baseColor.jpg";
    std::shared_ptr<Image> img;
    GltfParser gltf_parser;
    gltf_parser.ParseImage(imgPath, img);

    g_pMemoryManager->Finalize();

    delete g_pMemoryManager;

    return 0;
}


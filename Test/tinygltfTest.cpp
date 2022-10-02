#include "AssetLoader.h"
#include "MemoryManager.h"
#include "tinyglTF/tiny_gltf.h"

using namespace Corona;
using namespace tinygltf;

Model model;
TinyGLTF loader;
std::string err;
std::string warn;

namespace Corona 
{
    MemoryManager* g_pMemoryManager = new MemoryManager();
}

int main()
{
    g_pMemoryManager->Initialize();
    
    AssetLoader asset_loader;
    const std::string gltfFile = asset_loader.GetFilePath("Scene/Fox/Fox.gltf");
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, gltfFile);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, gltfFile); // for binary glTF(.glb)

    if (!warn.empty()) {
    printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
    printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
    printf("Failed to parse glTF\n");
    return -1;
    }
}

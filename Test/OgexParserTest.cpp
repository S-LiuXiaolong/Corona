#include <iostream>
#include <string>
#include "AssetLoader.h"
#include "MemoryManager.h"
#include "OGEX.h"

using namespace Corona;
using namespace std;

namespace Corona {
    MemoryManager* g_pMemoryManager = new MemoryManager();
}

int main(int , char** )
{
    g_pMemoryManager->Initialize();

    AssetLoader asset_loader;
    string ogex_text = asset_loader.SyncOpenAndReadTextFileToString("Scene/Example.ogex");

    OgexParser* ogex_parser = new OgexParser ();
    unique_ptr<BaseSceneNode> root_node = ogex_parser->Parse(ogex_text);
    delete ogex_parser;

    cout << *root_node << endl;

    g_pMemoryManager->Finalize();

    delete g_pMemoryManager;

    return 0;
}


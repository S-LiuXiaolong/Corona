#include "AssetLoader.h"
#include "MemoryManager.h"
#include "glTFSceneObject.h"

using namespace Corona;

namespace Corona 
{
    MemoryManager* g_pMemoryManager = new MemoryManager();
}

int main()
{
    g_pMemoryManager->Initialize();
    
    AssetLoader asset_loader;
    const std::string gltfFile = asset_loader.GetFilePath("Scene/Box.glb");
    
    Model::CreateInfo CI = {};
	CI.FileName = gltfFile;
	Model m_Model{ CI };
    
    std::unique_ptr<int> test = nullptr;
    test.reset();

    std::vector<uint32_t> data;
    data = m_Model.LinearNodes[0]->pMesh->Primitives[0]->IndexData;

    uint32_t* pData = static_cast<uint32_t*>(data.data());

    for (int i = 0; i < data.size(); i++)
    {
        printf("%d ", *pData);
        pData++;
    }

// 	std::vector<int>* v = new std::vector<int>(10);
// 	v->at(2); //Retrieve using pointer to member
// 	v->operator[](2); //Retrieve using pointer to operator member
// 	v->size(); //Retrieve size
// 	std::vector<int>& vr = *v; //Create a reference
// 	vr[2]; //Normal access through reference
// 	delete& vr; //Delete the reference. You could do the same with
// 	//a pointer (but not both!)
}

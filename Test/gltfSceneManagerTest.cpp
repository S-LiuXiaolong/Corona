#include "AssetLoader.h"
#include "MemoryManager.h"
#include "SceneManager.h"

using namespace Corona;

namespace Corona
{
	MemoryManager* g_pMemoryManager = static_cast<MemoryManager*>(new MemoryManager);
	AssetLoader* g_pAssetLoader = static_cast<AssetLoader*>(new AssetLoader);
	SceneManager* g_pSceneManager = static_cast<SceneManager*>(new SceneManager);
}

int main()
{
	g_pMemoryManager->Initialize();
	g_pSceneManager->Initialize();
	g_pAssetLoader->Initialize();

	g_pSceneManager->LoadScene("Scene/Box.glb");
	auto& scene = g_pSceneManager->GetSceneForRendering();

// 	cout << "Dump of Cameras" << endl;
// 	cout << "---------------------------" << endl;
// 	weak_ptr<SceneObjectCamera> pCamera = scene.GetFirstCamera();
// 	while (auto pObj = pCamera.lock())
// 	{
// 		cout << *pObj << endl;
// 		pCamera = scene.GetNextCamera();
// 	}
// 
// 
// 	cout << "Dump of Geometries" << endl;
// 	cout << "---------------------------" << endl;
// 	weak_ptr<SceneObjectGeometry> pGeometry = scene.GetFirstGeometry();
// 	while (auto pObj = pGeometry.lock())
// 	{
// 		cout << *pObj << endl;
// 		pGeometry = scene.GetNextGeometry();
// 	}


	g_pSceneManager->Finalize();
	g_pAssetLoader->Finalize();
	g_pMemoryManager->Finalize();

	delete g_pSceneManager;
	delete g_pAssetLoader;
	delete g_pMemoryManager;

	return 0;
}


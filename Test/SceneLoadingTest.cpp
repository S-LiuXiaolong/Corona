#include <iostream>
#include <string>
#include "AssetLoader.h"
#include "MemoryManager.h"
#include "SceneManager.h"

using namespace Corona;
using namespace std;

namespace Corona {
	MemoryManager* g_pMemoryManager = new MemoryManager();
	AssetLoader* g_pAssetLoader = new AssetLoader();
	SceneManager* g_pSceneManager = new SceneManager();
}

template<typename T>
static ostream& operator<<(ostream& out, unordered_map<string, shared_ptr<T>> map)
{
	for (auto p : map)
	{
		out << *p.second << endl;
	}

	return out;
}

int main(int argc, char** argv)
{
	g_pMemoryManager->Initialize();
	g_pSceneManager->Initialize();
	g_pAssetLoader->Initialize();

	if (argc >= 2) {
		g_pSceneManager->LoadScene(argv[1]);
	}
	else
	{
		g_pSceneManager->LoadScene("Scene/DamagedHelmet/DamagedHelmet.gltf");
		// g_pSceneManager->LoadScene("Scene/Suzanne/Suzanne.gltf");
		// g_pSceneManager->LoadScene("Scene/ABeautifulGame/ABeautifulGame.gltf");
		// g_pSceneManager->LoadScene("Scene/SciFiHelmet/SciFiHelmet.gltf");
		// g_pSceneManager->LoadScene("Scene/FlightHelmet/FlightHelmet.gltf");
	}
	auto& scene = g_pSceneManager->GetSceneForRendering();

// 	cout << "Dump of Geometries" << endl;
// 	cout << "---------------------------" << endl;
// 	for (auto _it : scene.GeometryNodes)
// 	{
// 		auto pGeometryNode = _it.second.lock();
// 		cout << *pGeometryNode << endl;
// 		if (pGeometryNode) {
// 			cout << *pGeometryNode << endl;
// 			weak_ptr<SceneObjectMesh> pGeometry = scene.GetGeometry(pGeometryNode->GetSceneObjectRef());
// 			auto pObj = pGeometry.lock();
// 			if (pObj)
// 				cout << *pObj << endl;
// 		}
// 	}
// 
// 	cout << "Dump of Cameras" << endl;
// 	cout << "---------------------------" << endl;
// 	for (auto _it : scene.CameraNodes)
// 	{
// 		auto pCameraNode = _it.second.lock();
// 		if (pCameraNode) {
// 			cout << *pCameraNode << endl;
// 			weak_ptr<SceneObjectCamera> pCamera = scene.GetCamera(pCameraNode->GetSceneObjectRef());
// 			auto pObj = pCamera.lock();
// 			if (pObj)
// 				cout << *pObj << endl;
// 		}
// 	}
// 
// 	cout << "Dump of Lights" << endl;
// 	cout << "---------------------------" << endl;
// 	for (auto _it : scene.LightNodes)
// 	{
// 		auto pLightNode = _it.second.lock();
// 		if (pLightNode) {
// 			cout << *pLightNode << endl;
// 			weak_ptr<SceneObjectLight> pLight = scene.GetLight(pLightNode->GetSceneObjectRef());
// 			auto pObj = pLight.lock();
// 			if (pObj)
// 				cout << *pObj << endl;
// 		}
// 	}
// 
// 
// 	cout << "Dump of Materials" << endl;
// 	cout << "---------------------------" << endl;
// 	for (auto _it : scene.Materials)
// 	{
// 		auto pMaterial = _it.second;
// 		if (pMaterial)
// 			cout << *pMaterial << endl;
// 	}

// 	cout << "Dump of Bone Nodes" << endl;
// 	cout << "---------------------------" << endl;
// 	for (auto _it : scene.BoneNodes)
// 	{
// 		auto pBone = _it.second.lock();
// 		if (pBone)
// 			cout << *pBone << endl;
// 	}

	g_pSceneManager->Finalize();
	g_pAssetLoader->Finalize();
	g_pMemoryManager->Finalize();

	delete g_pSceneManager;
	delete g_pAssetLoader;
	delete g_pMemoryManager;

	return 0;
}


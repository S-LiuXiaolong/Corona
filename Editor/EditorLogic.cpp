#include "EditorLogic.h"
#include "IApplication.h"
#include "SceneManager.h"

using namespace Corona;
using namespace std;

int EditorLogic::Initialize()
{
    int result;

    cout << "[EditorLogic] Editor Logic Initialize" << endl;

    if (g_pApp->GetCommandLineArgumentsCount() > 1)
    {
        auto scene_filename = g_pApp->GetCommandLineArgument(1);
        cout << "[EditorLogic] Loading Scene: " << scene_filename << endl;
        result = g_pSceneManager->LoadScene(scene_filename);
    }
    else
    {
        cout << "[EditorLogic] Loading Splash Scene" << endl;
        // result = g_pSceneManager->LoadScene("Scene/Suzanne/Suzanne.gltf");
        // result = g_pSceneManager->LoadScene("Scene/DamagedHelmet/DamagedHelmet.gltf");
        // result = g_pSceneManager->LoadScene("Scene/ABeautifulGame/ABeautifulGame.gltf");
        // result = g_pSceneManager->LoadScene("Scene/Lantern/Lantern.gltf");
        // result = g_pSceneManager->LoadScene("Scene/MetalRoughSpheres/MetalRoughSpheres.gltf");
        // result = g_pSceneManager->LoadScene("Scene/FlightHelmet/FlightHelmet.gltf");
        result = g_pSceneManager->LoadScene("Scene/SciFiHelmet/SciFiHelmet.gltf");
    }

    return result;
}

void EditorLogic::Finalize()
{ 
    cout << "[EditorLogic] Finalize" << endl;
}

void EditorLogic::Tick()
{
}

#ifdef DEBUG
void EditorLogic::DrawDebugInfo()
{
}
#endif

void EditorLogic::OnLeftKeyDown()
{
    // auto& scene = g_pSceneManager->GetSceneForRendering();
    // auto pCameraNode = scene.GetFirstCameraNode();
    // if (pCameraNode) {
    //     auto local_axis = pCameraNode->GetLocalAxis(); 
    //     Vector3f camera_x_axis;
    //     memcpy(camera_x_axis.data, local_axis[0], sizeof(camera_x_axis));

    //     // move camera along its local axis x direction
    //     pCameraNode->MoveBy(camera_x_axis);
    // }
}

void EditorLogic::OnRightKeyDown()
{
    // auto& scene = g_pSceneManager->GetSceneForRendering();
    // auto pCameraNode = scene.GetFirstCameraNode();
    // if (pCameraNode) {
    //     auto local_axis = pCameraNode->GetLocalAxis(); 
    //     Vector3f camera_x_axis;
    //     memcpy(camera_x_axis.data, local_axis[0], sizeof(camera_x_axis));

    //     // move along camera local axis -x direction
    //     pCameraNode->MoveBy(camera_x_axis * -1.0f);
    // }
}

void EditorLogic::OnUpKeyDown()
{
    // auto& scene = g_pSceneManager->GetSceneForRendering();
    // auto pCameraNode = scene.GetFirstCameraNode();
    // if (pCameraNode) {
    //     auto local_axis = pCameraNode->GetLocalAxis(); 
    //     Vector3f camera_y_axis;
    //     memcpy(camera_y_axis.data, local_axis[1], sizeof(camera_y_axis));

    //     // move camera along its local axis y direction
    //     pCameraNode->MoveBy(camera_y_axis);
    // }
}

void EditorLogic::OnDownKeyDown()
{
    // auto& scene = g_pSceneManager->GetSceneForRendering();
    // auto pCameraNode = scene.GetFirstCameraNode();
    // if (pCameraNode) {
    //     auto local_axis = pCameraNode->GetLocalAxis(); 
    //     Vector3f camera_y_axis;
    //     memcpy(camera_y_axis.data, local_axis[1], sizeof(camera_y_axis));

    //     // move camera along its local axis -y direction
    //     pCameraNode->MoveBy(camera_y_axis * -1.0f);
    // }
}

void EditorLogic::OnAnalogStick(int id, float deltaX, float deltaY)
{
    if (id == 0)
    {
        auto& scene = g_pSceneManager->GetSceneForRendering();
        auto pCameraNode = scene.GetFirstCameraNode();
        if (pCameraNode) {
            auto screen_width = g_pApp->GetConfiguration().screenWidth;
            auto screen_height = g_pApp->GetConfiguration().screenHeight;
            // move camera along its local axis -y direction
            pCameraNode->RotateBy(deltaX / screen_width * PI, deltaY / screen_height * PI, 0.0f);
        }
    }
}
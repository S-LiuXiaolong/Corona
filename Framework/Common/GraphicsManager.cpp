#include <iostream>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "IApplication.h"

using namespace std;

namespace Corona
{

    int GraphicsManager::Initialize()
    {
        int result = 0;
        InitConstants();
        return result;
    }

    void GraphicsManager::Finalize()
    {
    }

    void GraphicsManager::Tick()
    {
        if (g_pSceneManager->IsSceneChanged())
        {
            cout << "[GraphicsManager] Detected Scene Change, reinitialize buffers ..." << endl;
            ClearBuffers();
            ClearShaders();
            Scene& scene = g_pSceneManager->GetSceneForRendering();
            InitializeShaders();
            InitializeBuffers();
            g_pSceneManager->NotifySceneIsRenderingQueued();
        }

        UpdateConstants();

        Clear();
        Draw();
    }

    void GraphicsManager::UpdateConstants()
    {
        // Generate the view matrix based on the camera's position.
        CalculateCameraMatrix();
        CalculateLights();
    }

    void GraphicsManager::Clear()
    {
    }

    void GraphicsManager::Draw()
    {
        UpdateConstants();

        RenderBuffers();
#ifdef DEBUG
        RenderDebugBuffers();
#endif
    }

    // void GraphicsManager::InitConstants()
    // {
    //     // Initialize the world/model matrix to the identity matrix.
    //     BuildIdentityMatrix(m_worldMatrix);
	// 	// use default build-in camera
    //     position.x = mRadius*sinf(mPhi)*cosf(mTheta);
    //     position.z = mRadius*sinf(mPhi)*sinf(mTheta);
    //     position.y = mRadius*cosf(mPhi);
    //     lookAt = { 0, 0, 0 }, up = { 0, 1, 0 };

	// 	fieldOfView = PI / 2.0f;
	// 	nearClipDistance = 0.1f;
	// 	farClipDistance = 100.0f;

	// 	const GfxConfiguration& conf = g_pApp->GetConfiguration();

	// 	screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;
    // }

    void GraphicsManager::InitConstants()
    {
        // Initialize the world/model matrix to the identity matrix.
        BuildIdentityMatrix(m_worldMatrix);
    }

    bool GraphicsManager::InitializeShaders()
    {
        cout << "[GraphicsManager] GraphicsManager::InitializeShader()" << endl;
        return true;
    }

    void GraphicsManager::ClearShaders()
    {
        cout << "[GraphicsManager] GraphicsManager::ClearShaders()" << endl;
    }

    void GraphicsManager::CalculateCameraMatrix()
    {
        // TODO
        auto& scene = g_pSceneManager->GetSceneForRendering();
        // auto pCameraNode = scene.GetFirstCamera();
        // if (pCameraNode)
        // {
        //     m_DrawFrameContext.m_viewMatrix = *pCameraNode->GetCalculatedTransform();
        //     InverseMatrix4X4f(m_DrawFrameContext.m_viewMatrix);
        // }
        // else
        // {
        //     // use default build-in camera
        //     Vector3f position = { 0, -5, 0 }, lookAt = { 0, 0, 0 }, up = { 0, 0, 1 };
        //     BuildViewMatrix(m_DrawFrameContext.m_viewMatrix, position, lookAt, up);
        // }

        // use default build-in camera
        Vector3f position = { 0.3, 0.1, 3 }, lookAt = { 0, 0, 0 }, up = { 0, 1, 0 };
        // ABeautifulGame
        // Vector3f position = { 0, 0.5, 0.5 }, lookAt = { 0, 0, 0 }, up = { 0, 1, 0 };
        BuildViewMatrix(m_viewMatrix, position, lookAt, up);

        float fieldOfView = PI / 2.0f;
        float nearClipDistance = 0.1f;
        float farClipDistance = 100.0f;

        // // if (pCameraNode) 
        // // {
        // //     auto pCamera = scene.GetCamera(pCameraNode->GetSceneObjectRef());
        // //     // Set the field of view and screen aspect ratio.
        // //     fieldOfView = dynamic_pointer_cast<SceneObjectPerspectiveCamera>(pCamera)->GetFov();
        // //     nearClipDistance = pCamera->GetNearClipDistance();
        // //     farClipDistance = pCamera->GetFarClipDistance();
        // // }

        const GfxConfiguration& conf = g_pApp->GetConfiguration();

        float screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;

        // // Build the perspective projection matrix.
        // BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);
        //BuildIdentityMatrix(m_DrawFrameContext.m_projectionMatrix);

        // The Axis Location:
        //               Y
        //               |
        //               |
        //               | 
        //    X----------|
        //              /
        //             /
        //            /
        //           /
        //          Z
		// position.x = mRadius * sinf(mPhi) * cosf(mTheta);
		// position.z = mRadius * sinf(mPhi) * sinf(mTheta);
		// position.y = mRadius * cosf(mPhi);

        m_DrawFrameContext.m_cameraPosition = Vector4f(position, 1.0f);
		BuildViewMatrix(m_viewMatrix, position, lookAt, up);
		// Build the perspective projection matrix.
		BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);
        
        // �ܲ��ܴ�����������������Ȼ����������˵õ�������棿���ҹ����ǲ��У�û׼��ξ�����ԣ���
        m_DrawFrameContext.m_worldMatrix = m_worldMatrix;
        m_DrawFrameContext.m_worldViewMatrix = m_worldMatrix * m_viewMatrix;
        m_DrawFrameContext.m_worldViewProjectionMatrix = m_worldMatrix * m_viewMatrix * m_projectionMatrix;
        
        // ��C++��DirectXMath�У�������XMFLOAT4X4������ʹ�ú������ɵ�XMMATRIX�����ǲ������������Ľ��ͷ�ʽ,
        // ����HLSL�У�Ĭ�ϵ�matrix��float4x4���õ������������Ľ�����ʽ��

        // ������ϣ���һ���������ܹ��������Ƶ������
        // 1��C++����˲�����ת�ã�HLSL��ʹ��row_major matrix(���������)��mul�����������������(������)������ʵ���������(������ X ���������) �����ַ���������⣬����������dp4����ȡ������кܲ����㣬��HLSL�л��������ת�þ���Ĵ���ָ�����������ʧ��
        // 2��C++����˽���ת�ã�HLSL��ʹ��matrix(���������) ��mul�����������������(������)����������(������ X ���������)����C++�����Ҫ����һ�ξ���ת�ã�HLSL�ڲ�������ת�� �����ǹٷ�������ʹ�õķ�ʽ����������ʹ��dp4�������ֱ��ȡ�����������У��Ӷ������ڲ�����������ת��ָ�����Ҳʹ�����ַ�ʽ��
        // 3��C++����˲�����ת�ã�HLSL��ʹ��matrix(���������)��mul���������������ұ�(������)��ʵ��������(��������� X ������)�����ַ�����ȷ���У�ȡ�о������Ҳ�ȽϷ��㣬Ч�����ֺ�2��ͬ������HLSL�Ǳߵľ���˷���Ҫ������д��Ȼ��DX������ǳ�����������ģ���OpenGL��ϰ�ߴ�������е㡣����
        // 4��C++����˽���ת�ã�HLSL��ʹ��row_major matrix(���������)��mul���������������ұ�(������)��ʵ��������(��������� X ������)�� �������ַ���Ҳ���Ի��Ƴ����������Ǻ��������ܡ�
        Transpose(m_DrawFrameContext.m_worldMatrix);
        Transpose(m_DrawFrameContext.m_worldViewMatrix);
        Transpose(m_DrawFrameContext.m_worldViewProjectionMatrix);
    }

    void GraphicsManager::CalculateLights()
    {
        // TODO
        auto& scene = g_pSceneManager->GetSceneForRendering();
        // auto pLightNode = scene.GetFirstLight();
        // if (pLightNode) 
        // {
        //     m_DrawFrameContext.m_lightPosition = { 0.0f, 0.0f, 0.0f };
        //     TransformCoord(m_DrawFrameContext.m_lightPosition, *pLightNode->GetCalculatedTransform());

        //     auto pLight = scene.GetLight(pLightNode->GetSceneObjectRef());
        //     if (pLight) {
        //         m_DrawFrameContext.m_lightColor = pLight->GetColor().Value;
        //     }
        // }
        // else 
        // {
        //     // use default build-in light 
        //     m_DrawFrameContext.m_lightPosition = { -1.0f, -5.0f, 0.0f};
        //     m_DrawFrameContext.m_lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        // }

        // only support default light at the time
        m_DrawFrameContext.m_lightPosition = { 0.0f, 1.0f, 1.0f};
        m_DrawFrameContext.m_lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    }

    bool GraphicsManager::InitializeBuffers()
    {
        cout << "[GraphicsManager] GraphicsManager::InitializeBuffers()" << endl;
        return true;
    }

    void GraphicsManager::ClearBuffers()
    {
        cout << "[GraphicsManager] GraphicsManager::ClearBuffers()" << endl;
    }

    void GraphicsManager::RenderBuffers()
    {
        cout << "[GraphicsManager] GraphicsManager::RenderBuffers()" << endl;
    }

#ifdef DEBUG
    // add debug manager operations
#endif
//     // temporary. should be moved to scene manager and script engine (policy engine)
//     void GraphicsManager::WorldRotateX(float radians)
//     {
//         Matrix4X4f rotationMatrix;
//         MatrixRotationX(rotationMatrix, radians);
//         m_worldMatrix = m_worldMatrix * rotationMatrix;
//     }

//     void GraphicsManager::WorldRotateY(float radians)
//     {
//         Matrix4X4f rotationMatrix;
//         MatrixRotationY(rotationMatrix, radians);
//         m_worldMatrix = m_worldMatrix * rotationMatrix;
//     }

// 	void GraphicsManager::WorldRotateZ(float radians)
// 	{
// 		Matrix4X4f rotationMatrix;
// 		MatrixRotationZ(rotationMatrix, radians);
// 		m_worldMatrix = m_worldMatrix * rotationMatrix;
// 	}

// 	void GraphicsManager::CameraRotateX(float radians)
// 	{
//         mPhi += radians;
//         // mPhi = Clamp(mPhi, 0.1f, PI - 0.1f);
// 	}

// 	void GraphicsManager::CameraRotateZ(float radians)
// 	{
//         mTheta += radians;
// 	}

//     void GraphicsManager::CameraTranslationX(float distance)
// 	{
// 		position = position + Vector3f(distance, 0, 0);
//         lookAt = lookAt + Vector3f(distance, 0, 0);
// 	}

// 	void GraphicsManager::CameraTranslationY(float distance)
// 	{
//         position = position + Vector3f(0, distance, 0);
//         lookAt = lookAt + Vector3f(0, distance, 0);
// 	}


// 	void GraphicsManager::OnMouseDown(int x, int y)
// 	{
// 		mLastMousePos_x = x;
// 		mLastMousePos_y = y;
// 	}

// 	void GraphicsManager::OnMouseMoveL(int x, int y)
// 	{
// 		// Make each pixel correspond to a quarter of a degree.
// 		float dx = (0.05f * static_cast<float>(x - mLastMousePos_x)) * PI / 180;
// 		float dy = (0.05f * static_cast<float>(y - mLastMousePos_y)) * PI / 180;
//         mTheta -= dx;
// 		mPhi -= dy;

//         mPhi = Clamp(mPhi, 0.1f, PI - 0.1f);
// 	}

// 	void GraphicsManager::OnMouseMoveR(int x, int y)
// 	{
// // 		float dx = 0.0005f * static_cast<float>(x - mLastMousePos_x);
// // 		float dy = 0.0005f * static_cast<float>(y - mLastMousePos_y);
// // 
// // 		position = position + Vector3f(-dy, dx, 0.0f);
// // 		lookAt = lookAt + Vector3f(-dy, dx, 0.0f);
// 	}

// 	void GraphicsManager::OnMouseWheel(int delta)
// 	{
//          fieldOfView += 0.05f * delta * PI / 180;
// 	}

}
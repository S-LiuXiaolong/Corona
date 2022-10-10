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
            cout << "Detected Scene Change, reinitialize Graphics Manager..." << endl;
            Finalize();
            Initialize();
        }
        // Generate the view matrix based on the camera's position.
        CalculateCameraMatrix();
        CalculateLights();
    }

    void GraphicsManager::Clear()
    {
    }

    void GraphicsManager::Draw()
    {
    }

    bool GraphicsManager::SetPerFrameShaderParameters()
    {
        cout << "[RHI] GraphicsManager::SetPerFrameShaderParameters(void)" << endl;
        return true;
    }
    
    bool GraphicsManager::SetPerBatchShaderParameters(const char* paramName, const Matrix4X4f& param)
    {
        cout << "[RHI] GraphicsManager::SetPerFrameShaderParameters(const char* paramName, const Matrix4X4f& param)" << endl;
        cout << "paramName = " << paramName << endl;
        cout << "param = " << param << endl;
        return true;
    }

    bool GraphicsManager::SetPerBatchShaderParameters(const char* paramName, const Vector3f& param)
    {
        cout << "[RHI] GraphicsManager::SetPerFrameShaderParameters(const char* paramName, const Vector3f& param)" << endl;
        cout << "paramName = " << paramName << endl;
        cout << "param = " << param << endl;
        return true;
    }

    bool GraphicsManager::SetPerBatchShaderParameters(const char* paramName, const float param)
    {
        cout << "[RHI] GraphicsManager::SetPerFrameShaderParameters(const char* paramName, const float param)" << endl;
        cout << "paramName = " << paramName << endl;
        cout << "param = " << param << endl;
        return true;
    }

    bool GraphicsManager::SetPerBatchShaderParameters(const char* paramName, const int param)
    {
        cout << "[RHI] GraphicsManager::SetPerFrameShaderParameters(const char* paramName, const int param)" << endl;
        cout << "paramName = " << paramName << endl;
        cout << "param = " << param << endl;
        return true;
    }

    void GraphicsManager::InitConstants()
    {
        // Initialize the world/model matrix to the identity matrix.
        BuildIdentityMatrix(m_worldMatrix);
		// use default build-in camera
        position.x = mRadius*sinf(mPhi)*cosf(mTheta);
        position.z = mRadius*sinf(mPhi)*sinf(mTheta);
        position.y = mRadius*cosf(mPhi);
        lookAt = { 0, 0, 0 }, up = { 0, 0, 1 };

		fieldOfView = PI / 2.0f;
		nearClipDistance = 0.1f;
		farClipDistance = 100.0f;

		const GfxConfiguration& conf = g_pApp->GetConfiguration();

		screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;
    }

    bool GraphicsManager::InitializeShader(const char* vsFilename, const char* fsFilename)
    {
        cout << "[RHI] GraphicsManager::InitializeShader(const char* vsFilename, const char* fsFilename)" << endl;
        cout << "VS Filename: " << vsFilename << endl;
        cout << "PS Filename: " << fsFilename << endl;
        return true;
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

//         // use default build-in camera
//         Vector3f position = { 0, -2, 0 }, lookAt = { 0, 0, 0 }, up = { 0, 0, 1 };
//         BuildViewMatrix(m_viewMatrix, position, lookAt, up);
// 
//         float fieldOfView = PI / 2.0f;
//         float nearClipDistance = 0.1f;
//         float farClipDistance = 100.0f;
// 
//         // if (pCameraNode) 
//         // {
//         //     auto pCamera = scene.GetCamera(pCameraNode->GetSceneObjectRef());
//         //     // Set the field of view and screen aspect ratio.
//         //     fieldOfView = dynamic_pointer_cast<SceneObjectPerspectiveCamera>(pCamera)->GetFov();
//         //     nearClipDistance = pCamera->GetNearClipDistance();
//         //     farClipDistance = pCamera->GetFarClipDistance();
//         // }
// 
//         const GfxConfiguration& conf = g_pApp->GetConfiguration();
// 
//         float screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;
// 
//         // Build the perspective projection matrix.
//         BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);
        //BuildIdentityMatrix(m_DrawFrameContext.m_projectionMatrix);
        // 
		// Attention
        Vector3f mUp = up;
        if (mPhi >= 0)
        {
			if (((int)(mPhi / PI) & 1) == 0)
			{

			}
			else
			{
				mUp.x *= -1;
				mUp.y *= -1;
				mUp.z *= -1;
			}
        }
        else
        {
			if (((int)(mPhi / PI) & 1) == 0)
			{
				mUp.x *= -1;
				mUp.y *= -1;
				mUp.z *= -1;
			}
			else
			{

			}
        }

		position.x = mRadius * sinf(mPhi) * cosf(mTheta);
		position.y = mRadius * sinf(mPhi) * sinf(mTheta);
		position.z = mRadius * cosf(mPhi);

        m_DrawFrameContext.m_cameraPosition = Vector4f(position, 1.0f);
		BuildViewMatrix(m_viewMatrix, position, lookAt, mUp);
		// Build the perspective projection matrix.
		BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);

        m_DrawFrameContext.m_worldMatrix = m_worldMatrix;
        m_DrawFrameContext.m_worldViewMatrix = m_worldMatrix * m_viewMatrix;
        m_DrawFrameContext.m_worldViewProjectionMatrix = m_worldMatrix * m_viewMatrix * m_projectionMatrix;
        
        // 在C++的DirectXMath中，无论是XMFLOAT4X4，还是使用函数生成的XMMATRIX，都是采用行主序矩阵的解释方式,
        // 而在HLSL中，默认的matrix或float4x4采用的是列主序矩阵的解释形式。

        // 经过组合，就一共有四种能够正常绘制的情况：
        // 1、C++代码端不进行转置，HLSL中使用row_major matrix(行主序矩阵)，mul函数让向量放在左边(行向量)，这样实际运算就是(行向量 X 行主序矩阵) 。这种方法易于理解，但是这样做dp4运算取矩阵的列很不方便，在HLSL中会产生用于转置矩阵的大量指令，性能上有损失。
        // 2、C++代码端进行转置，HLSL中使用matrix(列主序矩阵) ，mul函数让向量放在左边(行向量)，这样就是(行向量 X 列主序矩阵)，但C++这边需要进行一次矩阵转置，HLSL内部不产生转置 。这是官方例程所使用的方式，这样可以使得dp4运算可以直接取列主序矩阵的行，从而避免内部产生大量的转置指令。后续也使用这种方式。
        // 3、C++代码端不进行转置，HLSL中使用matrix(列主序矩阵)，mul函数让向量放在右边(列向量)，实际运算是(列主序矩阵 X 列向量)。这种方法的确可行，取列矩阵的行也比较方便，效率上又和2等同，就是HLSL那边的矩阵乘法都要反过来写，然而DX本身就是崇尚行主矩阵的，把OpenGL的习惯带来这边有点。。。
        // 4、C++代码端进行转置，HLSL中使用row_major matrix(行主序矩阵)，mul函数让向量放在右边(列向量)，实际运算是(行主序矩阵 X 列向量)。 就算这种方法也可以绘制出来，但还是很让人难受。
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
        m_DrawFrameContext.m_lightPosition = { -1.0f, -5.0f, 0.0f};
        m_DrawFrameContext.m_lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    }

    void GraphicsManager::InitializeBuffers()
    {
    }

    void GraphicsManager::RenderBuffers()
    {
        cout << "[RHI] GraphicsManager::RenderBuffers()" << endl;
    }

    // temporary. should be moved to scene manager and script engine (policy engine)
    void GraphicsManager::WorldRotateX(float radians)
    {
        Matrix4X4f rotationMatrix;
        MatrixRotationX(rotationMatrix, radians);
        m_worldMatrix = m_worldMatrix * rotationMatrix;
    }

    void GraphicsManager::WorldRotateY(float radians)
    {
        Matrix4X4f rotationMatrix;
        MatrixRotationY(rotationMatrix, radians);
        m_worldMatrix = m_worldMatrix * rotationMatrix;
    }

	void GraphicsManager::WorldRotateZ(float radians)
	{
		Matrix4X4f rotationMatrix;
		MatrixRotationZ(rotationMatrix, radians);
		m_worldMatrix = m_worldMatrix * rotationMatrix;
	}

	void GraphicsManager::CameraRotateX(float radians)
	{
        mPhi += radians;
        // mPhi = Clamp(mPhi, 0.1f, PI - 0.1f);
	}

	void GraphicsManager::CameraRotateZ(float radians)
	{
        mTheta += radians;
	}

    void GraphicsManager::CameraTranslationX(float distance)
	{
		position = position + Vector3f(distance, 0, 0);
        lookAt = lookAt + Vector3f(distance, 0, 0);
	}

	void GraphicsManager::CameraTranslationY(float distance)
	{
        position = position + Vector3f(0, distance, 0);
        lookAt = lookAt + Vector3f(0, distance, 0);
	}


	void GraphicsManager::OnMouseDown(int x, int y)
	{
		mLastMousePos_x = x;
		mLastMousePos_y = y;
	}

	void GraphicsManager::OnMouseMoveL(int x, int y)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = (0.05f * static_cast<float>(x - mLastMousePos_x)) * PI / 180;
		float dy = (0.05f * static_cast<float>(y - mLastMousePos_y)) * PI / 180;

		// Update angles based on input to orbit camera around box.
		if (mPhi >= 0)
		{
			if (((int)(mPhi / PI) & 1) == 0)
			{
                mTheta += dx;
			}
			else
			{
                mTheta -= dx;
			}

		}
		else
		{
			if (((int)(-mPhi / PI) & 1) == 0)
			{
				mTheta -= dx;
			}
			else
			{
				mTheta += dx;
			}
		}
		mPhi -= dy;
	}

	void GraphicsManager::OnMouseMoveR(int x, int y)
	{
// 		float dx = 0.0005f * static_cast<float>(x - mLastMousePos_x);
// 		float dy = 0.0005f * static_cast<float>(y - mLastMousePos_y);
// 
// 		position = position + Vector3f(-dy, dx, 0.0f);
// 		lookAt = lookAt + Vector3f(-dy, dx, 0.0f);
	}

	void GraphicsManager::OnMouseWheel(int delta)
	{
         fieldOfView += 0.05f * delta * PI / 180;
	}

}
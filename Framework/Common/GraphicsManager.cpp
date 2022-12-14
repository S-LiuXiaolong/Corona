#include <iostream>
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "IApplication.h"
#include "ForwardRenderPass.h"
#include "ShadowMapPass.h"
#include "HUDPass.h"

using namespace std;

namespace Corona
{

    int GraphicsManager::Initialize()
    {
        int result = 0;
        m_Frames.resize(kFrameCount);
        InitConstants();
        // m_DrawPasses.push_back(make_shared<ShadowMapPass>());
        m_DrawPasses.push_back(make_shared<ForwardRenderPass>());
        // m_DrawPasses.push_back(make_shared<HUDPass>());
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
        // update scene object position
        auto& frame = m_Frames[m_nFrameIndex];

        for (auto dbc : frame.batchContexts)
        {
            dbc->trans = dbc->node->Transforms.matrix;
        }

        // TODO: Update scene nodes before update constants (temporary for test)
		auto& scene = g_pSceneManager->GetSceneForRendering();
		for (auto& root_node : scene.RootNodes) // should calculate only those needed (camera, light, etc.)
		{
			root_node.lock()->UpdateTransforms();
		}
		// TODO: use this function to get boundbox and BVH
		// CalculateSceneDimensions();
		// UpdatePrimitiveData();

        // Generate the view matrix based on the camera's position.
        CalculateCameraMatrix();
        CalculateLights();
    }

    void GraphicsManager::Clear()
    {
    }

    void GraphicsManager::Draw()
    {
        auto& frame = m_Frames[m_nFrameIndex];

        for (auto pDrawPass : m_DrawPasses)
        {
            pDrawPass->Draw(frame);
        }

//         UpdateConstants();

//         RenderBuffers();
// #ifdef DEBUG
//         RenderDebugBuffers();
// #endif
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
        BuildIdentityMatrix(m_Frames[m_nFrameIndex].m_worldMatrix);
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

    void GraphicsManager::InitCameraMatrix()
    {
		auto& scene = g_pSceneManager->GetSceneForRendering();
		auto pCameraNode = scene.GetFirstCameraNode();
// 		if (pCameraNode)
// 		{
// 			pCameraNode->InitMatrix();
// 		}
    }

    void GraphicsManager::CalculateCameraMatrix()
    {
        // TODO
        auto& scene = g_pSceneManager->GetSceneForRendering();
		auto pCameraNode = scene.GetFirstCameraNode();
        DrawFrameContext& frameContext = m_Frames[m_nFrameIndex].frameContext;
		if (pCameraNode) 
        {
			auto transform = pCameraNode->GetViewMatrix();
			m_Frames[m_nFrameIndex].m_viewMatrix = transform;
            frameContext.m_cameraPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
            Transform(frameContext.m_cameraPosition, transform);
		}
		else
        {
			// use default build-in camera
			Vector3f position = { 0.3f, 0.1f, 3.0f }, lookAt = { 0.0f, 0.0f, 0.0f }, up = { 0.0f, 1.0f, 0.0f };
			BuildViewMatrix(m_Frames[m_nFrameIndex].m_viewMatrix, position, lookAt, up);
		}

		// // use default build-in camera
		// Vector3f position = { 0.3f, 0.1f, 3.0f }, lookAt = { 0.0f, 0.0f, 0.0f }, up = { 0.0f, 1.0f, 0.0f };
		// BuildViewMatrix(m_viewMatrix, position, lookAt, up);

        float fieldOfView = PI / 2.0f;
        float nearClipDistance = 0.1f;
        float farClipDistance = 100.0f;

        if (pCameraNode) 
        {
            auto pCamera = pCameraNode->pCamera;
            // Set the field of view and screen aspect ratio.
            fieldOfView = dynamic_pointer_cast<SceneObjectPerspectiveCamera>(pCamera)->GetFov();
            nearClipDistance = pCamera->GetNearClipDistance();
            farClipDistance = pCamera->GetFarClipDistance();
        }

        const GfxConfiguration& conf = g_pApp->GetConfiguration();

        float screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;

        // // Build the perspective projection matrix.
        // BuildPerspectiveFovLHMatrix(m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);
        //BuildIdentityMatrix(frameContext.m_projectionMatrix);

        // The world Axis Location:
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

		// Build the perspective projection matrix.
		BuildPerspectiveFovLHMatrix(m_Frames[m_nFrameIndex].m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);
        
        // ??????????????????????????????????????????????????????????????????????????????????????????
        // TODO
        frameContext.m_worldMatrix = m_Frames[m_nFrameIndex].m_worldMatrix;
        frameContext.m_worldViewMatrix = m_Frames[m_nFrameIndex].m_worldMatrix * m_Frames[m_nFrameIndex].m_viewMatrix;
        frameContext.m_worldViewProjectionMatrix = m_Frames[m_nFrameIndex].m_worldMatrix * m_Frames[m_nFrameIndex].m_viewMatrix * m_Frames[m_nFrameIndex].m_projectionMatrix;
        
        // ??C++??DirectXMath??????????XMFLOAT4X4????????????????????XMMATRIX??????????????????????????????,
        // ????HLSL??????????matrix??float4x4??????????????????????????????

        // ??????????????????????????????????????????
        // 1??C++??????????????????HLSL??????row_major matrix(??????????)??mul??????????????????(??????)??????????????????(?????? X ??????????) ??????????????????????????????dp4??????????????????????????HLSL??????????????????????????????????????????????
        // 2??C++????????????????HLSL??????matrix(??????????) ??mul??????????????????(??????)??????????(?????? X ??????????)????C++??????????????????????????HLSL?????????????? ????????????????????????????????????????dp4??????????????????????????????????????????????????????????????????????????????????
        // 3??C++??????????????????HLSL??????matrix(??????????)??mul??????????????????(??????)????????????(?????????? X ??????)??????????????????????????????????????????????????????2??????????HLSL????????????????????????????????DX??????????????????????????OpenGL????????????????????????
        // 4??C++????????????????HLSL??????row_major matrix(??????????)??mul??????????????????(??????)????????????(?????????? X ??????)?? ??????????????????????????????????????????????
        Transpose(frameContext.m_worldMatrix);
        Transpose(frameContext.m_worldViewMatrix);
        Transpose(frameContext.m_worldViewProjectionMatrix);
    }

    void GraphicsManager::CalculateLights()
    {
        DrawFrameContext& frameContext = m_Frames[m_nFrameIndex].frameContext;
        auto& scene = g_pSceneManager->GetSceneForRendering();
//         auto pLightNode = scene.GetFirstLight();
//         if (pLightNode) 
//         {
//             frameContext.m_lightPosition = { 0.0f, 0.0f, 0.0f };
//             TransformCoord(frameContext.m_lightPosition, *pLightNode->GetCalculatedTransform());
// 
//             auto pLight = scene.GetLight(pLightNode->GetSceneObjectRef());
//             if (pLight) {
//                 frameContext.m_lightColor = pLight->GetColor().Value;
//             }
//         }
//         else 
//         {
//             // use default build-in light
//             frameContext.m_lightPosition = { -1.0f, -5.0f, 0.0f};
//             frameContext.m_lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
//         }

        // TODO: Add light-pos camera to get shadowMap
        int i = 0;
        for (auto LightNode : scene.LightNodes)
        {
            Light light;
            auto pLightNode = LightNode.second.lock();
            if (!pLightNode) continue;
            if (!pLightNode->m_Parent)
            {
				auto trans_ptr = pLightNode->Transforms.matrix;
				light.m_lightPosition = { 0.0f, 0.0f, 0.0f};
                TransformCoord(light.m_lightPosition, trans_ptr);
                // TransformCoord cannot be used here
                Vector4f direction = { 0.0f, -1.0f, 0.0f, 0.0f };
				Transform(direction, trans_ptr);
				light.m_lightDirection = direction.xyz;

                auto pOrientationNode = pLightNode->m_Children;
                auto pLight = scene.LinearLights[pOrientationNode[0]->lightIndex].lock();
                if (pLight)
                {
                    light.m_lightColor = pLight->GetColor().xyz;
                    light.m_lightIntensity = pLight->GetIntensity();
                    if (pLight->GetType() == SceneObjectType::kSceneObjectTypeLightSpot)
                    {
                        auto pSpotLight = dynamic_pointer_cast<SceneObjectSpotLight>(pLight);
                        light.m_fallOffStart = pSpotLight->GetInnerConeAngle();
                        light.m_fallOffEnd = pSpotLight->GetOuterConeAngle();
                    }
                }
                else
                {
                    assert(0);
                }

                frameContext.m_lights[i] = light;
                ++i;
            }

        }

		// // only support default light at the time
		// frameContext.m_lightPosition = { 0.0f, 0.0f, 10.0f};
		// frameContext.m_lightColor = { 0.0f, 0.0f, 1.0f, 1.0f }; // ARGB
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

#ifdef _DEBUG
    void GraphicsManager::RenderDebugBuffers()
    {
        cout << "[GraphicsManager] GraphicsManager::RenderDebugBuffers()" << endl;
    }

    void GraphicsManager::DrawPoint(const Point& point, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawPoint(" << point << ","
            << color << ")" << endl;
    }

    void GraphicsManager::DrawPointSet(const PointSet& point_set, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawPointSet(" << point_set.size() << ","
            << color << ")" << endl;
    }

    void GraphicsManager::DrawPointSet(const PointSet& point_set, const Matrix4X4f& trans, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawPointSet(" << point_set.size() << ","
            << trans << "," 
            << color << ")" << endl;
    }

    void GraphicsManager::DrawLine(const Point& from, const Point& to, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawLine(" << from << ","
            << to << "," 
            << color << ")" << endl;
    }

    void GraphicsManager::DrawLine(const PointList& vertices, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawLine(" << vertices.size() << ","
            << color << ")" << endl;
    }

    void GraphicsManager::DrawLine(const PointList& vertices, const Matrix4X4f& trans, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawLine(" << vertices.size() << ","
            << trans << "," 
            << color << ")" << endl;
    }

    void GraphicsManager::DrawEdgeList(const EdgeList& edges, const Vector3f& color)
    {
        PointList point_list;

        for (auto edge : edges)
        {
            point_list.push_back(edge->first);
            point_list.push_back(edge->second);
        }

        DrawLine(point_list, color);
    }

    void GraphicsManager::DrawTriangle(const PointList& vertices, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawTriangle(" << vertices.size() << ","
            << color << ")" << endl;
    }

    void GraphicsManager::DrawTriangle(const PointList& vertices, const Matrix4X4f& trans, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawTriangle(" << vertices.size() << ","
            << color << ")" << endl;
    }

    void GraphicsManager::DrawTriangleStrip(const PointList& vertices, const Vector3f& color)
    {
        cout << "[GraphicsManager] GraphicsManager::DrawTriangleStrip(" << vertices.size() << ","
            << color << ")" << endl;
    }

    void GraphicsManager::DrawPolygon(const Face& polygon, const Vector3f& color)
    {
        PointSet vertices;
        PointList edges;
        for (auto pEdge : polygon.Edges)
        {
            vertices.insert({pEdge->first, pEdge->second});
            edges.push_back(pEdge->first);
            edges.push_back(pEdge->second);
        }
        DrawLine(edges, color);

        DrawPointSet(vertices, color);

        DrawTriangle(polygon.GetVertices(), color);
    }

    void GraphicsManager::DrawPolygon(const Face& polygon, const Matrix4X4f& trans, const Vector3f& color)
    {
        PointSet vertices;
        PointList edges;
        for (auto pEdge : polygon.Edges)
        {
            vertices.insert({pEdge->first, pEdge->second});
            edges.push_back(pEdge->first);
            edges.push_back(pEdge->second);
        }
        DrawLine(edges, trans, color);

        DrawPointSet(vertices, trans, color);

        DrawTriangle(polygon.GetVertices(), trans, color);
    }

    void GraphicsManager::DrawPolyhydron(const Polyhedron& polyhedron, const Vector3f& color)
    {
        for (auto pFace : polyhedron.Faces)
        {
            DrawPolygon(*pFace, color);
        }
    }

    void GraphicsManager::DrawPolyhydron(const Polyhedron& polyhedron, const Matrix4X4f& trans, const Vector3f& color)
    {
        for (auto pFace : polyhedron.Faces)
        {
            DrawPolygon(*pFace, trans, color);
        }
    }

    void GraphicsManager::DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color)
    {
        //  ******0--------3********
        //  *****/:       /|********
        //  ****1--------2 |********
        //  ****| :      | |********
        //  ****| 4- - - | 7********
        //  ****|/       |/*********
        //  ****5--------6**********

        // vertices
        PointPtr points[8];
        for (int i = 0; i < 8; i++)
            points[i] = make_shared<Point>(bbMin);
        *points[0] = *points[2] = *points[3] = *points[7] = bbMax;
        points[0]->data[0] = bbMin[0];
        points[2]->data[1] = bbMin[1];
        points[7]->data[2] = bbMin[2];
        points[1]->data[2] = bbMax[2];
        points[4]->data[1] = bbMax[1];
        points[6]->data[0] = bbMax[0];

        // edges
        EdgeList edges;
        
        // top
        edges.push_back(make_shared<Edge>(make_pair(points[0], points[3])));
        edges.push_back(make_shared<Edge>(make_pair(points[3], points[2])));
        edges.push_back(make_shared<Edge>(make_pair(points[2], points[1])));
        edges.push_back(make_shared<Edge>(make_pair(points[1], points[0])));

        // bottom
        edges.push_back(make_shared<Edge>(make_pair(points[4], points[7])));
        edges.push_back(make_shared<Edge>(make_pair(points[7], points[6])));
        edges.push_back(make_shared<Edge>(make_pair(points[6], points[5])));
        edges.push_back(make_shared<Edge>(make_pair(points[5], points[4])));

        // side
        edges.push_back(make_shared<Edge>(make_pair(points[0], points[4])));
        edges.push_back(make_shared<Edge>(make_pair(points[1], points[5])));
        edges.push_back(make_shared<Edge>(make_pair(points[2], points[6])));
        edges.push_back(make_shared<Edge>(make_pair(points[3], points[7])));

        DrawEdgeList(edges, color);
    }

    void GraphicsManager::ClearDebugBuffers()
    {
        cout << "[GraphicsManager] GraphicsManager::ClearDebugBuffers(void)" << endl;
    }
#endif

    void GraphicsManager::UseShaderProgram(const intptr_t shaderProgram)
    {
        cout << "[GraphicsManager] UseShaderProgram(" << shaderProgram << ")" << endl;
    }

    void GraphicsManager::SetPerFrameConstants(const DrawFrameContext& context)
    {
        cout << "[GraphicsManager] SetPerFrameConstants(" << &context << ")" << endl;
    }

    void GraphicsManager::DrawBatch(const DrawBatchContext& context)
    {
        cout << "[GraphicsManager] DrawBatch(" << &context << ")" << endl;
    }

    void GraphicsManager::DrawBatchDepthOnly(const DrawBatchContext& context)
    {
        cout << "[GraphicsManager] DrawBatchDepthOnly(" << &context << ")" << endl;
    }

    intptr_t GraphicsManager::GenerateShadowMapArray(uint32_t count)
    {
        cout << "[GraphicsManager] GenerateShadowMap(" << count << ")" << endl;
        return 0;
    }

    void GraphicsManager::BeginShadowMap(const Light& light, const intptr_t shadowmap, uint32_t layer_index)
    {
        cout << "[GraphicsManager] BeginShadowMap(" << shadowmap << ", " << layer_index << ")" << endl;
    }

    void GraphicsManager::EndShadowMap(const intptr_t shadowmap, uint32_t layer_index)
    {
        cout << "[GraphicsManager] EndShadowMap(" << shadowmap << ", " << layer_index << ")" << endl;
    }

    void GraphicsManager::SetShadowMap(const intptr_t shadowmap)
    {
        cout << "[GraphicsManager] SetShadowMap(" << shadowmap << ")" << endl;
    }

    void GraphicsManager::DestroyShadowMap(intptr_t& shadowmap)
    {
        cout << "[GraphicsManager] DestroyShadowMap(" << shadowmap << ")" << endl;
        shadowmap = -1;
    }

}
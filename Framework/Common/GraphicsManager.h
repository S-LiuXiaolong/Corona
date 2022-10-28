#pragma once
#include "GfxStructures.h"
#include "IRuntimeModule.h"
#include "IDrawPass.h"
#include "IShaderManager.h"
#include "geommath.h"
#include "Scene.h"
#include "Polyhedron.h"

namespace Corona
{
    class Scene;

    class GraphicsManager : implements IRuntimeModule
    {
    public:
        virtual ~GraphicsManager() {};

        virtual int Initialize();
	    virtual void Finalize();

	    virtual void Tick();

        virtual void Clear();
        virtual void Draw();

        virtual void UseShaderProgram(const intptr_t shaderProgram);
        virtual void SetPerFrameConstants(const DrawFrameContext& context);
        virtual void DrawBatch(const DrawBatchContext& context);
        virtual void DrawBatchDepthOnly(const DrawBatchContext& context);

        virtual intptr_t GenerateShadowMapArray(uint32_t count);
        virtual void BeginShadowMap(const Light& light, const intptr_t shadowmap, uint32_t layer_index);
        virtual void EndShadowMap(const intptr_t shadowmap, uint32_t layer_index);
        virtual void SetShadowMap(const intptr_t shadowmap);
        virtual void DestroyShadowMap(intptr_t& shadowmap);

#ifdef _DEBUG
        virtual void DrawPoint(const Point& point, const Vector3f& color);
        virtual void DrawPointSet(const PointSet& point_set, const Vector3f& color);
        virtual void DrawPointSet(const PointSet& point_set, const Matrix4X4f& trans, const Vector3f& color);
        virtual void DrawLine(const Point& from, const Point& to, const Vector3f &color);
        virtual void DrawLine(const PointList& vertices, const Vector3f &color);
        virtual void DrawLine(const PointList& vertices, const Matrix4X4f& trans, const Vector3f &color);
        virtual void DrawTriangle(const PointList& vertices, const Vector3f &color);
        virtual void DrawTriangle(const PointList& vertices, const Matrix4X4f& trans, const Vector3f &color);
        virtual void DrawTriangleStrip(const PointList& vertices, const Vector3f &color);
        void DrawEdgeList(const EdgeList& edges, const Vector3f& color);
        void DrawPolygon(const Face& face, const Vector3f& color);
        void DrawPolygon(const Face& face, const Matrix4X4f& trans, const Vector3f& color);
        void DrawPolyhydron(const Polyhedron& polyhedron, const Vector3f& color);
        void DrawPolyhydron(const Polyhedron& polyhedron, const Matrix4X4f& trans, const Vector3f& color);
        void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color);
        virtual void ClearDebugBuffers();
#endif

    protected:
        virtual bool InitializeShaders();
        virtual void ClearShaders();
        virtual bool InitializeBuffers();
        virtual void ClearBuffers();

        virtual void InitConstants();
        virtual void InitCameraMatrix();
        virtual void CalculateCameraMatrix();
        virtual void CalculateLights();
        virtual void UpdateConstants();
        virtual void RenderBuffers();
#ifdef _DEBUG
        virtual void RenderDebugBuffers();
#endif

    protected:
        // Vector3f position, lookAt, up;
        // float fieldOfView;
		// float nearClipDistance;
		// float farClipDistance;
		// float screenAspect;

        // float mTheta = 0.5f * PI;
        // float mPhi = 0.5f * PI;
        // float mRadius = 3.0f;

        // int mLastMousePos_x;
        // int mLastMousePos_y;

        static const uint32_t kFrameCount = 2;
        static const uint32_t kMaxSceneObjectCount = 65535;
        static const uint32_t kMaxTextureCount = 2048;

        uint32_t m_nFrameIndex = 0;

        const int32_t kShadowMapWidth = 512;
        const int32_t kShadowMapHeight = 512;

        std::vector<Frame> m_Frames;
        std::vector<std::shared_ptr<IDrawPass>> m_DrawPasses;
    };

    extern GraphicsManager* g_pGraphicsManager;
}
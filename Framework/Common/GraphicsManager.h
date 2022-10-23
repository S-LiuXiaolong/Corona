#pragma once
#include "IRuntimeModule.h"
#include "geommath.h"
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

        // temporary. should be moved to scene manager and script engine (policy engine)
        // void WorldRotateX(float radians);
        // void WorldRotateY(float radians);
        // void WorldRotateZ(float radians);

		// void CameraRotateX(float radians);
		// void CameraRotateZ(float radians);
        // void CameraTranslationX(float distance);
        // void CameraTranslationY(float distance);
        // void OnMouseDown(int x, int y);
        // void OnMouseMoveL(int x, int y);
        // void OnMouseMoveR(int x, int y);
        // void OnMouseWheel(int delta);

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
        struct DrawFrameContext 
        {
            Matrix4X4f  m_worldMatrix;
//             Matrix4X4f  m_viewMatrix;
//             Matrix4X4f  m_projectionMatrix;
            Matrix4X4f  m_worldViewMatrix;
            Matrix4X4f  m_worldViewProjectionMatrix;
            Vector3f    m_lightPosition;
            Vector4f    m_lightColor;
            Vector4f    m_cameraPosition;
        };

        DrawFrameContext    m_DrawFrameContext;

        // TODO
        Matrix4X4f m_worldMatrix;
        Matrix4X4f m_viewMatrix;
        Matrix4X4f m_projectionMatrix;

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
    };

    extern GraphicsManager* g_pGraphicsManager;
}
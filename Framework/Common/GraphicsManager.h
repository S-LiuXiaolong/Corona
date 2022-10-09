#pragma once
#include "IRuntimeModule.h"
#include "geommath.h"

namespace Corona
{
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
        void WorldRotateX(float radians);
        void WorldRotateY(float radians);
        void WorldRotateZ(float radians);

		void CameraRotateX(float radians);
		void CameraRotateZ(float radians);
        void CameraTranslationX(float distance);
        void CameraTranslationY(float distance);
        void OnMouseDown(int x, int y);
        void OnMouseMoveL(int x, int y);
        void OnMouseMoveR(int x, int y);

    protected:
        bool SetPerFrameShaderParameters();
        bool SetPerBatchShaderParameters(const char* paramName, const Matrix4X4f& param);
        bool SetPerBatchShaderParameters(const char* paramName, const Vector3f& param);
        bool SetPerBatchShaderParameters(const char* paramName, const float param);
        bool SetPerBatchShaderParameters(const char* paramName, const int param);

        void InitConstants();
        bool InitializeShader(const char* vsFilename, const char* fsFilename);
        void InitializeBuffers();
        void CalculateCameraMatrix();
        void CalculateLights();
        void RenderBuffers();

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
        Matrix4X4f m_worldMatrix;
        Matrix4X4f m_viewMatrix;
        Matrix4X4f m_projectionMatrix;

        Vector3f position, lookAt, up;
        float fieldOfView;
		float nearClipDistance;
		float farClipDistance;
		float screenAspect;

        float mTheta = 0.0f;
        float mPhi = 0.2f * PI;
        float mRadius = 3.0f;

        int mLastMousePos_x;
        int mLastMousePos_y;
    };

    extern GraphicsManager* g_pGraphicsManager;
}
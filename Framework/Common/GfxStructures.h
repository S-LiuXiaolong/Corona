#pragma once
#include <vector>
#include "geommath.h"
#include "Scene.h"

namespace Corona
{
    struct Light
    {
        Vector3f    m_lightPosition;
        float       m_lightIntensity = 10.0f;
        Vector3f    m_lightColor;
        float       m_fallOffStart = 0.0f;
        Vector3f    m_lightDirection;
        float       m_fallOffEnd = PI / 4;
        Matrix4X4f  m_lightVP;
        int32_t     m_lightShadowMapIndex;
    };

    struct DrawFrameContext 
    {
        Matrix4X4f  m_worldMatrix;
        // Matrix4X4f  m_viewMatrix;
        // Matrix4X4f  m_projectionMatrix;
        Matrix4X4f  m_worldViewMatrix;
        Matrix4X4f  m_worldViewProjectionMatrix;
        Vector4f    m_cameraPosition;

        Light m_lights[3];
    };

    struct DrawBatchContext
    {
        std::shared_ptr<SceneNode> node;
        std::shared_ptr<SceneObjectMaterial> material;
        Matrix4X4f trans;

        virtual ~DrawBatchContext() = default;
    };

    struct Frame
    {
        DrawFrameContext frameContext;
        std::vector<std::shared_ptr<DrawBatchContext>> batchContexts;
        intptr_t shadowMap;
        uint32_t shadowMapCount;

        // TODO
        Matrix4X4f m_worldMatrix;
        Matrix4X4f m_viewMatrix;
        Matrix4X4f m_projectionMatrix;

        Frame()
        {
            shadowMap = -1;
            shadowMapCount = 0;
        }
    };

}
#pragma once
#include "BaseSceneObject.h"
#include "geommath.h"

namespace Corona
{
    class SceneObjectCamera : public BaseSceneObject
    {
    protected:
        std::string type;
        float m_fNearClipDistance;
        float m_fFarClipDistance;

    public:
        float GetNearClipDistance() const { return m_fNearClipDistance; };
        float GetFarClipDistance() const { return m_fFarClipDistance; };

    protected:
        // can only be used as base class
        SceneObjectCamera(void) : BaseSceneObject(SceneObjectType::kSceneObjectTypeCamera), m_fNearClipDistance(1.0f), m_fFarClipDistance(100.0f){};
        SceneObjectCamera(std::string Type, float NearClipDistance, float FarClipDistance)
            : BaseSceneObject(SceneObjectType::kSceneObjectTypeCamera),
              type(Type),
              m_fNearClipDistance(NearClipDistance),
              m_fFarClipDistance(FarClipDistance){};

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectCamera &obj);
    };

    class SceneObjectOrthogonalCamera : public SceneObjectCamera
    {
    protected:
        float m_fXMag;
        float m_fYMag;

    public:
        using SceneObjectCamera::SceneObjectCamera;
        SceneObjectOrthogonalCamera(std::string Type, float NearClipDistance, float FarClipDistance, float XMag = 16.0f, float YMag = 9.0f)
            : SceneObjectCamera(Type, NearClipDistance, FarClipDistance), m_fXMag(XMag), m_fYMag(YMag){};

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectOrthogonalCamera &obj);
    };

    class SceneObjectPerspectiveCamera : public SceneObjectCamera
    {
    protected:
        float m_fAspect;
        float m_fFov;

    public:
        using SceneObjectCamera::SceneObjectCamera;
        SceneObjectPerspectiveCamera(std::string Type, float NearClipDistance, float FarClipDistance, float Aspect = 16.0f / 9.0f, float fov = PI / 2.0)
            : SceneObjectCamera(Type, NearClipDistance, FarClipDistance), m_fAspect(Aspect), m_fFov(fov){};
            
        float GetFov() const { return m_fFov; };

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectPerspectiveCamera &obj);
    };

}
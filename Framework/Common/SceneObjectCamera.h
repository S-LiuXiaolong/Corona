#pragma once
#include "BaseSceneObject.h"
#include "geommath.h"

namespace Corona
{
    class SceneObjectCamera : public BaseSceneObject
    {
    protected:
        float m_fAspect;
        float m_fNearClipDistance;
        float m_fFarClipDistance;

    public:
        void SetParam(std::string &attrib, float param)
        {
            if (attrib == "near")
            {
                m_fNearClipDistance = param;
            }
            else if (attrib == "far")
            {
                m_fFarClipDistance = param;
            }
        };

        float GetNearClipDistance() const { return m_fNearClipDistance; };
        float GetFarClipDistance() const { return m_fFarClipDistance; };

    protected:
        // can only be used as base class
        SceneObjectCamera(void) : BaseSceneObject(SceneObjectType::kSceneObjectTypeCamera), m_fAspect(16.0f / 9.0f), m_fNearClipDistance(1.0f), m_fFarClipDistance(100.0f){};

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectCamera &obj);
    };

    class SceneObjectOrthogonalCamera : public SceneObjectCamera
    {
    public:
        using SceneObjectCamera::SceneObjectCamera;

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectOrthogonalCamera &obj);
    };

    class SceneObjectPerspectiveCamera : public SceneObjectCamera
    {
    protected:
        float m_fFov;

    public:
        void SetParam(std::string &attrib, float param)
        {
            if (attrib == "fov")
            {
                m_fFov = param;
            }
            SceneObjectCamera::SetParam(attrib, param);
        };

    public:
        SceneObjectPerspectiveCamera(float fov = PI / 2.0) : SceneObjectCamera(), m_fFov(fov){};
        float GetFov() const { return m_fFov; };

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectPerspectiveCamera &obj);
    };

}
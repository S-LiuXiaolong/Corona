#pragma once
#include "BaseSceneObject.h"
#include "SceneObjectDef.h"

namespace Corona
{
    class SceneObjectLight : public BaseSceneObject
    {
    protected:
        Vector4f m_LightColor;
        float m_fIntensity;
        float m_fRange; // 0.0 = infinite
        bool m_bCastShadows;

    public:
        void SetIfCastShadow(bool shadow) { m_bCastShadows = shadow; };

        void SetColor(std::string &attrib, Vector4f &color)
        {
            if (attrib == "light")
            {
                m_LightColor = color;
            }
        };

        void SetParam(std::string &attrib, float param)
        {
            if (attrib == "intensity")
            {
                m_fIntensity = param;
            }
        };

        const Vector4f &GetColor() { return m_LightColor; };
        float GetIntensity() { return m_fIntensity; };

    protected:
        // can only be used as base class of delivered lighting objects
        SceneObjectLight(void) : BaseSceneObject(SceneObjectType::kSceneObjectTypeLight),
                                m_LightColor(Vector4f(1.0f)),
                                m_fIntensity(100.0f),
                                m_bCastShadows(false),
                                m_fRange(0.0f){};

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectLight &obj);
    };

    class SceneObjectOmniLight : public SceneObjectLight
    {
    public:
        using SceneObjectLight::SceneObjectLight;

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectOmniLight &obj);
    };

    class SceneObjectSpotLight : public SceneObjectLight
    {
    protected:
        float m_fInnerConeAngle;
        float m_fOuterConeAngle;

    public:
        SceneObjectSpotLight(void) : SceneObjectLight(), m_fInnerConeAngle(0.0f), m_fOuterConeAngle(0.7853981634f){}; // PI / 4 = 0.785

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectSpotLight &obj);
    };

    class SceneObjectInfiniteLight : public SceneObjectLight
    {
    public:
        using SceneObjectLight::SceneObjectLight;

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectInfiniteLight &obj);
    };

}
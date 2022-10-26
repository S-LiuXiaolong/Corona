#pragma once
#include "BaseSceneObject.h"
#include "SceneObjectDef.h"

namespace Corona
{
	ENUM(AttenCurveType) {
		kLinear = 0,
			kSmooth = 1,
			kInverse = 2,
			kInverseSquare = 3
	};

	struct AttenCurve {
		AttenCurveType type;
		union AttenCurveParams {
			struct LinearParam { float begin_atten; float end_atten; } linear_params;
			struct SmoothParam { float begin_atten; float end_atten; } smooth_params;
			struct InverseParam { float scale; float offset; float kl; float kc; } inverse_params;
			struct InverseSquareParam { float scale; float offset; float kq; float kl; float kc; } inverse_squre_params;
		} u;

		AttenCurve() : type(AttenCurveType::kLinear),
			u({ {0.0f, 1.0f} })
		{}
	};

    class SceneObjectLight : public BaseSceneObject
    {
    protected:
        Vector4f m_LightColor;
        float m_fIntensity;
        float m_fRange; // 0.0 = infinite
        bool m_bCastShadows;

    public:
        std::string m_type;

    public:
        void SetIfCastShadow(bool shadow) { m_bCastShadows = shadow; };

        // TODO: clean these
        void SetColor(std::string attrib, Vector4f& color)
        {
            if (attrib == "light")
            {
                m_LightColor = color;
            }
        };

        void SetParam(std::string attrib, float param)
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
        SceneObjectLight(const SceneObjectType type) : BaseSceneObject(type),
                                m_LightColor(Vector4f(1.0f)),
                                m_fIntensity(100.0f),
                                m_bCastShadows(false),
                                m_fRange(0.0f){};

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectLight &obj);
    };

    class SceneObjectOmniLight : public SceneObjectLight
    {
    public:
        SceneObjectOmniLight(void) : SceneObjectLight(SceneObjectType::kSceneObjectTypeLightOmni) {}

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectOmniLight &obj);
    };

    class SceneObjectSpotLight : public SceneObjectLight
    {
    protected:
        float m_fInnerConeAngle;
        float m_fOuterConeAngle;

    public:

        void SetInnerConeAngle(float innerConeAngle)
        {
            m_fInnerConeAngle = innerConeAngle;
        }
        void SetOuterConeAngle(float outerConeAngle)
        {
            m_fOuterConeAngle = outerConeAngle;
        }
        float GetInnerConeAngle()
        {
            return m_fInnerConeAngle;
        }
        float GetOuterConeAngle()
        {
            return m_fOuterConeAngle;
        }
        SceneObjectSpotLight(void) : SceneObjectLight(SceneObjectType::kSceneObjectTypeLightSpot), m_fInnerConeAngle(0.0f), m_fOuterConeAngle(0.7853981634f){}; // PI / 4 = 0.785

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectSpotLight &obj);
    };

    class SceneObjectInfiniteLight : public SceneObjectLight
    {
    public:
        SceneObjectInfiniteLight(void) : SceneObjectLight(SceneObjectType::kSceneObjectTypeLightInfi) {}

        friend std::ostream &operator<<(std::ostream &out, const SceneObjectInfiniteLight &obj);
    };

}
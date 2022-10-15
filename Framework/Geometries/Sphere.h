#pragma once
#include "Geometry.h"

namespace Corona {
    class Sphere : public Geometry
    {
    public:
        Sphere() = delete;
        Sphere(const float radius) : Geometry(GeometryType::kSphere), m_fRadius(radius) {};

        void GetAabb(const Matrix4X4f& trans, 
                                Vector3f& aabbMin, 
                                Vector3f& aabbMax) const final;

        float GetRadius() const { return m_fRadius; };

    protected:
        float m_fRadius;
    };
}
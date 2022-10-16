#pragma once
#include "BaseSceneObject.h"
#include "SceneObjectPrimitive.h"

namespace Corona
{
    class SceneObjectMesh : public BaseSceneObject
    {
    protected:
        std::vector<std::shared_ptr<SceneObjectPrimitive>> m_Primitive;
        // SceneObjectCollisionType m_CollisionType;
        // float       m_CollisionParameters[10];

    public:
        SceneObjectMesh(void) : BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh) {}

        // void SetCollisionType(SceneObjectCollisionType collision_type) { m_CollisionType = collision_type; }
        // const SceneObjectCollisionType CollisionType() const { return  m_CollisionType; }
        // void SetCollisionParameters(const float* param, int32_t count)
        // {
        //     assert(count > 0 && count < 10);
        //     memcpy(m_CollisionParameters, param, sizeof(float) * count);
        // }
        // const float* CollisionParameters() const { return m_CollisionParameters; }

        void AddPrimitive(std::shared_ptr<SceneObjectPrimitive>& primitive) { m_Primitive.push_back(std::move(primitive)); }
        // TODO
        // const std::weak_ptr<SceneObjectPrimitive> GetMesh() { return (m_Primitive.empty()? nullptr : m_Primitive[0]); }
        // const std::weak_ptr<SceneObjectPrimitive> GetMeshLOD(size_t lod) { return (lod < m_Primitive.size()? m_Primitive[lod] : nullptr); }
        // BoundingBox GetBoundingBox() const { return m_Primitive.empty()? BoundingBox() : m_Primitive[0]->GetBoundingBox(); }
        // ConvexHull GetConvexHull() const { return m_Primitive.empty()? ConvexHull() : m_Primitive[0]->GetConvexHull(); }

        friend std::ostream& operator<<(std::ostream& out, const SceneObjectMesh& obj);
    };
}
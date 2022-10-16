#pragma once
#include <vector>
#include <memory>
#include "SceneObjectDef.h"
#include "SceneObjectIndexArray.h"
#include "SceneObjectVertexArray.h"
#include "BaseSceneObject.h"
#include "geommath.h"

namespace Corona
{
    class SceneObjectPrimitive : public BaseSceneObject
    {
    protected:
        // TODO: maybe unique or shared ?
        SceneObjectVertexArray m_VertexArray;
        SceneObjectIndexArray m_IndexArray;
        // TODO: use types to draw different styles to draw primitives in one mesh(/geometry)
        // PrimitiveType m_PrimitiveType;

    public:
        SceneObjectPrimitive() : BaseSceneObject(SceneObjectType::kSceneObjectTypeMesh){};
        SceneObjectPrimitive(SceneObjectPrimitive &&primitive)
            : BaseSceneObject(SceneObjectType::kSceneObjectTypePrimitive),
              m_IndexArray(std::move(primitive.m_IndexArray)),
              m_VertexArray(std::move(primitive.m_VertexArray)) {};
        SceneObjectPrimitive(SceneObjectVertexArray&& vertex, SceneObjectIndexArray&& index)
             : BaseSceneObject(SceneObjectType::kSceneObjectTypePrimitive),
              m_IndexArray(std::move(m_IndexArray)),
              m_VertexArray(std::move(m_VertexArray)) {};

        // void SetPrimitiveType(PrimitiveType type) { m_PrimitiveType = type;  };

        size_t GetIndexCount() const { return m_IndexArray.GetIndexCount(); };
        size_t GetVertexCount() const { return m_VertexArray.GetVertexCount(); };
        // const PrimitiveType& GetPrimitiveType() { return m_PrimitiveType; };
        // BoundingBox GetBoundingBox() const;
        // ConvexHull GetConvexHull() const;

        friend std::ostream& operator<<(std::ostream& out, const SceneObjectMesh& obj);
    };
}
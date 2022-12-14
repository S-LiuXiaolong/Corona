#pragma once
#include <vector>
#include <memory>
#include "SceneObjectDef.h"
#include "BaseSceneObject.h"
#include "geommath.h"

namespace Corona
{
    struct VertexBasicAttribs
    {
        Vector3f pos;
        Vector3f normal;
        Vector2f uv0;
        Vector3f tangent;
    };

    class SceneObjectPrimitive : public BaseSceneObject
    {
    protected:
        // TODO: maybe unique or shared ?
        std::vector<VertexBasicAttribs> m_VertexArray;
        std::vector<uint32_t> m_IndexArray;
        // TODO: use types to draw different styles to draw primitives in one mesh(/geometry)
        // PrimitiveType m_PrimitiveType;

    public:
        SceneObjectPrimitive() : BaseSceneObject(SceneObjectType::kSceneObjectTypePrimitive){};
        SceneObjectPrimitive(SceneObjectPrimitive &&primitive)
            : BaseSceneObject(SceneObjectType::kSceneObjectTypePrimitive),
              m_IndexArray(std::move(primitive.m_IndexArray)),
              m_VertexArray(std::move(primitive.m_VertexArray)) {};
		SceneObjectPrimitive(std::vector<VertexBasicAttribs>& vertex, std::vector<uint32_t>& index)
			: BaseSceneObject(SceneObjectType::kSceneObjectTypePrimitive),
			m_IndexArray(index),
			m_VertexArray(vertex) {};
        SceneObjectPrimitive(std::vector<VertexBasicAttribs>&& vertex, std::vector<uint32_t>&& index)
             : BaseSceneObject(SceneObjectType::kSceneObjectTypePrimitive),
              m_IndexArray(std::move(index)),
              m_VertexArray(std::move(vertex)) {};

        // void SetPrimitiveType(PrimitiveType type) { m_PrimitiveType = type;  };

        size_t GetVertexCount() const { return m_VertexArray.size(); };
        size_t GetIndexCount() const { return m_IndexArray.size(); };
        std::vector<VertexBasicAttribs>& GetVertexData() { return m_VertexArray; };
        std::vector<uint32_t>& GetIndexData() { return m_IndexArray; };
        // const PrimitiveType& GetPrimitiveType() { return m_PrimitiveType; };
        // BoundingBox GetBoundingBox() const;
        // ConvexHull GetConvexHull() const;

        friend std::ostream& operator<<(std::ostream& out, const SceneObjectPrimitive& obj);
    };
}
#pragma once
#include "SceneObjectDef.h"

namespace Corona
{
    // TODO
    struct VertexBasicAttribs
    {
        Vector3f pos;
        Vector3f normal;
        Vector2f uv0;
        Vector3f tangent;
    };

    class SceneObjectVertexArray 
    {
    protected:
        const void*      m_pData;
        const size_t     m_szData;

    public:
        SceneObjectVertexArray(const void* data = nullptr, const size_t data_size = 0) : m_pData(data), m_szData(data_size) {};
        SceneObjectVertexArray(SceneObjectVertexArray& arr) = default; 
        SceneObjectVertexArray(SceneObjectVertexArray&& arr) = default; 

        size_t GetDataSize() const 
        { 
            size_t size = m_szData * sizeof(float);
            return size;
        }; 
        const void* GetData() const { return m_pData; };
        size_t GetVertexCount() const
        {
            // TODO
            size_t size = m_szData / sizeof(VertexBasicAttribs);
            return size;
        }

        friend std::ostream& operator<<(std::ostream& out, const SceneObjectVertexArray& obj);
    };
}
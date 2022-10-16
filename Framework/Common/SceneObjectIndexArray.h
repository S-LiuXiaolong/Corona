#pragma once
#include "SceneObjectDef.h"

namespace Corona
{
    class SceneObjectIndexArray
    {
    protected:
        const void*       m_pData;
        const size_t      m_szData;

    public:
        SceneObjectIndexArray(const void* data = nullptr, const size_t data_size = 0) 
            : m_pData(data), m_szData(data_size) {};
        SceneObjectIndexArray(SceneObjectIndexArray& arr) = default;
        SceneObjectIndexArray(SceneObjectIndexArray&& arr) = default;

        const void* GetData() const { return m_pData; };
        size_t GetDataSize() const 
        {
            size_t size = m_szData * sizeof(int16_t);
            return size;
        };

        size_t GetIndexCount() const
        {
            return m_szData;
        }

        friend std::ostream& operator<<(std::ostream& out, const SceneObjectIndexArray& obj);
    };
}
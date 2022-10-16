#pragma once
#include "crossguid/Guid.hpp"
#include "portable.h"

namespace Corona
{
    ENUM(SceneObjectType) {
        kSceneObjectTypeMesh    =   "MESH"_i32,
        kSceneObjectTypeMaterial=   "MATL"_i32,
        kSceneObjectTypeTexture =   "TXTU"_i32,
        kSceneObjectTypeLight   =   "LGHT"_i32,
        kSceneObjectTypeCamera  =   "CAMR"_i32,
        kSceneObjectTypeAnimationClip =   "ANIM"_i32,
        kSceneObjectTypeClip    =   "CLIP"_i32,
        kSceneObjectTypeVertexArray   =   "VARR"_i32,
        kSceneObjectTypeIndexArray    =   "VARR"_i32,
        kSceneObjectTypePrimitive =  "PRIM"_i32,
        kSceneObjectTypeTransform =  "TRFM"_i32,
        kSceneObjectTypeTranslate =  "TSLT"_i32,
        kSceneObjectTypeRotate =  "ROTA"_i32,
        kSceneObjectTypeScale =  "SCAL"_i32,
        kSceneObjectTypeTrack = "TRAC"_i32
    };

    using namespace xg;
    class BaseSceneObject
    {
        protected:
            Guid m_Guid;
            SceneObjectType m_Type;
        protected:
            // can only be used as base class
            BaseSceneObject(SceneObjectType type) : m_Type(type) { m_Guid = newGuid(); };
            BaseSceneObject(Guid& guid, SceneObjectType type) : m_Guid(guid), m_Type(type) {};
            BaseSceneObject(Guid&& guid, SceneObjectType type) : m_Guid(std::move(guid)), m_Type(type) {};
            BaseSceneObject(BaseSceneObject&& obj) : m_Guid(std::move(obj.m_Guid)), m_Type(obj.m_Type) {};
            BaseSceneObject& operator=(BaseSceneObject&& obj) { this->m_Guid = std::move(obj.m_Guid); this->m_Type = obj.m_Type; return *this; };
            virtual ~BaseSceneObject() {};
            
        private:
            // a type must be specified
            BaseSceneObject() = delete; 
            // can not be copied
            BaseSceneObject(BaseSceneObject& obj) = delete;
            BaseSceneObject& operator=(BaseSceneObject& obj) = delete;

        public:
            const Guid& GetGuid() const { return m_Guid; };
            const SceneObjectType GetType() const { return m_Type; };

        friend std::ostream& operator<<(std::ostream& out, const BaseSceneObject& obj);
    };
}
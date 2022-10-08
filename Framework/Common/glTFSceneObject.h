#pragma once
#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "crossguid/guid.hpp"
//#include "portable.h"
//#include "Image.h"
#include "geommath.h"

namespace tinygltf
{
    class Node;
    class Model;
    struct Mesh;
    struct Material;
}

// You have to rename the two SceneObject namespace to GLTF and OGEX
namespace Corona
{
    namespace details
    {
        constexpr int32_t i32(const char *s, int32_t v)
        {
            return *s ? i32(s + 1, v * 256 + *s) : v;
        }
    }

    constexpr int32_t operator"" _i32(const char *s, size_t)
    {
        return details::i32(s, 0);
    }

    // use ascii to get id of SceneObjectTypes
    enum SceneObjectType
    {
        SceneObjectTypePrimitive = "PRIM"_i32,
        SceneObjectTypeMesh = "MESH"_i32,
        SceneObjectTypeCamera = "CAMR"_i32,
        SceneObjectTypeVertexArray = "VARR"_i32,
        SceneObjectTypeIndexArray = "VARR"_i32
    };

    std::ostream &operator<<(std::ostream &out, SceneObjectType type);

    /// @brief Some util struct maybe used by other functions
    struct VertexBasicAttribs
    {
        Vector3f pos;
        Vector3f normal;
        Vector2f uv0;
        Vector2f uv1;
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
        BaseSceneObject(Guid &guid, SceneObjectType type) : m_Guid(guid), m_Type(type){};
        BaseSceneObject(Guid &&guid, SceneObjectType type) : m_Guid(std::move(guid)), m_Type(type){};
        BaseSceneObject(BaseSceneObject &&obj) : m_Guid(std::move(obj.m_Guid)), m_Type(obj.m_Type){};
        BaseSceneObject &operator=(BaseSceneObject &&obj)
        {
            this->m_Guid = std::move(obj.m_Guid);
            this->m_Type = obj.m_Type;
            return *this;
        };
        virtual ~BaseSceneObject(){};

    private:
        // a type must be specified
        BaseSceneObject() = delete;
        // can not be copied
        BaseSceneObject(BaseSceneObject &obj) = delete;
        BaseSceneObject &operator=(BaseSceneObject &obj) = delete;

    public:
        const Guid &GetGuid() const { return m_Guid; };
        const SceneObjectType GetType() const { return m_Type; };

        friend std::ostream &operator<<(std::ostream &out, const BaseSceneObject &obj);
    };

    // class SceneObjectVertexArray
    // {
    //     
    // };

    class SceneObjectPrimitive : public BaseSceneObject
    {
        // To be rewrite to add vertexBuffer and indexBuffer here.
    public:
        const uint32_t FirstIndex;
        const uint32_t IndexCount;
        const uint32_t VertexCount;
        // const uint32_t MaterialId;

        // const BoundBox BB;

        // TODO: These two are for test and can be changes to void* when used to add into buffers.
        std::vector<VertexBasicAttribs> VertexData;
        std::vector<uint32_t> IndexData;

    public:
        SceneObjectPrimitive(uint32_t _FirstIndex,
                             uint32_t _IndexCount,
                             uint32_t _VertexCount,
                             std::vector<VertexBasicAttribs> &&_VertexData,
                             std::vector<uint32_t> &&_IndexData) : BaseSceneObject(SceneObjectType::SceneObjectTypePrimitive),
                                                                   FirstIndex{_FirstIndex},
                                                                   IndexCount{_IndexCount},
                                                                   VertexCount{_VertexCount},
                                                                   VertexData{_VertexData},
                                                                   IndexData{_IndexData}
        {
            // TODO: Add function here.
        }

        SceneObjectPrimitive(SceneObjectPrimitive &&) = default;

        bool HasIndices() const
        {
            return IndexCount > 0;
        }
    };

    class SceneObjectMesh : BaseSceneObject
    {
    public:
        std::vector<std::shared_ptr<SceneObjectPrimitive>> Primitives;

        struct TransformData
        {
            Matrix4X4f matrix;
            // std::vector<Matrix4X4f> jointMatrices;
        };

        TransformData Transforms;

    public:
        SceneObjectMesh(const Matrix4X4f &matrix) : BaseSceneObject(SceneObjectType::SceneObjectTypeMesh)
        {
            Transforms.matrix = matrix;
        }
    };

    // struct Camera
    // {
    //     enum class Projection
    //     {
    //         Unknown,
    //         Perspective,
    //         Orthographic
    //     } Type = Projection::Unknown;

    //     std::string Name;

    //     struct PerspectiveAttribs
    //     {
    //         float AspectRatio;
    //         float YFov;
    //         float ZNear;
    //         float ZFar;
    //     };
    //     struct OrthographicAttribs
    //     {
    //         float XMag;
    //         float YMag;
    //         float ZNear;
    //         float ZFar;
    //     };
    //     union
    //     {
    //         PerspectiveAttribs  Perspective = {};
    //         OrthographicAttribs Orthographic;
    //     };

    //     Matrix4X4f matrix;
    // };
    class SceneObjectCamera : public BaseSceneObject
    {
    public:
        // TODO
        std::string Name;
        Matrix4X4f matrix;

        float ZNear;
        float ZFar;

    protected:
        // can only be used as base class
        SceneObjectCamera(float near_clip = 1.0f,
                          float far_clip = 100.0f) : BaseSceneObject(SceneObjectType::SceneObjectTypeCamera),
                                                     ZNear(near_clip),
                                                     ZFar(far_clip)
        {
        }
    };

    class SceneObjectOrthogonalCamera : public SceneObjectCamera
    {
    public:
        float XMag;
        float YMag;

    public:
        SceneObjectOrthogonalCamera(float xMag,
                                    float yMag,
                                    float near_clip = 1.0f,
                                    float far_clip = 100.0f) : SceneObjectCamera(near_clip, far_clip),
                                                               XMag(xMag),
                                                               YMag(yMag)
        {
        }
    };

    class SceneObjectPerspectiveCamera : public SceneObjectCamera
    {
    public:
        float AspectRatio;
        float YFov;

    public:
        SceneObjectPerspectiveCamera(float aspectRatio,
                                     float yFov,
                                     float near_clip = 1.0f,
                                     float far_clip = 100.0f) : SceneObjectCamera(near_clip, far_clip),
                                                                AspectRatio(aspectRatio),
                                                                YFov(yFov)
        {
        }
    };

    struct SceneNode
    {
        std::string Name;
        SceneNode *Parent = nullptr;
        uint32_t Index;

        std::vector<std::unique_ptr<SceneNode>> Children;

        Matrix4X4f Matrix;
        std::shared_ptr<SceneObjectMesh> pMesh;
        std::shared_ptr<SceneObjectCamera> pCamera;
        Vector3f Translation;
        Vector3f Scale;
        Quaternion Rotation;

        Matrix4X4f LocalMatrix() const;
        Matrix4X4f GetMatrix() const;
        void UpdateTransforms();
    };

    struct Model
    {
        /// Model create information
        struct CreateInfo
        {
            std::string FileName = "";

            CreateInfo() = default;
            explicit CreateInfo(std::string _FileName) : FileName(_FileName){};
        };
        // struct VertexSkinAttribs
        // {
        //     Vector4f joint0;
        //     Vector4f weight0;
        // };

        // enum BUFFER_ID
        // {
        //     BUFFER_ID_VERTEX_BASIC_ATTRIBS = 0,
        //     BUFFER_ID_VERTEX_SKIN_ATTRIBS,
        //     BUFFER_ID_INDEX,
        //     BUFFER_ID_NUM_BUFFERS
        // };
        // int32_t IndexCount = 0;

        std::vector<std::unique_ptr<SceneNode>> Nodes;
        std::vector<SceneNode *> LinearNodes;

        // std::vector<std::unique_ptr<Material>> Materials;
        std::vector<std::string> Extensions;

        Model();
        // Model(const CreateInfo &CI);
        ~Model();

    // private:
    //     void LoadFromFile(const CreateInfo &CI);

    //     struct ConvertedBufferViewKey
    //     {
    //         int PosAccess = -1;
    //         int UV0Access = -1;
    //         int UV1Access = -1;
    //         int NormAccess = -1;
    //         // int JointAccess = -1;
    //         // int WeightAccess = -1;

    //         bool operator==(const ConvertedBufferViewKey& Rhs) const
    //         {
    //             return PosAccess == Rhs.PosAccess &&
    //                 UV0Access == Rhs.UV0Access &&
    //                 UV1Access == Rhs.UV1Access &&
    //                 NormAccess == Rhs.NormAccess;
	// 				// JointAccess == Rhs.JointAccess &&
	// 				// WeightAccess == Rhs.WeightAccess;
    //         }

    //         struct Hasher
    //         {
    //             // TODO: add hash function here
    //             size_t operator()(const ConvertedBufferViewKey& Key) const
    //             {
    //                 return 0;
    //             }
    //         };
    //     };

    //     struct ConvertedBufferViewData
    //     {
    //         size_t VertexBasicDataOffset = ~size_t(0);
    //         size_t VertexSkinDataOffset = ~size_t(0);

    //         bool IsInitialized() const { return VertexBasicDataOffset != ~size_t(0); }
    //     };

    //     using ConvertedBufferViewMap = std::unordered_map<ConvertedBufferViewKey, ConvertedBufferViewData, ConvertedBufferViewKey::Hasher>;

    //     void LoadNode(SceneNode *parent,
    //                   const tinygltf::Node &gltf_node,
    //                   uint32_t nodeIndex,
    //                   const tinygltf::Model &gltf_model,
    //                   std::vector<uint32_t> &IndexData,
    //                   std::vector<VertexBasicAttribs> &VertexBasicData,
    //                   ConvertedBufferViewMap &ConvertedBuffers);

    //     void ConvertBuffers(const ConvertedBufferViewKey &Key,
    //                         ConvertedBufferViewData &Data,
    //                         const tinygltf::Model &gltf_model,
    //                         std::vector<VertexBasicAttribs> &VertexBasicData) const;

    //     // TODO: put all index and vertex data into every primitive
    //     void UpdatePrimitiveData();
    };
}

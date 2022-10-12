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
#include "Image.h"
#include "AssetLoader.h"
#include "BMP.h"
#include "JPEG.h"
#include "PNG.h"

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


    // use ascii to get id of SceneObjectTypes
    enum SceneObjectType
    {
        SceneObjectTypePrimitive = "PRIM"_i32,
        SceneObjectTypeMesh = "MESH"_i32,
        SceneObjectTypeMaterial=   "MATL"_i32,
        SceneObjectTypeTexture =   "TXTU"_i32,
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
        Vector3f tangent;
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

	class SceneObjectTexture : public BaseSceneObject
	{
	protected:
        // TODO: Add sampler (?)
		std::string m_Name;
        // std::shared_ptr<Sampler> m_pSampler;
		std::shared_ptr<Image> m_pImage;

	public:
		SceneObjectTexture() : BaseSceneObject(SceneObjectType::SceneObjectTypeTexture) {};
		SceneObjectTexture(const std::string& name) : BaseSceneObject(SceneObjectType::SceneObjectTypeTexture), m_Name(name) {};
		SceneObjectTexture(std::shared_ptr<Image>& image) : BaseSceneObject(SceneObjectType::SceneObjectTypeTexture), m_pImage(image) {};
		SceneObjectTexture(std::shared_ptr<Image>&& image) : BaseSceneObject(SceneObjectType::SceneObjectTypeTexture), m_pImage(std::move(image)) {};
		SceneObjectTexture(SceneObjectTexture&) = default;
		SceneObjectTexture(SceneObjectTexture&&) = default;
		void SetName(const std::string& name) { m_Name = name; };
		void SetName(std::string&& name) { m_Name = std::move(name); };
		const std::string& GetName() const { return m_Name; };

		Image& GetTextureImage()
		{
			return *m_pImage;
		};

		friend std::ostream& operator<<(std::ostream& out, const SceneObjectTexture& obj);
	};

    // class SceneObjectMaterial : public BaseSceneObject
    struct SceneObjectMaterial
    {
    public:
        enum PBR_WORKFLOW
        {
            PBR_WORKFLOW_METALL_ROUGH = 0,
            PBR_WORKFLOW_SPEC_GLOSS
        };

        enum ALPHA_MODE
        {
            ALPHA_MODE_OPAQUE = 0,
            ALPHA_MODE_MASK,
            ALPHA_MODE_BLEND,
            ALPHA_MODE_NUM_MODES
        };

        // Material attributes packed in a shader-friendly format
        struct ShaderAttribs
        {
            Vector4f BaseColorFactor = Vector4f{1, 1, 1, 1};
            Vector4f EmissiveFactor  = Vector4f{1, 1, 1, 1};
            Vector4f SpecularFactor  = Vector4f{1, 1, 1, 1};

            int   Workflow                     = PBR_WORKFLOW_METALL_ROUGH;
            float BaseColorUVSelector          = -1;
            float PhysicalDescriptorUVSelector = -1;
            float NormalUVSelector             = -1;

            float OcclusionUVSelector     = -1;
            float EmissiveUVSelector      = -1;
            float BaseColorSlice          = 0;
            float PhysicalDescriptorSlice = 0;

            float NormalSlice    = 0;
            float OcclusionSlice = 0;
            float EmissiveSlice  = 0;
            float MetallicFactor = 1;

            float RoughnessFactor = 1;
            int   AlphaMode       = ALPHA_MODE_OPAQUE;
            float AlphaCutoff     = 0.5f;
            float Dummy0          = 0;

            // When texture atlas is used, UV scale and bias applied to
            // each texture coordinate set
            Vector4f BaseColorUVScaleBias          = Vector4f{1, 1, 0, 0};
            Vector4f PhysicalDescriptorUVScaleBias = Vector4f{1, 1, 0, 0};
            Vector4f NormalUVScaleBias             = Vector4f{1, 1, 0, 0};
            Vector4f OcclusionUVScaleBias          = Vector4f{1, 1, 0, 0};
            Vector4f EmissiveUVScaleBias           = Vector4f{1, 1, 0, 0};

            // Any user-specific data
            Vector4f CustomData = Vector4f{0, 0, 0, 0};
        };

        enum TEXTURE_ID
        {
            // Base color for metallic-roughness workflow or
            // diffuse color for specular-glossinees workflow
            TEXTURE_ID_BASE_COLOR = 0,

            // Metallic-roughness or specular-glossinees map
            TEXTURE_ID_PHYSICAL_DESC,

            TEXTURE_ID_NORMAL_MAP,
            TEXTURE_ID_OCCLUSION,
            TEXTURE_ID_EMISSIVE,
            TEXTURE_ID_NUM_TEXTURES
        };

        ShaderAttribs Attribs;

        bool DoubleSided = false;

        // Texture indices in Model.Textures array
        std::array<int, TEXTURE_ID_NUM_TEXTURES> TextureIds = {-1, -1, -1, -1, -1};
        std::unordered_map<int, std::shared_ptr<SceneObjectTexture>> Textures;
    public:
        // SceneObjectMaterial() : BaseSceneObject(SceneObjectType::SceneObjectTypeMaterial) {};
    };

}

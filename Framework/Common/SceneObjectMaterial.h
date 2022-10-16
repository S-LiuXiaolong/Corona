#pragma once
#include <string>
#include "geommath.h"
#include "BaseSceneObject.h"
#include "SceneObjectTexture.h"
#include "SceneObjectDef.h"

namespace Corona
{
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
        Vector4f EmissiveFactor = Vector4f{1, 1, 1, 1};
        Vector4f SpecularFactor = Vector4f{1, 1, 1, 1};

        int Workflow = PBR_WORKFLOW_METALL_ROUGH;
        float BaseColorUVSelector = -1;
        float PhysicalDescriptorUVSelector = -1;
        float NormalUVSelector = -1;

        float OcclusionUVSelector = -1;
        float EmissiveUVSelector = -1;
        float BaseColorSlice = 0;
        float PhysicalDescriptorSlice = 0;

        float NormalSlice = 0;
        float OcclusionSlice = 0;
        float EmissiveSlice = 0;
        float MetallicFactor = 1;

        float RoughnessFactor = 1;
        int AlphaMode = ALPHA_MODE_OPAQUE;
        float AlphaCutoff = 0.5f;
        float Dummy0 = 0;

        // When texture atlas is used, UV scale and bias applied to
        // each texture coordinate set
        Vector4f BaseColorUVScaleBias = Vector4f{1, 1, 0, 0};
        Vector4f PhysicalDescriptorUVScaleBias = Vector4f{1, 1, 0, 0};
        Vector4f NormalUVScaleBias = Vector4f{1, 1, 0, 0};
        Vector4f OcclusionUVScaleBias = Vector4f{1, 1, 0, 0};
        Vector4f EmissiveUVScaleBias = Vector4f{1, 1, 0, 0};

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

    class SceneObjectMaterial : public BaseSceneObject
    {
    protected:
        std::string m_Name;
        ShaderAttribs m_Attribs;

        // Texture indices in Model.Textures array
        std::array<int, TEXTURE_ID_NUM_TEXTURES> TextureIds;
        std::unordered_map<int, std::shared_ptr<SceneObjectTexture>> Textures;

    public:
        SceneObjectMaterial(void) : BaseSceneObject(SceneObjectType::kSceneObjectTypeMaterial),
                                    m_Name(""),
                                    TextureIds({-1, -1, -1, -1, -1}){};
        SceneObjectMaterial(const char *name) : SceneObjectMaterial()
        {
            m_Name = name;
        };
        SceneObjectMaterial(const std::string &name) : SceneObjectMaterial()
        {
            m_Name = name;
        };
        SceneObjectMaterial(std::string &&name) : SceneObjectMaterial()
        {
            m_Name = std::move(name);
        };

        const std::string& GetName() const { return m_Name; };
        void SetName(const std::string& name) { m_Name = name; };
        void SetName(std::string&& name) { m_Name = std::move(name); };
        ShaderAttribs& GetShaderAttribs() { return m_Attribs; };

        // TODO
        void LoadMaterial()
        {

        }

        std::shared_ptr<SceneObjectTexture> GetColorMap();
        std::shared_ptr<SceneObjectTexture> GetPhysicsDescriptorMap();
        std::shared_ptr<SceneObjectTexture> GetNormalMap();
        std::shared_ptr<SceneObjectTexture> GetAOMap();
        std::shared_ptr<SceneObjectTexture> GetEmissiveMap();
    };
}
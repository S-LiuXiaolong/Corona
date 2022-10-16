#pragma once
#include <unordered_map>
#include "SceneNode.h"
#include "tinyglTF/tiny_gltf.h"
#include "SceneParser.h"

namespace tinygltf
{
    class Node;
    class Model;
    struct Mesh;
    struct Material;
}

namespace Corona
{
    class GltfParser : implements SceneParser
    {
    private:
        struct ConvertedBufferViewKey
        {
            int PosAccess = -1;
            int UV0Access = -1;
            int UV1Access = -1;
            int NormAccess = -1;
            int TanAccess = -1;
            // int JointAccess = -1;
            // int WeightAccess = -1;

            bool operator==(const ConvertedBufferViewKey &Rhs) const
            {
                return PosAccess == Rhs.PosAccess &&
                       UV0Access == Rhs.UV0Access &&
                       UV1Access == Rhs.UV1Access &&
                       NormAccess == Rhs.NormAccess &&
                       TanAccess == Rhs.TanAccess;
                // JointAccess == Rhs.JointAccess &&
                // WeightAccess == Rhs.WeightAccess;
            }

            struct Hasher
            {
                // TODO: add hash function here
                size_t operator()(const ConvertedBufferViewKey &Key) const
                {
                    return 0;
                }
            };
        };

        struct ConvertedBufferViewData
        {
            size_t VertexBasicDataOffset = ~size_t(0);
            size_t VertexSkinDataOffset = ~size_t(0);

            bool IsInitialized() const { return VertexBasicDataOffset != ~size_t(0); }
        };

        using ConvertedBufferViewMap = std::unordered_map<ConvertedBufferViewKey, ConvertedBufferViewData, ConvertedBufferViewKey::Hasher>;

    public:
        void ConvertBuffers(const ConvertedBufferViewKey &Key,
                            ConvertedBufferViewData &Data,
                            const tinygltf::Model &gltf_model,
                            std::vector<VertexBasicAttribs> &VertexBasicData) const
        {
            const float *bufferPos = nullptr;
            const float *bufferNormals = nullptr;
            const float *bufferTangents = nullptr;
            const float *bufferTexCoordSet0 = nullptr;
            const float *bufferTexCoordSet1 = nullptr;

            uint32_t vertexCount = 0;
            int posStride = -1;
            int normalsStride = -1;
            int tangentsStride = -1;
            int texCoordSet0Stride = -1;
            int texCoordSet1Stride = -1;

            if (Key.PosAccess >= 0)
            {
                const tinygltf::Accessor &posAccessor = gltf_model.accessors[Key.PosAccess];
                const tinygltf::BufferView &posView = gltf_model.bufferViews[posAccessor.bufferView];
                if (posAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
                    printf("Position component type is expected to be float");
                if (posAccessor.type != TINYGLTF_TYPE_VEC3)
                    printf("Position type is expected to be vec3");
                // VERIFY(posAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "Position component type is expected to be float");
                // VERIFY(posAccessor.type == TINYGLTF_TYPE_VEC3, "Position type is expected to be vec3");

                bufferPos = reinterpret_cast<const float *>(&(gltf_model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
                vertexCount = static_cast<uint32_t>(posAccessor.count);

                posStride = posAccessor.ByteStride(posView) / tinygltf::GetComponentSizeInBytes(posAccessor.componentType);
                if (posStride <= 0)
                    printf("Position stride is invalid");
                // VERIFY(posStride > 0, "Position stride is invalid");
            }

            if (Key.NormAccess >= 0)
            {
                const tinygltf::Accessor &normAccessor = gltf_model.accessors[Key.NormAccess];
                const tinygltf::BufferView &normView = gltf_model.bufferViews[normAccessor.bufferView];
                // VERIFY(normAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "Normal component type is expected to be float");
                // VERIFY(normAccessor.type == TINYGLTF_TYPE_VEC3, "Normal type is expected to be vec3");

                bufferNormals = reinterpret_cast<const float *>(&(gltf_model.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]));
                normalsStride = normAccessor.ByteStride(normView) / tinygltf::GetComponentSizeInBytes(normAccessor.componentType);
                // VERIFY(normalsStride > 0, "Normal stride is invalid");
            }

            if (Key.TanAccess >= 0)
            {
                const tinygltf::Accessor &tanAccessor = gltf_model.accessors[Key.TanAccess];
                const tinygltf::BufferView &tanView = gltf_model.bufferViews[tanAccessor.bufferView];
                // VERIFY(normAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "Normal component type is expected to be float");
                // VERIFY(normAccessor.type == TINYGLTF_TYPE_VEC3, "Normal type is expected to be vec3");

                bufferTangents = reinterpret_cast<const float *>(&(gltf_model.buffers[tanView.buffer].data[tanAccessor.byteOffset + tanView.byteOffset]));
                tangentsStride = tanAccessor.ByteStride(tanView) / tinygltf::GetComponentSizeInBytes(tanAccessor.componentType);
                // VERIFY(normalsStride > 0, "Normal stride is invalid");
            }

            if (Key.UV0Access >= 0)
            {
                const tinygltf::Accessor &uvAccessor = gltf_model.accessors[Key.UV0Access];
                const tinygltf::BufferView &uvView = gltf_model.bufferViews[uvAccessor.bufferView];
                // VERIFY(uvAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "UV0 component type is expected to be float");
                // VERIFY(uvAccessor.type == TINYGLTF_TYPE_VEC2, "UV0 type is expected to be vec2");

                bufferTexCoordSet0 = reinterpret_cast<const float *>(&(gltf_model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
                texCoordSet0Stride = uvAccessor.ByteStride(uvView) / tinygltf::GetComponentSizeInBytes(uvAccessor.componentType);
                // VERIFY(texCoordSet0Stride > 0, "Texcoord0 stride is invalid");
            }

            if (Key.UV1Access >= 0)
            {
                const tinygltf::Accessor &uvAccessor = gltf_model.accessors[Key.UV1Access];
                const tinygltf::BufferView &uvView = gltf_model.bufferViews[uvAccessor.bufferView];
                // VERIFY(uvAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT, "UV1 component type is expected to be float");
                // VERIFY(uvAccessor.type == TINYGLTF_TYPE_VEC2, "UV1 type is expected to be vec2");

                bufferTexCoordSet1 = reinterpret_cast<const float *>(&(gltf_model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
                texCoordSet1Stride = uvAccessor.ByteStride(uvView) / tinygltf::GetComponentSizeInBytes(uvAccessor.componentType);
                // VERIFY(texCoordSet1Stride > 0, "Texcoord1 stride is invalid");
            }
            Data.VertexBasicDataOffset = VertexBasicData.size();

            for (size_t v = 0; v < vertexCount; v++)
            {
                VertexBasicAttribs BasicAttribs{};

                BasicAttribs.pos = Vector3f{bufferPos + v * posStride};
                BasicAttribs.normal = bufferNormals != nullptr ? normalize(Vector3f{bufferNormals + v * normalsStride}) : Vector3f{};
                // BasicAttribs.normal = bufferNormals != nullptr ? Vector3f{ bufferNormals + v * normalsStride } : Vector3f{};
                // ?
                BasicAttribs.tangent = bufferTangents != nullptr ? normalize(Vector3f{bufferTangents + v * tangentsStride}) : Vector3f{};
                BasicAttribs.uv0 = bufferTexCoordSet0 != nullptr ? Vector2f{bufferTexCoordSet0 + v * texCoordSet0Stride} : Vector2f{};

                VertexBasicData.push_back(BasicAttribs);
            }
        }

        void LoadNode(SceneNode *parent,
                      const tinygltf::Node &gltf_node,
                      uint32_t nodeIndex,
                      const tinygltf::Model &gltf_model,
                      std::vector<uint32_t> &IndexData,
                      std::vector<VertexBasicAttribs> &VertexBasicData,
                      ConvertedBufferViewMap &ConvertedBuffers,
                      std::unique_ptr<Scene> &pScene)
        {
            std::unique_ptr<SceneNode> NewNode{new SceneNode{}};
            NewNode->Index = nodeIndex;
            NewNode->Parent = parent;
            NewNode->Name = gltf_node.name;
            NewNode->Matrix = BuildIdentityMatrix();

            std::unique_ptr<Model> &m_pModel = pScene->pModel;
            // std::unique_ptr<Model> model = std::make_unique<Model>( Model::CreateInfo{filename} );
            // std::unique_ptr<Model>& m_pModel = model;
            auto &m_Cameras = pScene->Cameras;
            auto &m_Geometries = pScene->Geometries;

            // Any node can define a local space transformation either by supplying a matrix property,
            // or any of translation, rotation, and scale properties (also known as TRS properties).

            // Generate local node matrix
            // float3 Translation;
            if (gltf_node.translation.size() == 3)
            {
                NewNode->Translation = Vector3f{
                    static_cast<float>(gltf_node.translation[0]),
                    static_cast<float>(gltf_node.translation[1]),
                    static_cast<float>(gltf_node.translation[2])};
            }

            if (gltf_node.rotation.size() == 4)
            {
                NewNode->Rotation = Vector4f{
                    static_cast<float>(gltf_node.rotation[0]),
                    static_cast<float>(gltf_node.rotation[1]),
                    static_cast<float>(gltf_node.rotation[2]),
                    static_cast<float>(gltf_node.rotation[3])};
            }

            if (gltf_node.scale.size() == 3)
            {
                NewNode->Scale = Vector3f{
                    static_cast<float>(gltf_node.scale[0]),
                    static_cast<float>(gltf_node.scale[1]),
                    static_cast<float>(gltf_node.scale[2])};
            }

            if (gltf_node.matrix.size() == 16)
            {
                std::vector<double> vals = gltf_node.matrix;
                NewNode->Matrix = Matrix4X4f //
                    {
                        static_cast<float>(vals[0]), static_cast<float>(vals[1]), static_cast<float>(vals[2]), static_cast<float>(vals[3]),
                        static_cast<float>(vals[4]), static_cast<float>(vals[5]), static_cast<float>(vals[6]), static_cast<float>(vals[7]),
                        static_cast<float>(vals[8]), static_cast<float>(vals[9]), static_cast<float>(vals[10]), static_cast<float>(vals[11]) //
                    };
            }

            // Node with children
            if (gltf_node.children.size() > 0)
            {
                for (size_t i = 0; i < gltf_node.children.size(); i++)
                {
                    LoadNode(NewNode.get(), gltf_model.nodes[gltf_node.children[i]], gltf_node.children[i],
                             gltf_model, IndexData, VertexBasicData, ConvertedBuffers, pScene);
                }
            }

            if (gltf_node.mesh >= 0)
            {
                const tinygltf::Mesh &gltf_mesh = gltf_model.meshes[gltf_node.mesh];
                std::shared_ptr<SceneObjectMesh> pNewMesh{new SceneObjectMesh{NewNode->Matrix}};

                for (size_t j = 0; j < gltf_mesh.primitives.size(); j++)
                {
                    const tinygltf::Primitive &primitive = gltf_mesh.primitives[j];

                    uint32_t indexStart = static_cast<uint32_t>(IndexData.size());
                    uint32_t vertexStart = 0;

                    uint32_t indexCount = 0;
                    uint32_t vertexCount = 0;
                    Vector3f PosMin;
                    Vector3f PosMax;
                    bool hasIndices = primitive.indices > -1;

                    // vertices
                    {
                        ConvertedBufferViewKey Key;

                        {
                            auto position_it = primitive.attributes.find("POSITION");
                            // TODO: add assert function here: "Position attribute is required"

                            const tinygltf::Accessor &posAccessor = gltf_model.accessors[position_it->second];

                            Key.PosAccess = position_it->second;

                            PosMin =
                                Vector3f //
                                {
                                    static_cast<float>(posAccessor.minValues[0]),
                                    static_cast<float>(posAccessor.minValues[1]),
                                    static_cast<float>(posAccessor.minValues[2]) //
                                };
                            PosMax =
                                Vector3f //
                                {
                                    static_cast<float>(posAccessor.maxValues[0]),
                                    static_cast<float>(posAccessor.maxValues[1]),
                                    static_cast<float>(posAccessor.maxValues[2]) //
                                };
                            vertexCount = static_cast<uint32_t>(posAccessor.count);
                        }

                        if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
                        {
                            Key.NormAccess = primitive.attributes.find("NORMAL")->second;
                        }

                        if (primitive.attributes.find("TANGENT") != primitive.attributes.end())
                        {
                            Key.TanAccess = primitive.attributes.find("TANGENT")->second;
                        }

                        if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
                        {
                            Key.UV0Access = primitive.attributes.find("TEXCOORD_0")->second;
                        }
                        
                        if (primitive.attributes.find("TEXCOORD_1") != primitive.attributes.end())
                        {
                            Key.UV1Access = primitive.attributes.find("TEXCOORD_1")->second;
                        }

                        auto &Data = ConvertedBuffers[Key];
                        if (!Data.IsInitialized())
                        {
                            ConvertBuffers(Key, Data, gltf_model, VertexBasicData);
                        }

                        // TODO: Skinning

                        // Indices
                        if (hasIndices)
                        {
                            const tinygltf::Accessor &accessor = gltf_model.accessors[primitive.indices > -1 ? primitive.indices : 0];
                            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
                            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];

                            indexCount = static_cast<uint32_t>(accessor.count);

                            const void *dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

                            IndexData.reserve(IndexData.size() + accessor.count);
                            switch (accessor.componentType)
                            {
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                            {
                                const uint32_t *buf = static_cast<const uint32_t *>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index] + vertexStart);
                                }
                                break;
                            }
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                            {
                                const uint16_t *buf = static_cast<const uint16_t *>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index] + vertexStart);
                                }
                                break;
                            }
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                            {
                                const uint8_t *buf = static_cast<const uint8_t *>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index] + vertexStart);
                                }
                                break;
                            }
                            default:
                                std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
                                return;
                            }
                        }

                        // TODO: add function to get index and vertex data in constructor of SceneObjectPrimitive
                        pNewMesh->Primitives.emplace_back( //
                            std::make_shared<SceneObjectPrimitive>(indexStart,
                                                                   indexCount,
                                                                   vertexCount,
                                                                   std::move(VertexBasicData),
                                                                   std::move(IndexData))
                            // primitive.material >= 0 ? static_cast<uint32_t>(primitive.material) : static_cast<uint32_t>(Materials.size() - 1),
                            // PosMin,
                            // PosMax
                            //
                        );
                    }
                }

                m_Geometries[NewNode->Name] = pNewMesh;
                NewNode->pMesh = std::move(pNewMesh);
            }

            // Node contains camera
            if (gltf_node.camera >= 0)
            {
                const auto &gltf_cam = gltf_model.cameras[gltf_node.camera];

                std::shared_ptr<SceneObjectCamera> pNewCamera;

                if (gltf_cam.type == "perspective")
                {
                    // pNewCamera->Type                    = Camera::Projection::Perspective;
                    // pNewCamera->Perspective.AspectRatio = static_cast<float>(gltf_cam.perspective.aspectRatio);
                    // pNewCamera->Perspective.YFov        = static_cast<float>(gltf_cam.perspective.yfov);
                    // pNewCamera->Perspective.ZNear       = static_cast<float>(gltf_cam.perspective.znear);
                    // pNewCamera->Perspective.ZFar        = static_cast<float>(gltf_cam.perspective.zfar);
                    pNewCamera = std::make_unique<SceneObjectPerspectiveCamera>(
                        static_cast<float>(gltf_cam.perspective.aspectRatio),
                        static_cast<float>(gltf_cam.perspective.yfov),
                        static_cast<float>(gltf_cam.perspective.znear),
                        static_cast<float>(gltf_cam.perspective.zfar));
                    pNewCamera->Name = gltf_cam.name;
                }
                else if (gltf_cam.type == "orthographic")
                {
                    // pNewCamera->Type               = Camera::Projection::Orthographic;
                    // pNewCamera->Orthographic.XMag  = static_cast<float>(gltf_cam.orthographic.xmag);
                    // pNewCamera->Orthographic.YMag  = static_cast<float>(gltf_cam.orthographic.ymag);
                    // pNewCamera->Orthographic.ZNear = static_cast<float>(gltf_cam.orthographic.znear);
                    // pNewCamera->Orthographic.ZFar  = static_cast<float>(gltf_cam.orthographic.zfar);
                    pNewCamera = std::make_unique<SceneObjectPerspectiveCamera>(
                        static_cast<float>(gltf_cam.orthographic.xmag),
                        static_cast<float>(gltf_cam.orthographic.ymag),
                        static_cast<float>(gltf_cam.orthographic.znear),
                        static_cast<float>(gltf_cam.orthographic.zfar));
                    pNewCamera->Name = gltf_cam.name;
                }
                else
                {
                    // TODO: Add an assert here
                    // UNEXPECTED("Unexpected camera type: ", gltf_cam.type);
                    printf("Unexpected camera type");
                    pNewCamera.reset();
                }

                if (pNewCamera)
                {
                    m_Cameras[NewNode->Name] = pNewCamera;
                    NewNode->pCamera = std::move(pNewCamera);
                }
            }

            m_pModel->LinearNodes.push_back(NewNode.get());
            if (parent)
            {
                parent->Children.push_back(std::move(NewNode));
            }
            else
            {
                m_pModel->Nodes.push_back(std::move(NewNode));
            }
        }

        void ParseImage(std::string& imagePath, std::shared_ptr<Image>& pImage)
        {
            
// 			// we should lookup if the texture has been loaded already to prevent
//             // duplicated load. This could be done in Asset Loader Manager.
// 			Buffer buf = g_pAssetLoader->SyncOpenAndReadBinary(imagePath.c_str());
// 			std::string ext = imagePath.substr(imagePath.find_last_of("."));
// 			if (ext == ".jpg" || ext == ".jpeg")
// 			{
// 				JpegParser jpeg_parser;
// 				pImage = std::make_shared<Image>(jpeg_parser.Parse(buf));
// 			}
// 			else if (ext == ".png")
// 			{
// 				PngParser png_parser;
// 				pImage = std::make_shared<Image>(png_parser.Parse(buf));
// 			}
// 			// else if (ext == ".bmp")
// 			// {
// 			//     BmpParser bmp_parser;
// 			//     pImage = std::make_shared<Image>(bmp_parser.Parse(buf));
// 			// }
// 			// else if (ext == ".tga")
// 			// {
// 			//     TgaParser tga_parser;
// 			//     pImage = std::make_shared<Image>(tga_parser.Parse(buf));
// 			// }
// 			else if (ext == ".bmp")
// 			{
// 				BmpParser bmp_parser;
// 				pImage = std::make_shared<Image>(bmp_parser.Parse(buf));
// 			}
        }

        void LoadMaterialsAndTextures(const tinygltf::Model &gltf_model, std::unique_ptr<Scene> &pScene, std::string& BaseDir)
        {
            auto &m_Materials = pScene->Materials;
            for (const tinygltf::Material &gltf_mat : gltf_model.materials)
            {
                SceneObjectMaterial Mat;

                struct TextureParameterInfo
                {
                    const SceneObjectMaterial::TEXTURE_ID TextureId;
                    float &UVSelector;
                    Vector4f &UVScaleBias;
                    float &Slice;
                    const char *const TextureName;
                    const tinygltf::ParameterMap &Params;
                };
                // clang-format off
                std::array<TextureParameterInfo, 5> TextureParams =
                {
                    TextureParameterInfo{SceneObjectMaterial::TEXTURE_ID_BASE_COLOR,    Mat.Attribs.BaseColorUVSelector,          Mat.Attribs.BaseColorUVScaleBias,          Mat.Attribs.BaseColorSlice,          "baseColorTexture",         gltf_mat.values},
                    TextureParameterInfo{SceneObjectMaterial::TEXTURE_ID_PHYSICAL_DESC, Mat.Attribs.PhysicalDescriptorUVSelector, Mat.Attribs.PhysicalDescriptorUVScaleBias, Mat.Attribs.PhysicalDescriptorSlice, "metallicRoughnessTexture", gltf_mat.values},
                    TextureParameterInfo{SceneObjectMaterial::TEXTURE_ID_NORMAL_MAP,    Mat.Attribs.NormalUVSelector,             Mat.Attribs.NormalUVScaleBias,             Mat.Attribs.NormalSlice,             "normalTexture",            gltf_mat.additionalValues},
                    TextureParameterInfo{SceneObjectMaterial::TEXTURE_ID_OCCLUSION,     Mat.Attribs.OcclusionUVSelector,          Mat.Attribs.OcclusionUVScaleBias,          Mat.Attribs.OcclusionSlice,          "occlusionTexture",         gltf_mat.additionalValues},
                    TextureParameterInfo{SceneObjectMaterial::TEXTURE_ID_EMISSIVE,      Mat.Attribs.EmissiveUVSelector,           Mat.Attribs.EmissiveUVScaleBias,           Mat.Attribs.EmissiveSlice,           "emissiveTexture",          gltf_mat.additionalValues}
                };
                // clang-format on

                for (const auto &Param : TextureParams)
                {
                    auto tex_it = Param.Params.find(Param.TextureName);
                    if (tex_it != Param.Params.end())
                    {
                        Mat.TextureIds[Param.TextureId] = tex_it->second.TextureIndex();
                        Param.UVSelector = static_cast<float>(tex_it->second.TextureTexCoord());
                    }
                }

                auto ReadFactor = [](float &Factor, const tinygltf::ParameterMap &Params, const char *Name) //
                {
                    auto it = Params.find(Name);
                    if (it != Params.end())
                    {
                        Factor = static_cast<float>(it->second.Factor());
                    }
                };
                ReadFactor(Mat.Attribs.RoughnessFactor, gltf_mat.values, "roughnessFactor");
                ReadFactor(Mat.Attribs.MetallicFactor, gltf_mat.values, "metallicFactor");

                auto ReadColorFactor = [](Vector4f &Factor, const tinygltf::ParameterMap &Params, const char *Name) //
                {
                    auto it = Params.find(Name);
                    if (it != Params.end())
                    {
                        Factor = Vector4f::MakeVector(it->second.ColorFactor().data());
                    }
                };

                ReadColorFactor(Mat.Attribs.BaseColorFactor, gltf_mat.values, "baseColorFactor");
                ReadColorFactor(Mat.Attribs.EmissiveFactor, gltf_mat.additionalValues, "emissiveFactor");

                {
                    auto alpha_mode_it = gltf_mat.additionalValues.find("alphaMode");
                    if (alpha_mode_it != gltf_mat.additionalValues.end())
                    {
                        const tinygltf::Parameter &param = alpha_mode_it->second;
                        if (param.string_value == "BLEND")
                        {
                            Mat.Attribs.AlphaMode = SceneObjectMaterial::ALPHA_MODE_BLEND;
                        }
                        if (param.string_value == "MASK")
                        {
                            Mat.Attribs.AlphaMode = SceneObjectMaterial::ALPHA_MODE_MASK;
                            Mat.Attribs.AlphaCutoff = 0.5f;
                        }
                    }
                }

                ReadFactor(Mat.Attribs.AlphaCutoff, gltf_mat.additionalValues, "alphaCutoff");

                {
                    auto double_sided_it = gltf_mat.additionalValues.find("doubleSided");
                    if (double_sided_it != gltf_mat.additionalValues.end())
                    {
                        Mat.DoubleSided = double_sided_it->second.bool_value;
                    }
                }

                Mat.Attribs.Workflow = SceneObjectMaterial::PBR_WORKFLOW_METALL_ROUGH;

                // Extensions
                // @TODO: Find out if there is a nicer way of reading these properties with recent tinygltf headers
                {
                    auto ext_it = gltf_mat.extensions.find("KHR_materials_pbrSpecularGlossiness");
                    if (ext_it != gltf_mat.extensions.end())
                    {
                        if (ext_it->second.Has("specularGlossinessTexture"))
                        {
                            auto& index = ext_it->second.Get("specularGlossinessTexture").Get("index");
                            auto& texCoordSet = ext_it->second.Get("specularGlossinessTexture").Get("texCoord");

                            Mat.TextureIds[SceneObjectMaterial::TEXTURE_ID_PHYSICAL_DESC] = index.Get<int>();
                            Mat.Attribs.PhysicalDescriptorUVSelector = static_cast<float>(texCoordSet.Get<int>());

                            Mat.Attribs.Workflow = SceneObjectMaterial::PBR_WORKFLOW_SPEC_GLOSS;
                        }

                        if (ext_it->second.Has("diffuseTexture"))
                        {
                            auto& index = ext_it->second.Get("diffuseTexture").Get("index");
                            auto& texCoordSet = ext_it->second.Get("diffuseTexture").Get("texCoord");

                            Mat.TextureIds[SceneObjectMaterial::TEXTURE_ID_BASE_COLOR] = index.Get<int>();
                            Mat.Attribs.BaseColorUVSelector = static_cast<float>(texCoordSet.Get<int>());
                        }

                        if (ext_it->second.Has("diffuseFactor"))
                        {
                            auto& factor = ext_it->second.Get("diffuseFactor");
                            for (uint32_t i = 0; i < factor.ArrayLen(); i++)
                            {
                                const auto& val = factor.Get(i);
                                Mat.Attribs.BaseColorFactor[i] =
                                    val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
                            }
                        }

                        if (ext_it->second.Has("specularFactor"))
                        {
                            auto& factor = ext_it->second.Get("specularFactor");
                            for (uint32_t i = 0; i < factor.ArrayLen(); i++)
                            {
                                const auto& val = factor.Get(i);
                                Mat.Attribs.SpecularFactor[i] =
                                    val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
                            }
                        }
                    }
                }

                 for (const auto &Param : TextureParams)
                 {
                     auto TexIndex = Mat.TextureIds[Param.TextureId];
                     if (TexIndex >= 0)
                     {
						 // const auto &TexInfo = Textures[TexIndex];
						 // if (TexInfo.pAtlasSuballocation)
						 // {
						 //     Param.UVScaleBias = TexInfo.pAtlasSuballocation->GetUVScaleBias();
						 //     Param.Slice = static_cast<float>(TexInfo.pAtlasSuballocation->GetSlice());
						 // }
                     }
                 }

				 for (const tinygltf::Texture& gltf_tex : gltf_model.textures)
				 {
					 const tinygltf::Image& gltf_image = gltf_model.images[gltf_tex.source];
					 auto& ImageId = !gltf_image.uri.empty() ? (BaseDir + gltf_image.uri) : "";

                     
					 if (!ImageId.empty())
					 {
						 std::shared_ptr<Image> m_pImage{};
						 ParseImage(ImageId, m_pImage); // TODO
                         auto texture = std::make_shared<SceneObjectTexture>(m_pImage);
                         texture->SetName(gltf_image.uri);
                         Mat.Textures[gltf_tex.source] = texture;
					 }
				 }
                
                 m_Materials[gltf_mat.name] = std::make_shared<SceneObjectMaterial>(Mat);
            }

            // Push a default material at the end of the list for meshes with no material assigned
            if (m_Materials.empty())
            {
                m_Materials["default"] = std::make_shared<SceneObjectMaterial>();
            }
        }

        virtual std::unique_ptr<Scene> Parse(const std::string& FilePath) final
        {
            if (pScene->name == "")
                assert("File path must not be empty");

            std::string filename{pScene->name};

            bool binary = false;
            size_t extpos = filename.rfind('.', filename.length());
            if (extpos != std::string::npos)
            {
                binary = (filename.substr(extpos + 1, filename.length() - extpos) == "glb");
            }

            std::string error;
            std::string warning;
            tinygltf::Model gltf_model;

            tinygltf::TinyGLTF loader;
            bool fileLoaded = false;
            if (binary)
                fileLoaded = loader.LoadBinaryFromFile(&gltf_model, &error, &warning, filename);
            else
                fileLoaded = loader.LoadASCIIFromFile(&gltf_model, &error, &warning, filename);

            if (!fileLoaded)
            {
                printf("Failed to load gltf file");
            }
            if (!warning.empty())
            {
                printf("Loaded gltf file");
            }
            if (!error.empty())
            {
                printf("Loaded gltf file");
            }

            std::string baseDir;
			extpos = filename.rfind('/', filename.length());
			if (extpos != std::string::npos)
			{
                baseDir = filename.substr(0, extpos + 1);
			}

            // LoadTextureSamplers(pDevice, gltf_model);
            LoadMaterialsAndTextures(gltf_model, pScene, baseDir);

            std::vector<uint32_t> IndexData;
            std::vector<VertexBasicAttribs> VertexBasicData;
            ConvertedBufferViewMap ConvertedBuffers;

            pScene->pModel = std::make_unique<Model>();
            std::unique_ptr<Model> &m_pModel = pScene->pModel;
            // std::unique_ptr<Model> model = std::make_unique<Model>( Model::CreateInfo{filename} );
            // std::unique_ptr<Model>& m_pModel = model;
            // auto& m_Cameras = pScene->Cameras;
            // auto& m_Geometries = pScene->Geometries;

            // TODO: scene handling with no default scene
            const tinygltf::Scene &scene = gltf_model.scenes[gltf_model.defaultScene > -1 ? gltf_model.defaultScene : 0];
            for (size_t i = 0; i < scene.nodes.size(); i++)
            {
                const tinygltf::Node node = gltf_model.nodes[scene.nodes[i]];
                LoadNode(nullptr, node, scene.nodes[i], gltf_model,
                         IndexData, VertexBasicData, ConvertedBuffers, pScene);
            }

            // for (auto* node : LinearNodes)
            // {
            //     // Assign skins
            //     if (node->SkinIndex >= 0)
            //     {
            //         node->pSkin = Skins[node->SkinIndex].get();
            //     }
            // }

            // Initial pose
            for (auto &root_node : m_pModel->Nodes)
            {
                root_node->UpdateTransforms();
            }
            // TODO: use this function to get boundbox and BVH
            // CalculateSceneDimensions();

            // UpdatePrimitiveData();
        }
    };
}
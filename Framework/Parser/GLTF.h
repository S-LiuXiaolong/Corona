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
                            std::vector<VertexBasicAttribs> &VertexData) const
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
                // VERIFY(normalsStride > 0, "tangent stride is invalid");
            }
            else
            {
                // TODO: Calculate tangent space (with mikktspace ?)
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
            Data.VertexBasicDataOffset = VertexData.size();

            for (size_t v = 0; v < vertexCount; v++)
            {
                VertexBasicAttribs BasicAttribs{};

                BasicAttribs.pos = Vector3f{bufferPos + v * posStride};
                BasicAttribs.normal = bufferNormals != nullptr ? normalize(Vector3f{bufferNormals + v * normalsStride}) : Vector3f{};
                // BasicAttribs.normal = bufferNormals != nullptr ? Vector3f{ bufferNormals + v * normalsStride } : Vector3f{};
                // ?
                BasicAttribs.tangent = bufferTangents != nullptr ? normalize(Vector3f{bufferTangents + v * tangentsStride}) : Vector3f{};
                BasicAttribs.uv0 = bufferTexCoordSet0 != nullptr ? Vector2f{bufferTexCoordSet0 + v * texCoordSet0Stride} : Vector2f{};

                VertexData.push_back(BasicAttribs);
            }
        }

        void LoadNode(SceneNode *parent,
                      const tinygltf::Node &gltf_node,
                      uint32_t nodeIndex,
                      const tinygltf::Model &gltf_model,
                      std::vector<VertexBasicAttribs> &VertexData,
                      std::vector<uint32_t> &IndexData,
                      ConvertedBufferViewMap &ConvertedBuffers,
                      std::shared_ptr<Scene> &pScene)
        {
            std::unique_ptr<SceneNode> pTempNode(new SceneNode());
            std::shared_ptr<SceneNode> pNewNode(new SceneNode());
            std::shared_ptr<SceneNode> pNewNodeForTest;
            pNewNode->Index = nodeIndex;
            pNewNode->m_Parent = parent;
            pNewNode->m_strName = gltf_node.name;
            pNewNode->Matrix = BuildIdentityMatrix();

            // std::unique_ptr<Model> model = std::make_unique<Model>( Model::CreateInfo{filePath} );
            // std::unique_ptr<Model>& m_pModel = model;
            auto &m_Cameras = pScene->Cameras;
            auto &m_Lights = pScene->Lights;
            auto &m_Geometries = pScene->Geometries;

            auto &m_CameraNodes = pScene->CameraNodes;
            auto &m_LightNodes = pScene->LightNodes;
            auto &m_GeometryNodes = pScene->GeometryNodes;

            // Any node can define a local space transformation either by supplying a matrix property,
            // or any of translation, rotation, and scale properties (also known as TRS properties).

            // Generate local node matrix
            // float3 Translation;
            if (gltf_node.translation.size() == 3)
            {
                pNewNode->Translation = Vector3f{
                    static_cast<float>(gltf_node.translation[0]),
                    static_cast<float>(gltf_node.translation[1]),
                    static_cast<float>(gltf_node.translation[2])};
            }

            if (gltf_node.rotation.size() == 4)
            {
                pNewNode->Rotation = Vector4f{
                    static_cast<float>(gltf_node.rotation[0]),
                    static_cast<float>(gltf_node.rotation[1]),
                    static_cast<float>(gltf_node.rotation[2]),
                    static_cast<float>(gltf_node.rotation[3])};
            }

            if (gltf_node.scale.size() == 3)
            {
                pNewNode->Scale = Vector3f{
                    static_cast<float>(gltf_node.scale[0]),
                    static_cast<float>(gltf_node.scale[1]),
                    static_cast<float>(gltf_node.scale[2])};
            }

            if (gltf_node.matrix.size() == 16)
            {
                std::vector<double> vals = gltf_node.matrix;
                pNewNode->Matrix = Matrix4X4f //
                    {
                        static_cast<float>(vals[0]), static_cast<float>(vals[1]), static_cast<float>(vals[2]), static_cast<float>(vals[3]),
                        static_cast<float>(vals[4]), static_cast<float>(vals[5]), static_cast<float>(vals[6]), static_cast<float>(vals[7]),
                        static_cast<float>(vals[8]), static_cast<float>(vals[9]), static_cast<float>(vals[10]), static_cast<float>(vals[11]) //
                    };
            }

            if (gltf_node.mesh >= 0)
            {
                const tinygltf::Mesh &gltf_mesh = gltf_model.meshes[gltf_node.mesh];
                std::shared_ptr<SceneObjectMesh> pNewMesh(new SceneObjectMesh);

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
                            ConvertBuffers(Key, Data, gltf_model, VertexData);
                        }

                        vertexStart = static_cast<uint32_t>(Data.VertexBasicDataOffset);

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
                            // IndexData.reserve(IndexData.GetIndexCount() + accessor.count);
                            switch (accessor.componentType)
                            {
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                            {
                                const uint32_t *buf = static_cast<const uint32_t *>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index]);
                                }
                                break;
                            }
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                            {
                                const uint16_t *buf = static_cast<const uint16_t *>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index]);
                                }
                                break;
                            }
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                            {
                                const uint8_t *buf = static_cast<const uint8_t *>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index]);
                                }
                                break;
                            }
                            default:
                                std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
                                return;
                            }
                        }
                        // TODO: Should i use a std::move here ? NO.
                        std::vector<VertexBasicAttribs> cutVertexData(VertexData.begin() + vertexStart, VertexData.begin() + vertexStart + vertexCount);
                        std::vector<uint32_t> cutIndexData(IndexData.begin() + indexStart, IndexData.begin() + indexStart + indexCount);
                        std::shared_ptr<SceneObjectPrimitive> pNewPrimitive(
                            new SceneObjectPrimitive(std::move(cutVertexData), std::move(cutIndexData)));
                        pNewMesh->AddPrimitive(pNewPrimitive);
                        pNewMesh->SetMaterial(primitive.material >= 0 ? static_cast<uint32_t>(primitive.material) : -1 );
                    }
                }
                pNewNode->pMesh = pNewMesh;
                m_Geometries[gltf_mesh.name] = std::move(pNewMesh);
                m_GeometryNodes[gltf_node.name] = pNewNode; // TODO: Attention
                // pNewNode->pMesh = std::move(pNewMesh);
            }

            // Node contains camera
            if (gltf_node.camera >= 0)
            {
                pNewNodeForTest = std::make_shared<SceneCameraNode>(*pNewNode);
                const auto &gltf_cam = gltf_model.cameras[gltf_node.camera];

                if (gltf_cam.type == "perspective")
                {
                    // pNewCamera->Type                    = Camera::Projection::Perspective;
                    // pNewCamera->Perspective.AspectRatio = static_cast<float>(gltf_cam.perspective.aspectRatio);
                    // pNewCamera->Perspective.YFov        = static_cast<float>(gltf_cam.perspective.yfov);
                    // pNewCamera->Perspective.ZNear       = static_cast<float>(gltf_cam.perspective.znear);
                    // pNewCamera->Perspective.ZFar        = static_cast<float>(gltf_cam.perspective.zfar);
                    auto pNewCamera = std::make_shared<SceneObjectPerspectiveCamera>(
                        gltf_cam.type,
                        static_cast<float>(gltf_cam.perspective.znear),
                        static_cast<float>(gltf_cam.perspective.zfar),
                        static_cast<float>(gltf_cam.perspective.aspectRatio),
                        static_cast<float>(gltf_cam.perspective.yfov));

                    // TODO: A cleaner way?
                    std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest)->pCamera = pNewCamera;
                    m_Cameras[gltf_cam.name] = std::move(pNewCamera);
                    // TODO: add ref of pNewCamera and pNewCamNode
                }
                else if (gltf_cam.type == "orthographic")
                {
                    // pNewCamera->Type               = Camera::Projection::Orthographic;
                    // pNewCamera->Orthographic.XMag  = static_cast<float>(gltf_cam.orthographic.xmag);
                    // pNewCamera->Orthographic.YMag  = static_cast<float>(gltf_cam.orthographic.ymag);
                    // pNewCamera->Orthographic.ZNear = static_cast<float>(gltf_cam.orthographic.znear);
                    // pNewCamera->Orthographic.ZFar  = static_cast<float>(gltf_cam.orthographic.zfar);
                    auto pNewCamera = std::make_shared<SceneObjectPerspectiveCamera>(
                        gltf_cam.type,
                        static_cast<float>(gltf_cam.orthographic.znear),
                        static_cast<float>(gltf_cam.orthographic.zfar),
                        static_cast<float>(gltf_cam.orthographic.xmag),
                        static_cast<float>(gltf_cam.orthographic.ymag));

                    std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest)->pCamera = pNewCamera;
                    m_Cameras[gltf_cam.name] = std::move(pNewCamera);
                    // TODO
                }
                else
                {
                    // TODO: Add an assert here
                    // UNEXPECTED("Unexpected camera type: ", gltf_cam.type);
                    printf("Unexpected camera type");
                }

                m_CameraNodes[gltf_node.name] = std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest); // TODO
            }

            // use dynamic_cast and dynamic_pointer_cast to get various Nodes (?)
            pScene->LUT_Name_LinearNodes[gltf_node.name] = pNewNode;

			// temporary for light
			if (gltf_node.extensions.find("KHR_lights_punctual") != gltf_node.extensions.end())
            {
                pScene->LUT_Name_LinearNodes[parent->GetName()]->m_type = "Light";
				m_LightNodes[parent->GetName()] = pScene->LUT_Name_LinearNodes[parent->GetName()];
				auto _it = gltf_node.extensions.find("KHR_lights_punctual");
                pNewNode->m_type = "Light_Orietation";
				pNewNode->lightIndex = ((*_it).second).Get("light").GetNumberAsInt();
                m_LightNodes[pNewNode->GetName()] = pNewNode;
			}

            if (parent)
            {
                parent->m_Children.push_back(std::move(pNewNode));
            }
            else
            {
                pScene->RootNodes.push_back(std::move(pNewNode));
            }

            if(m_CameraNodes.find("Default_Camera") == m_CameraNodes.end())
            {
                pNewNodeForTest = std::make_shared<SceneCameraNode>();

                auto pNewCamera = std::make_shared<SceneObjectPerspectiveCamera>(
                    "perspective",
                    static_cast<float>(0.1f),
                    static_cast<float>(100.0f),
                    static_cast<float>(16.0f / 9.0f),
                    static_cast<float>(PI / 2.0f));

                std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest)->pCamera = pNewCamera;
                m_Cameras["Default_Camera"] = std::move(pNewCamera);

                std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest)->m_type = "Camera";
                std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest)->Translation = Vector3f{ 0.0f, 0.0f, 3.0f };
                m_CameraNodes["Default_Camera"] = std::dynamic_pointer_cast<SceneCameraNode>(pNewNodeForTest);

                // use dynamic_cast and dynamic_pointer_cast to get various Nodes (?)
                // Just because there is no NEW METHOD for mesh
                pScene->LUT_Name_LinearNodes["Default_Camera"] = pNewNodeForTest;
                if (parent)
                {
                    parent->m_Children.push_back(std::move(pNewNodeForTest));
                }
                else
                {
                    pScene->RootNodes.push_back(std::move(pNewNodeForTest));
                }
            }

			// Node with children
			if (gltf_node.children.size() > 0)
			{
				for (size_t i = 0; i < gltf_node.children.size(); i++)
				{
					LoadNode(pNewNode.get(), gltf_model.nodes[gltf_node.children[i]], gltf_node.children[i],
						gltf_model, VertexData, IndexData, ConvertedBuffers, pScene);
				}
			}

        }

        void ParseImage(std::string &imagePath, std::shared_ptr<Image> &pImage)
        {
            // we should lookup if the texture has been loaded already to prevent
                // duplicated load. This could be done in Asset Loader Manager.
            Buffer buf = g_pAssetLoader->SyncOpenAndReadBinary(imagePath.c_str());
            std::string ext = imagePath.substr(imagePath.find_last_of("."));
            if (ext == ".jpg" || ext == ".jpeg")
            {
             	JpegParser jpeg_parser;
             	pImage = std::make_shared<Image>(jpeg_parser.Parse(buf));
            }
            else if (ext == ".png")
            {
             	PngParser png_parser;
             	pImage = std::make_shared<Image>(png_parser.Parse(buf));
            }
            // else if (ext == ".bmp")
            // {
            //     BmpParser bmp_parser;
            //     pImage = std::make_shared<Image>(bmp_parser.Parse(buf));
            // }
            // else if (ext == ".tga")
            // {
            //     TgaParser tga_parser;
            //     pImage = std::make_shared<Image>(tga_parser.Parse(buf));
            // }
            else if (ext == ".bmp")
            {
             	BmpParser bmp_parser;
             	pImage = std::make_shared<Image>(bmp_parser.Parse(buf));
            }
        }

        void LoadMaterialsAndTextures(const tinygltf::Model &gltf_model, std::shared_ptr<Scene> &pScene, std::string &BasePath)
        {
            std::vector<std::string> NameOfTextures;
            std::vector<std::shared_ptr<Image>> m_pImages;
			// TODO: put every map on its own position
			for (const tinygltf::Texture& gltf_tex : gltf_model.textures)
			{
				const tinygltf::Image& gltf_image = gltf_model.images[gltf_tex.source];
				auto& ImageId = !gltf_image.uri.empty() ? (BasePath + gltf_image.uri) : "";

				if (!ImageId.empty())
				{
					std::shared_ptr<Image> m_pImage(new Image());
					ParseImage(ImageId, m_pImage); // TODO

                    NameOfTextures.push_back(gltf_image.uri);
                    m_pImages.push_back(m_pImage);
				}
			}

            auto &m_Materials = pScene->Materials;
            for (const tinygltf::Material &gltf_mat : gltf_model.materials)
            {
                std::shared_ptr<SceneObjectMaterial> pMat(new SceneObjectMaterial(gltf_mat.name));

                struct TextureParameterInfo
                {
                    const TEXTURE_ID TextureId;
                    float &UVSelector;
                    Vector4f &UVScaleBias;
                    float &Slice;
                    const char *const TextureName;
                    const tinygltf::ParameterMap &Params;
                };

                ShaderAttribs &attribs = pMat->GetShaderAttribs();

                std::array<TextureParameterInfo, 5> TextureParams =
                    {
                        TextureParameterInfo{TEXTURE_ID::TEXTURE_ID_BASE_COLOR, attribs.BaseColorUVSelector, attribs.BaseColorUVScaleBias, attribs.BaseColorSlice, "baseColorTexture", gltf_mat.values},
                        TextureParameterInfo{TEXTURE_ID::TEXTURE_ID_PHYSICAL_DESC, attribs.PhysicalDescriptorUVSelector, attribs.PhysicalDescriptorUVScaleBias, attribs.PhysicalDescriptorSlice, "metallicRoughnessTexture", gltf_mat.values},
                        TextureParameterInfo{TEXTURE_ID::TEXTURE_ID_NORMAL_MAP, attribs.NormalUVSelector, attribs.NormalUVScaleBias, attribs.NormalSlice, "normalTexture", gltf_mat.additionalValues},
                        TextureParameterInfo{TEXTURE_ID::TEXTURE_ID_OCCLUSION, attribs.OcclusionUVSelector, attribs.OcclusionUVScaleBias, attribs.OcclusionSlice, "occlusionTexture", gltf_mat.additionalValues},
                        TextureParameterInfo{TEXTURE_ID::TEXTURE_ID_EMISSIVE, attribs.EmissiveUVSelector, attribs.EmissiveUVScaleBias, attribs.EmissiveSlice, "emissiveTexture", gltf_mat.additionalValues}};

                int textureCount = 0;

                for (const auto &Param : TextureParams)
                {
                    auto tex_it = Param.Params.find(Param.TextureName);
                    if (tex_it != Param.Params.end())
                    {
                        pMat->TextureIds[Param.TextureId] = tex_it->second.TextureIndex();
                        Param.UVSelector = static_cast<float>(tex_it->second.TextureTexCoord());
                        textureCount++;
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
                ReadFactor(attribs.RoughnessFactor, gltf_mat.values, "roughnessFactor");
                ReadFactor(attribs.MetallicFactor, gltf_mat.values, "metallicFactor");

                auto ReadColorFactor = [](Vector4f &Factor, const tinygltf::ParameterMap &Params, const char *Name) //
                {
                    auto it = Params.find(Name);
                    if (it != Params.end())
                    {
                        Factor = Vector4f::MakeVector(it->second.ColorFactor().data());
                    }
                };

                ReadColorFactor(attribs.BaseColorFactor, gltf_mat.values, "baseColorFactor");
                ReadColorFactor(attribs.EmissiveFactor, gltf_mat.additionalValues, "emissiveFactor");

                {
                    auto alpha_mode_it = gltf_mat.additionalValues.find("alphaMode");
                    if (alpha_mode_it != gltf_mat.additionalValues.end())
                    {
                        const tinygltf::Parameter &param = alpha_mode_it->second;
                        if (param.string_value == "BLEND")
                        {
                            attribs.AlphaMode = ALPHA_MODE::ALPHA_MODE_BLEND;
                        }
                        if (param.string_value == "MASK")
                        {
                            attribs.AlphaMode = ALPHA_MODE::ALPHA_MODE_MASK;
                            attribs.AlphaCutoff = 0.5f;
                        }
                    }
                }

                ReadFactor(attribs.AlphaCutoff, gltf_mat.additionalValues, "alphaCutoff");

                {
                    auto double_sided_it = gltf_mat.additionalValues.find("doubleSided");
                    if (double_sided_it != gltf_mat.additionalValues.end())
                    {
                        pMat->SetDoubleSided(double_sided_it->second.bool_value);
                    }
                }

                attribs.Workflow = PBR_WORKFLOW::PBR_WORKFLOW_METALL_ROUGH;

                // Extensions
                // @TODO: Find out if there is a nicer way of reading these properties with recent tinygltf headers
                {
                    auto ext_it = gltf_mat.extensions.find("KHR_materials_pbrSpecularGlossiness");
                    if (ext_it != gltf_mat.extensions.end())
                    {
                        if (ext_it->second.Has("specularGlossinessTexture"))
                        {
                            auto &index = ext_it->second.Get("specularGlossinessTexture").Get("index");
                            auto &texCoordSet = ext_it->second.Get("specularGlossinessTexture").Get("texCoord");

                            pMat->TextureIds[TEXTURE_ID::TEXTURE_ID_PHYSICAL_DESC] = index.Get<int>();
                            attribs.PhysicalDescriptorUVSelector = static_cast<float>(texCoordSet.Get<int>());

                            attribs.Workflow = PBR_WORKFLOW::PBR_WORKFLOW_SPEC_GLOSS;
                        }

                        if (ext_it->second.Has("diffuseTexture"))
                        {
                            auto &index = ext_it->second.Get("diffuseTexture").Get("index");
                            auto &texCoordSet = ext_it->second.Get("diffuseTexture").Get("texCoord");

                            pMat->TextureIds[TEXTURE_ID::TEXTURE_ID_BASE_COLOR] = index.Get<int>();
                            attribs.BaseColorUVSelector = static_cast<float>(texCoordSet.Get<int>());
                        }

                        if (ext_it->second.Has("diffuseFactor"))
                        {
                            auto &factor = ext_it->second.Get("diffuseFactor");
                            for (uint32_t i = 0; i < factor.ArrayLen(); i++)
                            {
                                const auto &val = factor.Get(i);
                                attribs.BaseColorFactor[i] =
                                    val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
                            }
                        }

                        if (ext_it->second.Has("specularFactor"))
                        {
                            auto &factor = ext_it->second.Get("specularFactor");
                            for (uint32_t i = 0; i < factor.ArrayLen(); i++)
                            {
                                const auto &val = factor.Get(i);
                                attribs.SpecularFactor[i] =
                                    val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
                            }
                        }
                    }
                }

                for (const auto &Param : TextureParams)
                {
                    auto TexIndex = pMat->TextureIds[Param.TextureId];
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

//                 // TODO: put every map on its own position
//                 for (const tinygltf::Texture &gltf_tex : gltf_model.textures)
//                 {
//                     const tinygltf::Image &gltf_image = gltf_model.images[gltf_tex.source];
//                     auto &ImageId = !gltf_image.uri.empty() ? (BasePath + gltf_image.uri) : "";
// 
//                     if (!ImageId.empty())
//                     {
//                         std::shared_ptr<Image> m_pImage(new Image());
//                         ParseImage(ImageId, m_pImage); // TODO
//                         // auto texture = std::make_shared<SceneObjectTexture>(m_pImage);
//                         std::shared_ptr<SceneObjectTexture> texture(new SceneObjectTexture(m_pImage));
//                         texture->SetName(gltf_image.uri);
//                         // TODO
//                         // pMat.Textures[TextureParams[pMat.TextureIds[gltf_tex.source]].TextureName] = texture;
//                         // pMat->Textures[pMat->TextureIds[gltf_tex.source]] = texture;
//                         for (int i = 0; i < pMat->TextureIds.size(); i++)
//                         {
//                             if (gltf_tex.source == pMat->TextureIds[i])
//                             {
//                                 pMat->Textures[i] = texture;
//                             }
//                         }
//                     }
//                 }
                for (int i = 0; i < pMat->TextureIds.size(); i++)
                {
                    if (pMat->TextureIds[i] != -1)
                    {
						auto texture = std::make_shared<SceneObjectTexture>(m_pImages[pMat->TextureIds[i]]);
						texture->SetName(NameOfTextures[pMat->TextureIds[i]]);
						pMat->Textures[i] = texture;

                        if (i == 0)
                        {
                            pMat->ColorMap = texture;
                        }
						if (i == 1)
						{
							pMat->PhysicsDescriptorMap = texture;
						}
						if (i == 2)
						{
							pMat->NormalMap = texture;
						}
						if (i == 3)
						{
							pMat->AOMap = texture;
						}
						if (i == 4)
						{
							pMat->EmissiveMap = texture;
						}
                    }
                }

                pScene->LinearMaterials.emplace_back(pMat);
                m_Materials[gltf_mat.name] = pMat;
            }

            // TODO: we need to make this default material more clear
            // as we can add default material (if there is no) to m_pDefaultMaterial
//             // Push a default material at the end of the list for meshes with no material assigned
//             if (m_Materials.empty())
//             {
//                 m_Materials["default"] = std::make_shared<SceneObjectMaterial>();
//             }
        }

        void LoadLights(const tinygltf::Model& gltf_model, std::shared_ptr<Scene>& pScene)
        {
			auto& m_Lights = pScene->Lights;
            auto& m_LinearLights = pScene->LinearLights;
			auto& m_LightNodes = pScene->LightNodes;

            for (auto light : gltf_model.lights)
            {
                if (light.type == "point")
                {
                    std::shared_ptr<SceneObjectOmniLight> m_Light(new SceneObjectOmniLight());
					Vector4f color = Vector4f(Vector3f(light.color), 1.0f);
					m_Light->SetColor("light", color);
                    m_Light->SetParam("intensity", light.intensity);
                    m_Light->m_type = "point";

                    m_Lights[light.name] = m_Light;
                    m_LinearLights.emplace_back(m_Light);
                }
                else if (light.type == "spot")
                {
					std::shared_ptr<SceneObjectSpotLight> m_Light(new SceneObjectSpotLight());
					Vector4f color = Vector4f(Vector3f(light.color), 1.0f);
					m_Light->SetColor("light", color);
					m_Light->SetParam("intensity", light.intensity);
                    m_Light->SetInnerConeAngle(light.spot.innerConeAngle);
                    m_Light->SetOuterConeAngle(light.spot.outerConeAngle);
                    m_Light->m_type = "spot";

					m_Lights[light.name] = m_Light;
                    m_LinearLights.emplace_back(m_Light);
                }
            }
        }

        virtual std::shared_ptr<Scene> Parse(const std::string &FileName) final
        {
            std::shared_ptr<Scene> pScene(new Scene(FileName));
            // TODO: delete here after debug passes
            if (pScene->name == "")
                assert("File path must not be empty");

            std::string filePath = g_pAssetLoader->GetFilePath(FileName.c_str());

            bool binary = false;
            size_t extpos = filePath.rfind('.', filePath.length());
            if (extpos != std::string::npos)
            {
                binary = (filePath.substr(extpos + 1, filePath.length() - extpos) == "glb");
            }

            std::string error;
            std::string warning;
            tinygltf::Model gltf_model;

            tinygltf::TinyGLTF loader;
            bool fileLoaded = false;
            if (binary)
                fileLoaded = loader.LoadBinaryFromFile(&gltf_model, &error, &warning, filePath);
            else
                fileLoaded = loader.LoadASCIIFromFile(&gltf_model, &error, &warning, filePath);

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

            std::string basePath;
            extpos = filePath.rfind('/', filePath.length());
            if (extpos != std::string::npos)
            {
                basePath = filePath.substr(0, extpos + 1);
            }

            // LoadTextureSamplers(pDevice, gltf_model);
            LoadMaterialsAndTextures(gltf_model, pScene, basePath);

            LoadLights(gltf_model, pScene);

            // these are vertices and indices of all primitive (accumulated)
            std::vector<VertexBasicAttribs> VertexData;
            std::vector<uint32_t> IndexData;

            ConvertedBufferViewMap ConvertedBuffers;

            // pScene->pModel = std::make_unique<Model>();
            // std::unique_ptr<Model> &m_pModel = pScene->pModel;
            // std::unique_ptr<Model> model = std::make_unique<Model>( Model::CreateInfo{filePath} );
            // std::unique_ptr<Model>& m_pModel = model;
            // auto& m_Cameras = pScene->Cameras;
            // auto& m_Geometries = pScene->Geometries;

            // TODO: scene handling with no default scene
            const tinygltf::Scene &scene = gltf_model.scenes[gltf_model.defaultScene > -1 ? gltf_model.defaultScene : 0];
            for (size_t i = 0; i < scene.nodes.size(); i++)
            {
                const tinygltf::Node node = gltf_model.nodes[scene.nodes[i]];
                LoadNode(nullptr, node, scene.nodes[i], gltf_model,
                         VertexData, IndexData, ConvertedBuffers, pScene);
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
            for (auto &root_node : pScene->RootNodes)
            {
                root_node.lock()->UpdateTransforms();
            }
            // TODO: use this function to get boundbox and BVH
            // CalculateSceneDimensions();

            // UpdatePrimitiveData();
            return pScene;
        }
    };
}
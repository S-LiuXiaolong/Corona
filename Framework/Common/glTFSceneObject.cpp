#include "glTFSceneObject.h"
#include "tinyglTF/tiny_gltf.h"

namespace Corona
{
    Matrix4X4f SceneNode::LocalMatrix() const
    {
        // Translation, rotation, and scale properties and local space transformation are
        // mutually exclusive in GLTF.
        // We, however, may use non-trivial Matrix with TRS to apply transform to a model.
        Matrix4X4f mat = Matrix;

        MatrixTranslation(mat, Translation.x, Translation.y, Translation.z);
        MatrixRotationQuaternion(mat, Rotation);
        MatrixScale(mat, Scale.x, Scale.y, Scale.z);

        return mat;
    }

    // get matrix from child node to parent iteratively
    Matrix4X4f SceneNode::GetMatrix() const
    {
        auto mat = LocalMatrix();

        for (auto *p = Parent; p != nullptr; p = p->Parent)
        {
            mat = mat * p->LocalMatrix();
        }
        return mat;
    }

    void SceneNode::UpdateTransforms()
    {
        const auto NodeTransform = (pMesh || pCamera) ? GetMatrix() : BuildIdentityMatrix();
        if (pMesh)
        {
            pMesh->Transforms.matrix = NodeTransform;
            // if (pSkin != nullptr)
            // {
            //     // Update join matrices
            //     auto InverseTransform = pMesh->Transforms.matrix.Inverse(); // TODO: do not use inverse transform here
            //     if (pMesh->Transforms.jointMatrices.size() != pSkin->Joints.size())
            //         pMesh->Transforms.jointMatrices.resize(pSkin->Joints.size());
            //     for (size_t i = 0; i < pSkin->Joints.size(); i++)
            //     {
            //         auto* JointNode = pSkin->Joints[i];
            //         pMesh->Transforms.jointMatrices[i] =
            //             pSkin->InverseBindMatrices[i] * JointNode->GetMatrix() * InverseTransform;
            //     }
            // }
        }

        if (pCamera)
        {
            pCamera->matrix = NodeTransform;
        }

        for (auto &child : Children)
        {
            child->UpdateTransforms();
        }
    }

    void Model::LoadFromFile(const CreateInfo &CI)
    {
        if (CI.FileName == "")
            assert("File path must not be empty");

        const std::string filename{CI.FileName};

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

        if (!fileLoaded)
        {
            printf("Failed to load gltf file ", filename, ": ", error);
        }
        if (!warning.empty())
        {
            printf("Loaded gltf file ", filename, " with the following warning:", warning);
        }
        if (!error.empty())
        {
            printf("Loaded gltf file ", filename, " with the following error:", error);
        }

        std::vector<uint32_t> IndexData;
        std::vector<VertexBasicAttribs> VertexBasicData;
        ConvertedBufferViewMap ConvertedBuffers;

        // TODO: scene handling with no default scene
        const tinygltf::Scene &scene = gltf_model.scenes[gltf_model.defaultScene > -1 ? gltf_model.defaultScene : 0];
        for (size_t i = 0; i < scene.nodes.size(); i++)
        {
            const tinygltf::Node node = gltf_model.nodes[scene.nodes[i]];
            LoadNode(nullptr, node, scene.nodes[i], gltf_model,
                     IndexData, VertexBasicData, ConvertedBuffers);
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
        for (auto &root_node : Nodes)
        {
            root_node->UpdateTransforms();
        }
        // TODO: use this function to get boundbox and BVH
        // CalculateSceneDimensions();

        UpdatePrimitiveData();
    }

    void Model::LoadNode(SceneNode *parent,
                         const tinygltf::Node &gltf_node,
                         uint32_t nodeIndex,
                         const tinygltf::Model &gltf_model,
                         std::vector<uint32_t> &IndexData,
                         std::vector<VertexBasicAttribs> &VertexBasicData,
                         ConvertedBufferViewMap &ConvertedBuffers)
    {
        std::unique_ptr<SceneNode> NewNode{new SceneNode{}};
        NewNode->Index = nodeIndex;
        NewNode->Parent = parent;
        NewNode->Name = gltf_node.name;
        NewNode->Matrix = BuildIdentityMatrix();

        // Any node can define a local space transformation either by supplying a matrix property,
        // or any of translation, rotation, and scale properties (also known as TRS properties).

        // Generate local node matrix
        // float3 Translation;
        if (gltf_node.translation.size() == 3)
        {
            NewNode->Translation = Vector3f{gltf_node.translation[0], gltf_node.translation[1], gltf_node.translation[2]};
        }

        if (gltf_node.rotation.size() == 4)
        {
            NewNode->Rotation = Vector4f{gltf_node.rotation[0], gltf_node.rotation[1], gltf_node.rotation[2], gltf_node.rotation[3]};
        }

        if (gltf_node.scale.size() == 3)
        {
            NewNode->Scale = Vector3f{gltf_node.scale[0], gltf_node.scale[1], gltf_node.scale[2]};
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
                        gltf_model, IndexData, VertexBasicData, ConvertedBuffers);
            }
        }

        if (gltf_node.mesh >= 0)
        {
            const tinygltf::Mesh& gltf_mesh = gltf_model.meshes[gltf_node.mesh];
            std::unique_ptr<SceneObjectMesh> pNewMesh{new SceneObjectMesh{NewNode->Matrix}};

            for (size_t j = 0; j < gltf_mesh.primitives.size(); j++)
            {
                const tinygltf::Primitive& primitive = gltf_mesh.primitives[j];

                uint32_t indexStart = static_cast<uint32_t> (IndexData.size());
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

                        const tinygltf::Accessor& posAccessor = gltf_model.accessors[position_it->second];

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

                    if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
                    {
                        Key.UV0Access = primitive.attributes.find("TEXCOORD_0")->second;
                    }
                    if (primitive.attributes.find("TEXCOORD_1") != primitive.attributes.end())
                    {
                        Key.UV1Access = primitive.attributes.find("TEXCOORD_1")->second;
                    }

                    // TODO: Skinning

                    // Indices
                    if (hasIndices)
                    {
                        const tinygltf::Accessor&   accessor   = gltf_model.accessors[primitive.indices > -1 ? primitive.indices : 0];
                        const tinygltf::BufferView& bufferView = gltf_model.bufferViews[accessor.bufferView];
                        const tinygltf::Buffer&     buffer     = gltf_model.buffers[bufferView.buffer];

                        indexCount = static_cast<uint32_t>(accessor.count);

                        const void* dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

                        IndexData.reserve(IndexData.size() + accessor.count);
                        switch (accessor.componentType)
                        {
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                            {
                                const uint32_t* buf = static_cast<const uint32_t*>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index] + vertexStart);
                                }
                                break;
                            }
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                            {
                                const uint16_t* buf = static_cast<const uint16_t*>(dataPtr);
                                for (size_t index = 0; index < accessor.count; index++)
                                {
                                    IndexData.push_back(buf[index] + vertexStart);
                                }
                                break;
                            }
                            case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                            {
                                const uint8_t* buf = static_cast<const uint8_t*>(dataPtr);
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

                    pNewMesh->Primitives.emplace_back( //
                        indexStart,
                        indexCount,
                        vertexCount
                        // primitive.material >= 0 ? static_cast<uint32_t>(primitive.material) : static_cast<uint32_t>(Materials.size() - 1),
                        // PosMin,
                        // PosMax
                        //
                    );
                }
            }

            NewNode->pMesh = std::move(pNewMesh);
        }
    }

    void Model::ConvertBuffers(const ConvertedBufferViewKey &Key,
                               ConvertedBufferViewData &Data,
                               const tinygltf::Model &gltf_model,
                               std::vector<VertexBasicAttribs> &VertexBasicData) const
    {
    }
}
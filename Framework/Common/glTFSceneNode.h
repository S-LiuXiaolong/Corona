#pragma once
#include "gltfSceneObject.h"

namespace Corona
{
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

        std::vector<std::unique_ptr<SceneNode>> Nodes;
        std::vector<SceneNode *> LinearNodes;

        // std::vector<std::unique_ptr<Material>> Materials;
        std::vector<std::string> Extensions;

        Model();
        ~Model();

    };
}

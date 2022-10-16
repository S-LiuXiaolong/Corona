#pragma once
#include <map>
#include <string>
#include <vector>
#include "geommath.h"
#include "Tree.h"

namespace Corona
{
    // struct SceneNode
    // {
    //     std::string Name;
    //     SceneNode *Parent = nullptr;
    //     uint32_t Index;

    //     std::vector<std::unique_ptr<SceneNode>> Children;

    //     Matrix4X4f Matrix;
    //     std::shared_ptr<SceneObjectMesh> pMesh;
    //     std::shared_ptr<SceneObjectCamera> pCamera;
    //     Vector3f Translation;
    //     Vector3f Scale;
    //     Quaternion Rotation;

    //     Matrix4X4f LocalMatrix() const;
    //     Matrix4X4f GetMatrix() const;
    //     void UpdateTransforms();
    // };
    
    template <typename T>
    class SceneNode 
    {
    protected:
        std::string m_strName;

        SceneNode *Parent = nullptr;
        // ? index
        uint32_t Index;

        std::vector<std::unique_ptr<SceneNode>> m_Children;

        // std::map<int, std::shared_ptr<SceneObjectAnimationClip>> m_AnimationClips;
        Matrix4X4f Matrix;
        Vector3f Translation;
        Vector3f Scale;
        Quaternion Rotation;
        Matrix4X4f m_Transform;

    protected:
        virtual void dump(std::ostream& out) const {};
    
    public:
        // typedef std::map<int, std::shared_ptr<SceneObjectAnimationClip>>::const_iterator animation_clip_iterator;

    public:
        SceneNode() { BuildIdentityMatrix(m_Transform); };
        SceneNode(const std::string& name) { m_strName = name; BuildIdentityMatrix(m_Transform); };
        virtual ~SceneNode() {};

        const std::string GetName() const { return m_strName; };

        // void AttachAnimationClip(int clip_index, std::shared_ptr<SceneObjectAnimationClip> clip)
        // {
        //     m_AnimationClips.insert({clip_index, clip});
        // }

        // inline bool GetFirstAnimationClip(animation_clip_iterator& it) 
        // { 
        //     it = m_AnimationClips.cbegin(); 
        //     return it != m_AnimationClips.cend();
        // }

        // inline bool GetNextAnimationClip(animation_clip_iterator& it)
        // {
        //     it++;
        //     return it != m_AnimationClips.cend();
        // }

        Matrix4X4f LocalMatrix() const
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
        Matrix4X4f GetMatrix() const
        {
            auto mat = LocalMatrix();

            for (auto *p = Parent; p != nullptr; p = p->Parent)
            {
                mat = mat * p->LocalMatrix();
            }
            return mat;
        }

        virtual void UpdateTransforms()
        {
            // Add these in derivative classed
            // const auto NodeTransform = (pMesh || pCamera) ? GetMatrix() : BuildIdentityMatrix();
            // if (pMesh)
            // {
            //     pMesh->Transforms.matrix = NodeTransform;
            //     // if (pSkin != nullptr)
            //     // {
            //     //     // Update join matrices
            //     //     auto InverseTransform = pMesh->Transforms.matrix.Inverse(); // TODO: do not use inverse transform here
            //     //     if (pMesh->Transforms.jointMatrices.size() != pSkin->Joints.size())
            //     //         pMesh->Transforms.jointMatrices.resize(pSkin->Joints.size());
            //     //     for (size_t i = 0; i < pSkin->Joints.size(); i++)
            //     //     {
            //     //         auto* JointNode = pSkin->Joints[i];
            //     //         pMesh->Transforms.jointMatrices[i] =
            //     //             pSkin->InverseBindMatrices[i] * JointNode->GetMatrix() * InverseTransform;
            //     //     }
            //     // }
            // }

            // if (pCamera)
            // {
            //     pCamera->matrix = NodeTransform;
            // }

            for (auto &child : m_Children)
            {
                child->UpdateTransforms();
            }
        }

        void RotateBy(float rotation_angle_x, float rotation_angle_y, float rotation_angle_z)
        {
            Matrix4X4f rotate;
            MatrixRotationYawPitchRoll(rotate, rotation_angle_x, rotation_angle_y, rotation_angle_z);
            m_Transform = m_Transform * rotate;
        }

        void MoveBy(float distance_x, float distance_y, float distance_z)
        {
            Matrix4X4f translation;
            MatrixTranslation(translation, distance_x, distance_y, distance_z);
            m_Transform = m_Transform * translation;
        }

        void MoveBy(const Vector3f& distance)
        {
            MoveBy(distance[0], distance[1], distance[2]);
        }

        // TODO
        virtual Matrix3X3f GetLocalAxis()
        {
            return {1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f};
        }

        friend std::ostream& operator<<(std::ostream& out, const SceneNode& node)
        {
            static thread_local int32_t indent = 0;
            indent++;

            out << std::string(indent, ' ') << "Scene Node" << std::endl;
            out << std::string(indent, ' ') << "----------" << std::endl;
            out << std::string(indent, ' ') << "Name: " << node.m_strName << std::endl;
            node.dump(out);
            out << std::endl;

            for (auto& sub_node : node.m_Children) {
                out << *sub_node << std::endl;
            }

            out << *node.m_Transform << std::endl;

            // for (auto anim_clip : node.m_AnimationClips) {
            //     out << *anim_clip.second << std::endl;
            // }

            indent--;

            return out;
        }
    };

    // turn Model to Scene class

    // struct Model
    // {
    //     /// Model create information
    //     struct CreateInfo
    //     {
    //         std::string FileName = "";

    //         CreateInfo() = default;
    //         explicit CreateInfo(std::string _FileName) : FileName(_FileName){};
    //     };

    //     std::vector<std::unique_ptr<SceneNode>> Nodes;
    //     std::vector<SceneNode *> LinearNodes;

    //     std::vector<SceneObjectTexture> Textures;
    //     std::vector<SceneObjectMaterial> Materials;

    //     // std::vector<std::unique_ptr<Material>> Materials;
    //     std::vector<std::string> Extensions;

    //     Model();
    //     ~Model();
    // };

}

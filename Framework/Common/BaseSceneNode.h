#pragma once
#include <map>
#include <string>
#include <vector>
#include "geommath.h"
#include "Tree.h"
#include "SceneObject.h"

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
    
    class BaseSceneNode 
    {
    // TODO: public or protected ?
    public:
        std::string m_strName;

        BaseSceneNode *m_Parent = nullptr;
        // ? index
        uint32_t Index;

        std::vector<std::shared_ptr<BaseSceneNode>> m_Children;

        // std::map<int, std::shared_ptr<SceneObjectAnimationClip>> m_AnimationClips;
        Matrix4X4f Matrix;
        Vector3f Translation;
        Vector3f Scale;
        Quaternion Rotation;

    protected:
        virtual void dump(std::ostream& out) const {};
    
    public:
        // typedef std::map<int, std::shared_ptr<SceneObjectAnimationClip>>::const_iterator animation_clip_iterator;

    public:
        BaseSceneNode() { BuildIdentityMatrix(Matrix); };
        BaseSceneNode(const std::string& name) { m_strName = name; };
        virtual ~BaseSceneNode() {};

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

        void AppendChild(std::shared_ptr<BaseSceneNode>&& sub_node)
        {
            sub_node->m_Parent = this;
            m_Children.push_back(std::move(sub_node));
        }

        Matrix4X4f GetLocalTransform() const
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
        Matrix4X4f GetGlobalTransform() const
        {
            auto mat = GetLocalTransform();

            for (auto *p = m_Parent; p != nullptr; p = p->m_Parent)
            {
                mat = mat * p->GetLocalTransform();
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

        // TODO
        void RotateBy(float rotation_angle_x, float rotation_angle_y, float rotation_angle_z)
        {
            Matrix4X4f rotate;
            MatrixRotationYawPitchRoll(rotate, rotation_angle_x, rotation_angle_y, rotation_angle_z);
            Matrix = Matrix * rotate;
        }

        void MoveBy(float distance_x, float distance_y, float distance_z)
        {
            Matrix4X4f translation;
            MatrixTranslation(translation, distance_x, distance_y, distance_z);
            Matrix = Matrix * translation;
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

        friend std::ostream& operator<<(std::ostream& out, const BaseSceneNode& node)
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

            out << node.Matrix << std::endl;

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
    template <typename T>
    class SceneNode : public BaseSceneNode {
        protected:
            std::string m_keySceneObject;

        protected:
            virtual void dump(std::ostream& out) const 
            { 
                out << m_keySceneObject << std::endl;
            };

        public:
            using BaseSceneNode::BaseSceneNode;
            SceneNode() = default;

            void AddSceneObjectRef(const std::string& key) { m_keySceneObject = key; };

            const std::string& GetSceneObjectRef() { return m_keySceneObject; };
    };

    typedef BaseSceneNode SceneEmptyNode;

}

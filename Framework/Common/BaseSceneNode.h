#pragma once
#include <map>
#include <string>
#include <vector>
#include "geommath.h"
#include "Tree.h"
#include "SceneObject.h"

namespace Corona
{
	struct TransformData
	{
		Matrix4X4f              matrix;
		// std::vector<Matrix4X4f> jointMatrices;
	};

    class SceneNode
    {
	public:
		std::string m_strName;
		std::shared_ptr<SceneObjectMesh> pMesh;
		std::shared_ptr<SceneObjectCamera> pCamera;

		SceneNode* m_Parent = nullptr;
		// ? index
		uint32_t Index;

		std::vector<std::shared_ptr<SceneNode>> m_Children;

		// std::map<int, std::shared_ptr<SceneObjectAnimationClip>> m_AnimationClips;
		Matrix4X4f Matrix;
		Vector3f Translation;
		Vector3f Scale;
		Quaternion Rotation;
		TransformData Transforms;

	protected:
		virtual void dump(std::ostream& out) const {};

	public:
		// typedef std::map<int, std::shared_ptr<SceneObjectAnimationClip>>::const_iterator animation_clip_iterator;

	public:
		SceneNode() : Scale({ 1.0f, 1.0f, 1.0f }) {
		BuildIdentityMatrix(Matrix); BuildIdentityMatrix(Transforms.matrix);
		};
		// SceneNode() : Scale({ 1.0f, 1.0f, 1.0f }) {
		// 	BuildIdentityMatrix(Matrix);
		// };
		SceneNode(const std::string& name) : SceneNode() { m_strName = name; };

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

		void AppendChild(std::shared_ptr<SceneNode>&& sub_node)
		{
			sub_node->m_Parent = this;
			m_Children.push_back(std::move(sub_node));
		}

    public:
        Matrix4X4f GetLocalTransform() const
        {
            // Translation, rotation, and scale properties and local space transformation are
            // mutually exclusive in GLTF.
            // We, however, may use non-trivial Matrix with TRS to apply transform to a model.

			// TODO: BULLSHIT MATH
			Matrix4X4f matForCalc = BuildIdentityMatrix();
            Matrix4X4f mat = Matrix;

            MatrixTranslation(matForCalc, Translation.x, Translation.y, Translation.z);
			mat = mat * matForCalc;
            MatrixRotationQuaternion(matForCalc, Rotation);
			mat = mat * matForCalc;
            MatrixScale(matForCalc, Scale.x, Scale.y, Scale.z);
			mat = mat * matForCalc;

            return mat;
        }

        // get matrix from child node to parent iteratively
        Matrix4X4f GetGlobalTransform() const
        {
            auto mat = GetLocalTransform();

            for (auto* p = m_Parent; p != nullptr; p = p->m_Parent)
            {
				auto temp = p->GetLocalTransform();
                mat = mat * p->GetLocalTransform();
            }
            return mat;
        }

        virtual void UpdateTransforms()
        {
            // Add these in derivative classed
            const auto NodeTransform = (pMesh || pCamera) ? GetGlobalTransform() : BuildIdentityMatrix();
            if (pMesh)
            {
                this->Transforms.matrix = NodeTransform;
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
				this->Transforms.matrix = NodeTransform;
            }

            for (auto& child : m_Children)
            {
                child->UpdateTransforms();
            }
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

			for (auto& sub_node : node.m_Children)
			{
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

    typedef SceneNode SceneEmptyNode;

}

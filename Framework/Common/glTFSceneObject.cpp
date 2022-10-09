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

	Model::Model()
	{
		Nodes = std::vector<std::unique_ptr<SceneNode>>{};
		LinearNodes = std::vector<SceneNode*>{};

		Extensions = std::vector<std::string>{};
	}

    Model::~Model()
    {
    }

}
#pragma once
#include "BaseSceneNode.h"

namespace Corona
{
    class SceneCameraNode : public SceneNode
    {
    public:
        Vector3f m_Target = { 0.0f }; // initial "look at" direction vector

        Vector3f WorldUp = { 0.0f, 1.0f, 0.0f };
		Vector3f mRight = { 1.0f, 0.0f, 0.0f };
		Vector3f mUp = { 0.0f, 1.0f, 0.0f };
		Vector3f mLook = { 0.0f, 0.0f, 1.0f };

        Vector3f mPosition = { 0.0f };

        std::shared_ptr<SceneObjectCamera> pCamera;

        bool isInit = false;

    public:
        using SceneNode::SceneNode;
        SceneCameraNode(SceneNode&& node)
            : SceneNode(std::move(node)) {};
		SceneCameraNode(SceneNode& node)
			: SceneNode(std::move(node)) {}; // 传入的怎么会是左值啊（大雾）

        void SetTarget(Vector3f &target) { m_Target = target; };
        const Vector3f &GetTarget() { return m_Target; };

        // void InitMatrix()
        // {
		// 	Matrix4X4f result;
		// 	BuildIdentityMatrix(result);

		// 	auto pTransform = Transforms.matrix;
		// 	Vector3f target = GetTarget();
		// 	Vector3f mPosition = { 0.0f, 0.0f, 0.0f };
		// 	TransformCoord(mPosition, pTransform);

		// 	mLook = target - mPosition;
		// 	Normalize(mLook);
		// 	CrossProduct(mRight, WorldUp, mLook);
		// 	CrossProduct(mUp, mLook, mRight);
        // }
        // TODO
        // set it from Matrix3X3f to Matrix4X4f cause of calculating conveniently
        // Matrix4X4f GetInitAxis() // Get local axis in the world space
        // {
        //     Matrix4X4f result;
        //     BuildIdentityMatrix(result);

        //     memcpy(result[0], mRight.data, sizeof(mRight));
        //     memcpy(result[1], mUp.data, sizeof(mUp));
        //     memcpy(result[2], mLook.data, sizeof(mLook));

        //     return result;
        // }

        Matrix4X4f GetViewMatrix()
        {
            if (!isInit)
            {
				Matrix4X4f result;
				BuildIdentityMatrix(result);

				auto pTransform = Transforms.matrix;
				Vector3f target = GetTarget();
				// Vector3f mPosition = { 0.0f, 0.0f, 0.0f };
				TransformCoord(mPosition, pTransform);

				mLook = target - mPosition;
				Normalize(mLook);
				CrossProduct(mRight, WorldUp, mLook);
				CrossProduct(mUp, mLook, mRight);

                isInit = true;
            }
            // auto& initAxis = GetInitAxis();
            Matrix4X4f result;
            auto pTransform = Transforms.matrix;
            Vector3f camera_position = Vector3f(0.0f);
            TransformCoord(camera_position, pTransform);

            // auto temp = initAxis * pTransform;

			// Vector3f right; // xAxis
			// memcpy(right.data, (initAxis * pTransform)[0], sizeof(right));
			// Normalize(right);

            // Vector3f up; // yAxis
            // memcpy(up.data, (initAxis * pTransform)[1], sizeof(up));
            // Normalize(up);

			// Vector3f look; // zAxis
			// memcpy(look.data, (initAxis * pTransform)[2], sizeof(look));
			// Normalize(look);

            // BuildViewMatrix(result, camera_position, look, up);

			float result1, result2, result3;
            Vector3f right = this->mRight;
            Vector3f up = this->mUp;
            Vector3f look = this->mLook;

            Normalize(look);
            CrossProduct(up, look, right);
            Normalize(up);
            CrossProduct(right, up, look);

            mRight = right;
            mUp = up;
            mLook = look;

			DotProduct(result1, right, mPosition);
			result1 = -result1;

			DotProduct(result2, up, mPosition);
			result2 = -result2;

			DotProduct(result3, look, mPosition);
			result3 = -result3;

			// Set the computed values in the view matrix.
			Matrix4X4f tmp = { {{{mRight.x, mUp.x, mLook.x, 0.0f},
								{mRight.y, mUp.y, mLook.y, 0.0f},
								{mRight.z, mUp.z, mLook.z, 0.0f},
								{result1, result2, result3, 1.0f}}} };

			result = tmp;
            // BuildViewMatrix(result, camera_position, mLook, mUp);

            return result;
        }

        // for test
        void Pitch(float angle)
        {
            Matrix4X4f R;
            MatrixRotationAxis(R, mRight, angle);

            TransformCoord(mUp, R);
            TransformCoord(mLook, R);
        }

        void RotateY(float angle)
        {
            Matrix4X4f R;
            MatrixRotationY(R, angle);

			TransformCoord(mRight, R);
			TransformCoord(mUp, R);
			TransformCoord(mLook, R);
        }

        void Strafe(float d)
        {
            // mPosition += d*mRight;
            Vector3f s = { d };
            Vector3f r = mRight;
            Vector3f p = mPosition;

            Vector3f res;
            MulByElement(res, s, r);
            mPosition = res + p;
        }

		void Walk(float d)
		{
			// mPosition += d*mRight;
			Vector3f s = { d };
			Vector3f l = mLook;
			Vector3f p = mPosition;

			Vector3f res;
			MulByElement(res, s, l);
			mPosition = res + p;
		}
    };
}
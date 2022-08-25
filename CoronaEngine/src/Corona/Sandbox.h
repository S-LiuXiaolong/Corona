#include "Corona.h"
#include "Utility/UploadBuffer.h"
#include "Utility/Camera.h"

namespace Corona {

	using Microsoft::WRL::ComPtr;
	using namespace DirectX;
	using namespace DirectX::PackedVector;
	
	//struct Vertex
	//{
	//	XMFLOAT3 Pos;
	//	XMFLOAT4 Color;
	//};
	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 n;
	};

	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	};

	class Sandbox : public Application
	{
	public:
		Sandbox(HINSTANCE hInstance);
		Sandbox(const Sandbox& rhs) = delete;
		Sandbox& operator=(const Sandbox& rhs) = delete;
		~Sandbox();

		virtual bool Initialize()override;

	private:
		virtual void OnResize()override;
		virtual void Update(const GameTimer& gt)override;
		virtual void Draw(const GameTimer& gt)override;

		virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
		virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
		virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

		void OnKeyboardInput(const GameTimer& gt);

		void BuildDescriptorHeaps();
		void BuildConstantBuffers();
		void BuildRootSignature();
		void BuildShadersAndInputLayout();
		//void BuildBoxGeometry();
		void BuildModelGeometry();
		void BuildPSO();

	private:

		ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
		ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

		std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

		std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

		ComPtr<ID3DBlob> mvsByteCode = nullptr;
		ComPtr<ID3DBlob> mpsByteCode = nullptr;

		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

		ComPtr<ID3D12PipelineState> mPSO = nullptr;

		XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
		XMFLOAT4X4 mView = MathHelper::Identity4x4();
		XMFLOAT4X4 mProj = MathHelper::Identity4x4();

		float mTheta = 1.5f * XM_PI;
		float mPhi = XM_PIDIV4;
		float mRadius = 5.0f;

		POINT mLastMousePos;

		// Our ImGui state
		bool show_demo_window = true;

		Camera mCamera;
	};

}



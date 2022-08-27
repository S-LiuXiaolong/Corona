#pragma once

#include "Corona.h"
#include "Utility/UploadBuffer.h"
#include "Utility/Camera.h"
#include "Utility/GeometryGenerator.h"
#include "FrameResource.h"

// Cannot use definition here. Maybe re-def somewhere.
extern const int gNumFrameResources;

namespace Corona {

	using Microsoft::WRL::ComPtr;
	using namespace DirectX;
	using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")


	// Lightweight structure stores parameters to draw a shape.  This will
	// vary from app-to-app.
	struct RenderItem
	{
		RenderItem() = default;
		RenderItem(const RenderItem& rhs) = delete;

		// World matrix of the shape that describes the object's local space
		// relative to the world space, which defines the position, orientation,
		// and scale of the object in the world.
		XMFLOAT4X4 World = MathHelper::Identity4x4();
		XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

		// Dirty flag indicating the object data has changed and we need to update the constant buffer.
		// Because we have an object cbuffer for each FrameResource, we have to apply the
		// update to each FrameResource.  Thus, when we modify obect data we should set 
		// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
		int NumFramesDirty = gNumFrameResources;

		// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
		UINT ObjCBIndex = -1;

		Material* Mat = nullptr;
		MeshGeometry* Geo = nullptr;

		// Primitive topology.
		D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		BoundingBox Bounds;
		std::vector<InstanceData> Instances;

		// DrawIndexedInstanced parameters.
		UINT IndexCount = 0;
		UINT InstanceCount = 0;
		UINT StartIndexLocation = 0;
		int BaseVertexLocation = 0;
	};

	enum class RenderLayer : int
	{
		Opaque = 0,
		Sky,
		Count
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
		void AnimateMaterials(const GameTimer& gt);
		void UpdateInstanceData(const GameTimer& gt);
		void UpdateMaterialBuffer(const GameTimer& gt);
		void UpdateMainPassCB(const GameTimer& gt);

		void LoadTextures();
		void BuildDescriptorHeaps();
		void BuildRootSignature();
		void BuildShadersAndInputLayout();
		void BuildShapeGeometry();
		void BuildMyModelGeometry();
		void BuildSkullGeometry();
		void BuildPSOs();
		void BuildFrameResources();
		void BuildMaterials();
		void BuildMyRenderItems();
		void BuildRenderItems();
		void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

		std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	private:

		std::vector<std::unique_ptr<FrameResource>> mFrameResources;
		FrameResource* mCurrFrameResource = nullptr;
		int mCurrFrameResourceIndex = 0;

		UINT mCbvSrvDescriptorSize = 0;

		ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

		ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

		std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
		std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
		std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
		std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;
		std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;

		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

		// List of all the render items.
		std::vector<std::unique_ptr<RenderItem>> mAllRitems;

		// Render items divided by PSO.
		std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];

		UINT mInstanceCount = 0;

		bool mFrustumCullingEnabled = true;

		BoundingFrustum mCamFrustum;

		UINT mSkyTexHeapIndex = 0;

		PassConstants mMainPassCB;

		Camera mCamera;

		POINT mLastMousePos;

		// ImGui window
		bool show_demo_window = true;
	};

}



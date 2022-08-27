#include "crpch.h"
#include "Sandbox.h"
#include <DirectXColors.h>
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Corona {

	using namespace DirectX;

	static ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Sandbox::Sandbox(HINSTANCE hInstance)
		: Application(hInstance)
	{
	}

	Sandbox::~Sandbox()
	{
		if (md3dDevice != nullptr)
			FlushCommandQueue();
	}

	bool Sandbox::Initialize()
	{
		if (!Application::Initialize())
			return false;

		// Reset the command list to prep for initialization commands.
		ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

		// Get the increment size of a descriptor in this heap type.  This is hardware specific, 
		// so we have to query this information.
		mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


		mCamera.SetPosition(0.0f, 2.0f, -15.0f);

// 		BuildDescriptorHeaps();
// 		BuildConstantBuffers();
// 		BuildRootSignature();
// 		BuildShadersAndInputLayout();
// 		//BuildBoxGeometry();
// 		BuildModelGeometry();
// 		BuildPSO();
		LoadTextures();
		BuildRootSignature();
		BuildDescriptorHeaps();
		BuildShadersAndInputLayout();
		BuildShapeGeometry();
		//BuildMyModelGeometry();
		BuildSkullGeometry();
		BuildMaterials();
		//BuildMyRenderItems();
		BuildRenderItems();
		BuildFrameResources();
		BuildPSOs();

		// Execute the initialization commands.
		ThrowIfFailed(mCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until initialization is complete.
		FlushCommandQueue();

		return true;
	}

	void Sandbox::OnResize()
	{
		Application::OnResize();

		// The window resized, so update the aspect ratio and recompute the projection matrix.
		mCamera.SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

		BoundingFrustum::CreateFromMatrix(mCamFrustum, mCamera.GetProj());
	}

	void Sandbox::Update(const GameTimer& gt)
	{
		OnKeyboardInput(gt);

		// Cycle through the circular frame resource array.
		mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
		mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

		// Has the GPU finished processing the commands of the current frame resource?
		// If not, wait until the GPU has completed commands up to this fence point.
		if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
			ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}

		AnimateMaterials(gt);
		UpdateInstanceData(gt);
		UpdateMaterialBuffer(gt);
		UpdateMainPassCB(gt);
	}

	// TODO: Rewrite the exception class here
	void Sandbox::Draw(const GameTimer& gt)
	{
		auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished execution on the GPU.
		ThrowIfFailed(cmdListAlloc->Reset());

		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));

// 		// Start the Dear ImGui frame
// 		ImGui_ImplDX12_NewFrame();
// 		ImGui_ImplWin32_NewFrame();
// 		ImGui::NewFrame();
// 
// 		if (show_demo_window)
// 			ImGui::ShowDemoWindow(&show_demo_window);
// 
// 		// Rendering
// 		ImGui::Render();

		mCommandList->RSSetViewports(1, &mScreenViewport);
		mCommandList->RSSetScissorRects(1, &mScissorRect);

		// Indicate a state transition on the resource usage.
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		// Clear the back buffer and depth buffer.
		const float clearColorWithAlpha[4] = { clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w };
		mCommandList->ClearRenderTargetView(CurrentBackBufferView(), clearColorWithAlpha, 0, nullptr);
		mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Specify the buffers we are going to render to.
		mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

		ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvDescriptorHeap.Get() };
		mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

		// Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
		// set as a root descriptor.
		auto matBuffer = mCurrFrameResource->MaterialBuffer->Resource();
		mCommandList->SetGraphicsRootShaderResourceView(1, matBuffer->GetGPUVirtualAddress());

		auto passCB = mCurrFrameResource->PassCB->Resource();
		mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

		// Bind the sky cube map.  For our demos, we just use one "world" cube map representing the environment
		// from far away, so all objects will use the same cube map and we only need to set it once per-frame.  
		// If we wanted to use "local" cube maps, we would have to change them per-object, or dynamically
		// index into an array of cube maps.
		CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		skyTexDescriptor.Offset(mSkyTexHeapIndex, mCbvSrvDescriptorSize);
		mCommandList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);

		// Bind all the textures used in this scene.
		mCommandList->SetGraphicsRootDescriptorTable(4, mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

		DrawRenderItems(mCommandList.Get(), mRitemLayer[(int)RenderLayer::Opaque]);

		mCommandList->SetPipelineState(mPSOs["sky"].Get());
		DrawRenderItems(mCommandList.Get(), mRitemLayer[(int)RenderLayer::Sky]);

// 		// Never use '&' to get address here or you will get nothing.
// 		mCommandList->SetDescriptorHeaps(1, mSrvHeap.GetAddressOf());
// 		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mCommandList.Get());

		// Indicate a state transition on the resource usage.
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		// Done recording commands.
		ThrowIfFailed(mCommandList->Close());

		// Add the command list to the queue for execution.
		ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// swap the back and front buffers
		ThrowIfFailed(mSwapChain->Present(0, 0));
		mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

		// Advance the fence value to mark commands up to this fence point.
		mCurrFrameResource->Fence = ++mCurrentFence;

		// Add an instruction to the command queue to set a new fence point. 
		// Because we are on the GPU timeline, the new fence point won't be 
		// set until the GPU finishes processing all the commands prior to this Signal().
		mCommandQueue->Signal(mFence.Get(), mCurrentFence);
	}

	void Sandbox::OnMouseDown(WPARAM btnState, int x, int y)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(mhMainWnd);
	}

	void Sandbox::OnMouseUp(WPARAM btnState, int x, int y)
	{
		ReleaseCapture();
	}

	void Sandbox::OnMouseMove(WPARAM btnState, int x, int y)
	{
		if ((btnState & MK_LBUTTON) != 0)
		{
			// Make each pixel correspond to a quarter of a degree.
			float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
			float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

			mCamera.Pitch(dy);
			mCamera.RotateY(dx);
		}

		mLastMousePos.x = x;
		mLastMousePos.y = y;
	}

	void Sandbox::OnKeyboardInput(const GameTimer& gt)
	{
		const float dt = gt.DeltaTime();

		if (GetAsyncKeyState('W') & 0x8000)
			mCamera.Walk(100.0f * dt);

		if (GetAsyncKeyState('S') & 0x8000)
			mCamera.Walk(-100.0f * dt);

		if (GetAsyncKeyState('A') & 0x8000)
			mCamera.Strafe(-100.0f * dt);

		if (GetAsyncKeyState('D') & 0x8000)
			mCamera.Strafe(100.0f * dt);

		mCamera.UpdateViewMatrix();
	}


	void Sandbox::AnimateMaterials(const GameTimer& gt)
	{

	}

	// TODO
	void Sandbox::UpdateInstanceData(const GameTimer& gt)
	{
		XMMATRIX view = mCamera.GetView();
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);

		auto currInstanceBuffer = mCurrFrameResource->InstanceBuffer.get();
		for (auto& e : mAllRitems)
		{
			const auto& instanceData = e->Instances;

			int visibleInstanceCount = 0;

			for (UINT i = 0; i < (UINT)instanceData.size(); ++i)
			{
				XMMATRIX world = XMLoadFloat4x4(&instanceData[i].World);
				XMMATRIX texTransform = XMLoadFloat4x4(&instanceData[i].TexTransform);

				XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

				// View space to the object's local space.
				XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

				// Transform the camera frustum from view space to the object's local space.
				BoundingFrustum localSpaceFrustum;
				mCamFrustum.Transform(localSpaceFrustum, viewToLocal);

				// Perform the box/frustum intersection test in local space.
				if ((localSpaceFrustum.Contains(e->Bounds) != DirectX::DISJOINT) || (mFrustumCullingEnabled == false))
				{
					InstanceData data;
					XMStoreFloat4x4(&data.World, XMMatrixTranspose(world));
					XMStoreFloat4x4(&data.TexTransform, XMMatrixTranspose(texTransform));
					data.MaterialIndex = instanceData[i].MaterialIndex;

					// Write the instance data to structured buffer for the visible objects.
					currInstanceBuffer->CopyData(visibleInstanceCount++, data);
				}
			}

			//e->InstanceCount = visibleInstanceCount;

			std::wostringstream outs;
			outs.precision(6);
			outs << L"Instancing and Culling Demo" <<
				L"    " << e->InstanceCount <<
				L" objects visible out of " << e->Instances.size();
			mMainWndCaption = outs.str();
		}
	}

	void Sandbox::UpdateMaterialBuffer(const GameTimer& gt)
	{
		auto currMaterialBuffer = mCurrFrameResource->MaterialBuffer.get();
		for (auto& e : mMaterials)
		{
			// Only update the cbuffer data if the constants have changed.  If the cbuffer
			// data changes, it needs to be updated for each FrameResource.
			Material* mat = e.second.get();
			if (mat->NumFramesDirty > 0)
			{
				XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

				MaterialData matData;
				matData.DiffuseAlbedo = mat->DiffuseAlbedo;
				matData.FresnelR0 = mat->FresnelR0;
				matData.Roughness = mat->Roughness;
				XMStoreFloat4x4(&matData.MatTransform, XMMatrixTranspose(matTransform));
				matData.DiffuseMapIndex = mat->DiffuseSrvHeapIndex;

				currMaterialBuffer->CopyData(mat->MatCBIndex, matData);

				// Next FrameResource need to be updated too.
				mat->NumFramesDirty--;
			}
		}
	}

	void Sandbox::UpdateMainPassCB(const GameTimer& gt)
	{
		XMMATRIX view = mCamera.GetView();
		XMMATRIX proj = mCamera.GetProj();

		XMMATRIX viewProj = XMMatrixMultiply(view, proj);
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
		XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
		XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

		XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
		XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
		XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
		XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
		XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
		XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
		mMainPassCB.EyePosW = mCamera.GetPosition3f();
		mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
		mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
		mMainPassCB.NearZ = 1.0f;
		mMainPassCB.FarZ = 1000.0f;
		mMainPassCB.TotalTime = gt.TotalTime();
		mMainPassCB.DeltaTime = gt.DeltaTime();
		mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
		mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
		mMainPassCB.Lights[0].Strength = { 0.8f, 0.8f, 0.8f };
		mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
		mMainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
		mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
		mMainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };

		auto currPassCB = mCurrFrameResource->PassCB.get();
		currPassCB->CopyData(0, mMainPassCB);
	}

	void Sandbox::LoadTextures()
	{
		std::vector<std::string> texNames =
		{
			"bricksDiffuseMap",
			"tileDiffuseMap",
			"defaultDiffuseMap",
			"skyCubeMap"
		};

		std::vector<std::wstring> texFilenames =
		{
			L"../Assets/Textures/bricks2.dds",
			L"../Assets/Textures/tile.dds",
			L"../Assets/Textures/white1x1.dds",
			L"../Assets/Textures/grasscube1024.dds"
		};

		for (int i = 0; i < (int)texNames.size(); ++i)
		{
			auto texMap = std::make_unique<Texture>();
			texMap->Name = texNames[i];
			texMap->Filename = texFilenames[i];
			ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice.Get(),
				mCommandList.Get(), texMap->Filename.c_str(),
				texMap->Resource, texMap->UploadHeap));

			mTextures[texMap->Name] = std::move(texMap);
		}
	}

	void Sandbox::BuildDescriptorHeaps()
	{
		//
		// Create the SRV heap.
		//
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 4;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap)));

		//
		// Fill out the heap with actual descriptors.
		//
		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		auto bricksTex = mTextures["bricksDiffuseMap"]->Resource;
		auto tileTex = mTextures["tileDiffuseMap"]->Resource;
		auto whiteTex = mTextures["defaultDiffuseMap"]->Resource;
		auto skyTex = mTextures["skyCubeMap"]->Resource;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = bricksTex->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = bricksTex->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		md3dDevice->CreateShaderResourceView(bricksTex.Get(), &srvDesc, hDescriptor);

		// next descriptor
		hDescriptor.Offset(1, mCbvSrvDescriptorSize);

		srvDesc.Format = tileTex->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = tileTex->GetDesc().MipLevels;
		md3dDevice->CreateShaderResourceView(tileTex.Get(), &srvDesc, hDescriptor);

		// next descriptor
		hDescriptor.Offset(1, mCbvSrvDescriptorSize);

		srvDesc.Format = whiteTex->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = whiteTex->GetDesc().MipLevels;
		md3dDevice->CreateShaderResourceView(whiteTex.Get(), &srvDesc, hDescriptor);

		// next descriptor
		hDescriptor.Offset(1, mCbvSrvDescriptorSize);

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = skyTex->GetDesc().MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		srvDesc.Format = skyTex->GetDesc().Format;
		md3dDevice->CreateShaderResourceView(skyTex.Get(), &srvDesc, hDescriptor);

		mSkyTexHeapIndex = 3;
	}

	void Sandbox::BuildRootSignature()
	{
		CD3DX12_DESCRIPTOR_RANGE texTable0;
		texTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);

		CD3DX12_DESCRIPTOR_RANGE texTable1;
		texTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 1, 0);

		// Root parameter can be a table, root descriptor or root constants.
		CD3DX12_ROOT_PARAMETER slotRootParameter[6];

		// Perfomance TIP: Order from most frequent to least frequent.
		slotRootParameter[0].InitAsShaderResourceView(0, 1);
		slotRootParameter[1].InitAsShaderResourceView(1, 1);
		slotRootParameter[2].InitAsConstantBufferView(0);
		slotRootParameter[3].InitAsDescriptorTable(1, &texTable0, D3D12_SHADER_VISIBILITY_PIXEL);
		slotRootParameter[4].InitAsDescriptorTable(1, &texTable1, D3D12_SHADER_VISIBILITY_PIXEL);
		slotRootParameter[5].InitAsConstantBufferView(1);
		
		auto staticSamplers = GetStaticSamplers();

		// A root signature is an array of root parameters.
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(6, slotRootParameter,
			(UINT)staticSamplers.size(), staticSamplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(hr);

		ThrowIfFailed(md3dDevice->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(mRootSignature.GetAddressOf())));
	}

	void Sandbox::BuildShadersAndInputLayout()
	{
		const D3D_SHADER_MACRO alphaTestDefines[] =
		{
			"ALPHA_TEST", "1",
			NULL, NULL
		};

		mShaders["standardVS"] = d3dUtil::CompileShader(L"F:\\work_space\\CoronaEngine\\CoronaEngine\\src\\Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
		mShaders["opaquePS"] = d3dUtil::CompileShader(L"F:\\work_space\\CoronaEngine\\CoronaEngine\\src\\Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

		mShaders["skyVS"] = d3dUtil::CompileShader(L"F:\\work_space\\CoronaEngine\\CoronaEngine\\src\\Shaders\\Sky.hlsl", nullptr, "VS", "vs_5_1");
		mShaders["skyPS"] = d3dUtil::CompileShader(L"F:\\work_space\\CoronaEngine\\CoronaEngine\\src\\Shaders\\Sky.hlsl", nullptr, "PS", "ps_5_1");

		mInputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
	}

	void Sandbox::BuildShapeGeometry()
	{
		GeometryGenerator geoGen;
		GeometryGenerator::MeshData box = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
		GeometryGenerator::MeshData grid = geoGen.CreateGrid(20.0f, 30.0f, 60, 40);
		GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
		GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);

		//
		// We are concatenating all the geometry into one big vertex/index buffer.  So
		// define the regions in the buffer each submesh covers.
		//

		// Cache the vertex offsets to each object in the concatenated vertex buffer.
		UINT boxVertexOffset = 0;
		UINT gridVertexOffset = (UINT)box.Vertices.size();
		UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
		UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();

		// Cache the starting index for each object in the concatenated index buffer.
		UINT boxIndexOffset = 0;
		UINT gridIndexOffset = (UINT)box.Indices32.size();
		UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
		UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();

		SubmeshGeometry boxSubmesh;
		boxSubmesh.IndexCount = (UINT)box.Indices32.size();
		boxSubmesh.StartIndexLocation = boxIndexOffset;
		boxSubmesh.BaseVertexLocation = boxVertexOffset;

		SubmeshGeometry gridSubmesh;
		gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
		gridSubmesh.StartIndexLocation = gridIndexOffset;
		gridSubmesh.BaseVertexLocation = gridVertexOffset;

		SubmeshGeometry sphereSubmesh;
		sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
		sphereSubmesh.StartIndexLocation = sphereIndexOffset;
		sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

		SubmeshGeometry cylinderSubmesh;
		cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
		cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
		cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

		//
		// Extract the vertex elements we are interested in and pack the
		// vertices of all the meshes into one vertex buffer.
		//

		auto totalVertexCount =
			box.Vertices.size() +
			grid.Vertices.size() +
			sphere.Vertices.size() +
			cylinder.Vertices.size();

		std::vector<Vertex> vertices(totalVertexCount);

		UINT k = 0;
		for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
		{
			vertices[k].Pos = box.Vertices[i].Position;
			vertices[k].Normal = box.Vertices[i].Normal;
			vertices[k].TexC = box.Vertices[i].TexC;
		}

		for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
		{
			vertices[k].Pos = grid.Vertices[i].Position;
			vertices[k].Normal = grid.Vertices[i].Normal;
			vertices[k].TexC = grid.Vertices[i].TexC;
		}

		for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
		{
			vertices[k].Pos = sphere.Vertices[i].Position;
			vertices[k].Normal = sphere.Vertices[i].Normal;
			vertices[k].TexC = sphere.Vertices[i].TexC;
		}

		for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
		{
			vertices[k].Pos = cylinder.Vertices[i].Position;
			vertices[k].Normal = cylinder.Vertices[i].Normal;
			vertices[k].TexC = cylinder.Vertices[i].TexC;
		}

		std::vector<std::uint16_t> indices;
		indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
		indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
		indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
		indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		auto geo = std::make_unique<MeshGeometry>();
		geo->Name = "shapeGeo";

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
		CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

		geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

		geo->VertexByteStride = sizeof(Vertex);
		geo->VertexBufferByteSize = vbByteSize;
		geo->IndexFormat = DXGI_FORMAT_R16_UINT;
		geo->IndexBufferByteSize = ibByteSize;

		geo->DrawArgs["box"] = boxSubmesh;
		geo->DrawArgs["grid"] = gridSubmesh;
		geo->DrawArgs["sphere"] = sphereSubmesh;
		geo->DrawArgs["cylinder"] = cylinderSubmesh;

		mGeometries[geo->Name] = std::move(geo);
	}

	void Sandbox::BuildMyModelGeometry()
	{
		// TODO: RelAddr here.
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("../Assets/Models/suzanne.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];
		std::vector<Vertex> vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back({
				{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z },
				* reinterpret_cast<XMFLOAT3*>(&pMesh->mNormals[i])
				});
		}
		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		auto geo = std::make_unique<MeshGeometry>();
		geo->Name = "myGeo";

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
		CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

		geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

		geo->VertexByteStride = sizeof(Vertex);
		geo->VertexBufferByteSize = vbByteSize;
		geo->IndexFormat = DXGI_FORMAT_R16_UINT;
		geo->IndexBufferByteSize = ibByteSize;

		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT)indices.size();
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;

		geo->DrawArgs["my"] = submesh;

		mGeometries[geo->Name] = std::move(geo);
	}

	void Sandbox::BuildSkullGeometry()
	{
		std::ifstream fin("F:/work_space/CoronaEngine/Assets/Models/skull.txt");

		if (!fin)
		{
			MessageBox(0, L"model/skull.txt not found.", 0, 0);
			return;
		}

		UINT vcount = 0;
		UINT tcount = 0;
		std::string ignore;

		fin >> ignore >> vcount;
		fin >> ignore >> tcount;
		fin >> ignore >> ignore >> ignore >> ignore;

		XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
		XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

		XMVECTOR vMin = XMLoadFloat3(&vMinf3);
		XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

		std::vector<Vertex> vertices(vcount);
		for (UINT i = 0; i < vcount; ++i)
		{
			fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
			fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

			XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);

			// Project point onto unit sphere and generate spherical texture coordinates.
			XMFLOAT3 spherePos;
			XMStoreFloat3(&spherePos, XMVector3Normalize(P));

			float theta = atan2f(spherePos.z, spherePos.x);

			// Put in [0, 2pi].
			if (theta < 0.0f)
				theta += XM_2PI;

			float phi = acosf(spherePos.y);

			float u = theta / (2.0f * XM_PI);
			float v = phi / XM_PI;

			vertices[i].TexC = { u, v };

			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
		}

		BoundingBox bounds;
		XMStoreFloat3(&bounds.Center, 0.5f * (vMin + vMax));
		XMStoreFloat3(&bounds.Extents, 0.5f * (vMax - vMin));

		fin >> ignore;
		fin >> ignore;
		fin >> ignore;

		std::vector<std::int32_t> indices(3 * tcount);
		for (UINT i = 0; i < tcount; ++i)
		{
			fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
		}

		fin.close();

		//
		// Pack the indices of all the meshes into one index buffer.
		//

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);

		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::int32_t);

		auto geo = std::make_unique<MeshGeometry>();
		geo->Name = "skullGeo";

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
		CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

		geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
			mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

		geo->VertexByteStride = sizeof(Vertex);
		geo->VertexBufferByteSize = vbByteSize;
		geo->IndexFormat = DXGI_FORMAT_R32_UINT;
		geo->IndexBufferByteSize = ibByteSize;

		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT)indices.size();
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;
		submesh.Bounds = bounds;

		geo->DrawArgs["skull"] = submesh;

		mGeometries[geo->Name] = std::move(geo);
	}

	void Sandbox::BuildPSOs()
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

		//
		// PSO for opaque objects.
		//
		ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
		opaquePsoDesc.pRootSignature = mRootSignature.Get();
		opaquePsoDesc.VS =
		{
			reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
			mShaders["standardVS"]->GetBufferSize()
		};
		opaquePsoDesc.PS =
		{
			reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
			mShaders["opaquePS"]->GetBufferSize()
		};
		opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		opaquePsoDesc.SampleMask = UINT_MAX;
		opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		opaquePsoDesc.NumRenderTargets = 1;
		opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
		opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		opaquePsoDesc.DSVFormat = mDepthStencilFormat;
		ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));

		//
		// PSO for sky.
		//
		D3D12_GRAPHICS_PIPELINE_STATE_DESC skyPsoDesc = opaquePsoDesc;

		// The camera is inside the sky sphere, so just turn off culling.
		skyPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// Make sure the depth function is LESS_EQUAL and not just LESS.  
		// Otherwise, the normalized depth values at z = 1 (NDC) will 
		// fail the depth test if the depth buffer was cleared to 1.
		skyPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		skyPsoDesc.pRootSignature = mRootSignature.Get();
		skyPsoDesc.VS =
		{
			reinterpret_cast<BYTE*>(mShaders["skyVS"]->GetBufferPointer()),
			mShaders["skyVS"]->GetBufferSize()
		};
		skyPsoDesc.PS =
		{
			reinterpret_cast<BYTE*>(mShaders["skyPS"]->GetBufferPointer()),
			mShaders["skyPS"]->GetBufferSize()
		};
		ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&skyPsoDesc, IID_PPV_ARGS(&mPSOs["sky"])));

	}

	void Sandbox::BuildFrameResources()
	{
		for (int i = 0; i < gNumFrameResources; ++i)
		{
			mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
				1, mInstanceCount, (UINT)mMaterials.size()));
		}
	}

	void Sandbox::BuildMaterials()
	{
		auto bricks0 = std::make_unique<Material>();
		bricks0->Name = "bricks0";
		bricks0->MatCBIndex = 0;
		bricks0->DiffuseSrvHeapIndex = 0;
		bricks0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		bricks0->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
		bricks0->Roughness = 0.3f;

		auto tile0 = std::make_unique<Material>();
		tile0->Name = "tile0";
		tile0->MatCBIndex = 1;
		tile0->DiffuseSrvHeapIndex = 1;
		tile0->DiffuseAlbedo = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
		tile0->FresnelR0 = XMFLOAT3(0.2f, 0.2f, 0.2f);
		tile0->Roughness = 0.1f;

		auto mirror0 = std::make_unique<Material>();
		mirror0->Name = "mirror0";
		mirror0->MatCBIndex = 2;
		mirror0->DiffuseSrvHeapIndex = 2;
		mirror0->DiffuseAlbedo = XMFLOAT4(0.0f, 0.0f, 0.1f, 1.0f);
		mirror0->FresnelR0 = XMFLOAT3(0.98f, 0.97f, 0.95f);
		mirror0->Roughness = 0.1f;

		auto skullMat = std::make_unique<Material>();
		skullMat->Name = "skullMat";
		skullMat->MatCBIndex = 3;
		skullMat->DiffuseSrvHeapIndex = 2;
		skullMat->DiffuseAlbedo = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
		skullMat->FresnelR0 = XMFLOAT3(0.2f, 0.2f, 0.2f);
		skullMat->Roughness = 0.2f;

		auto sky = std::make_unique<Material>();
		sky->Name = "sky";
		sky->MatCBIndex = 4;
		sky->DiffuseSrvHeapIndex = 3;
		sky->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		sky->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
		sky->Roughness = 1.0f;

		mMaterials["bricks0"] = std::move(bricks0);
		mMaterials["tile0"] = std::move(tile0);
		mMaterials["mirror0"] = std::move(mirror0);
		mMaterials["skullMat"] = std::move(skullMat);
		mMaterials["sky"] = std::move(sky);
	}

	void Sandbox::BuildMyRenderItems()
	{
		auto myRitem = std::make_unique<RenderItem>();
		myRitem->World = MathHelper::Identity4x4();
		myRitem->TexTransform = MathHelper::Identity4x4();
		myRitem->ObjCBIndex = 0;
		myRitem->Mat = mMaterials["tile0"].get();
		myRitem->Geo = mGeometries["myGeo"].get();
		myRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		myRitem->InstanceCount = 0;
		myRitem->IndexCount = myRitem->Geo->DrawArgs["my"].IndexCount;
		myRitem->StartIndexLocation = myRitem->Geo->DrawArgs["my"].StartIndexLocation;
		myRitem->BaseVertexLocation = myRitem->Geo->DrawArgs["my"].BaseVertexLocation;
		myRitem->Bounds = myRitem->Geo->DrawArgs["my"].Bounds;

		// Generate instance data.
		const int n = 5;
		mInstanceCount = n * n * n;
		myRitem->Instances.resize(mInstanceCount);


		float width = 50.0f;
		float height = 50.0f;
		float depth = 50.0f;

		float x = -0.5f * width;
		float y = -0.5f * height;
		float z = -0.5f * depth;
		float dx = width / (n - 1);
		float dy = height / (n - 1);
		float dz = depth / (n - 1);
		for (int k = 0; k < n; ++k)
		{
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					int index = k * n * n + i * n + j;
					// Position instanced along a 3D grid.
					myRitem->Instances[index].World = XMFLOAT4X4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						x + j * dx, y + i * dy, z + k * dz, 1.0f);

					XMStoreFloat4x4(&myRitem->Instances[index].TexTransform, XMMatrixScaling(2.0f, 2.0f, 1.0f));
					myRitem->Instances[index].MaterialIndex = index % mMaterials.size();
				}
			}
		}


		mAllRitems.push_back(std::move(myRitem));

		// All the render items are opaque.
		for (auto& e : mAllRitems)
			mRitemLayer[(int)RenderLayer::Opaque].push_back(e.get());
	}

	void Sandbox::BuildRenderItems()
	{
		auto skyRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&skyRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
		skyRitem->TexTransform = MathHelper::Identity4x4();
		skyRitem->ObjCBIndex = 0;
		skyRitem->Mat = mMaterials["sky"].get();
		skyRitem->Geo = mGeometries["shapeGeo"].get();
		skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		skyRitem->InstanceCount = 1;
		skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
		skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
		skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
		skyRitem->Instances.resize(1);

		mRitemLayer[(int)RenderLayer::Sky].push_back(skyRitem.get());
		mAllRitems.push_back(std::move(skyRitem));

		auto skullRitem = std::make_unique<RenderItem>();
		skullRitem->World = MathHelper::Identity4x4();
		skullRitem->TexTransform = MathHelper::Identity4x4();
		skullRitem->ObjCBIndex = 0;
		skullRitem->Mat = mMaterials["tile0"].get();
		skullRitem->Geo = mGeometries["skullGeo"].get();
		skullRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		skullRitem->InstanceCount = 0;
		skullRitem->IndexCount = skullRitem->Geo->DrawArgs["skull"].IndexCount;
		skullRitem->StartIndexLocation = skullRitem->Geo->DrawArgs["skull"].StartIndexLocation;
		skullRitem->BaseVertexLocation = skullRitem->Geo->DrawArgs["skull"].BaseVertexLocation;
		skullRitem->Bounds = skullRitem->Geo->DrawArgs["skull"].Bounds;

		// Generate instance data.
		const int n = 5;
		mInstanceCount = n * n * n;
		skullRitem->Instances.resize(mInstanceCount);


		float width = 200.0f;
		float height = 200.0f;
		float depth = 200.0f;

		float x = -0.5f * width;
		float y = -0.5f * height;
		float z = -0.5f * depth;
		float dx = width / (n - 1);
		float dy = height / (n - 1);
		float dz = depth / (n - 1);
		for (int k = 0; k < n; ++k)
		{
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					int index = k * n * n + i * n + j;
					// Position instanced along a 3D grid.
					skullRitem->Instances[index].World = XMFLOAT4X4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						x + j * dx, y + i * dy, z + k * dz, 1.0f);

					XMStoreFloat4x4(&skullRitem->Instances[index].TexTransform, XMMatrixScaling(2.0f, 2.0f, 1.0f));
					skullRitem->Instances[index].MaterialIndex = index % mMaterials.size();
				}
			}
		}

		mRitemLayer[(int)RenderLayer::Opaque].push_back(skullRitem.get());
		mAllRitems.push_back(std::move(skullRitem));

	}

	void Sandbox::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
	{

		auto ri = ritems[0];

		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		// Set the instance buffer to use for this render-item.  For structured buffers, we can bypass 
		// the heap and set as a root descriptor.
		auto instanceBuffer = mCurrFrameResource->InstanceBuffer->Resource();
		mCommandList->SetGraphicsRootShaderResourceView(5, instanceBuffer->GetGPUVirtualAddress());

		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);

		// For each opaque render item...
		for (size_t i = 1; i < ritems.size(); ++i)
		{
			auto ri = ritems[i];

			cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
			cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
			cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

			// Set the instance buffer to use for this render-item.  For structured buffers, we can bypass 
			// the heap and set as a root descriptor.
			auto instanceBuffer = mCurrFrameResource->InstanceBuffer->Resource();
			mCommandList->SetGraphicsRootShaderResourceView(0, instanceBuffer->GetGPUVirtualAddress());

			cmdList->DrawIndexedInstanced(ri->IndexCount, ri->InstanceCount, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
		}
	}

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> Sandbox::GetStaticSamplers()
	{
		// Applications usually only need a handful of samplers.  So just define them all up front
// and keep them available as part of the root signature.  

		const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
			0, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
			1, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
			2, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
			3, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

		const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
			4, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
			0.0f,                             // mipLODBias
			8);                               // maxAnisotropy

		const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
			5, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
			0.0f,                              // mipLODBias
			8);                                // maxAnisotropy

		return {
			pointWrap, pointClamp,
			linearWrap, linearClamp,
			anisotropicWrap, anisotropicClamp };
	}

}


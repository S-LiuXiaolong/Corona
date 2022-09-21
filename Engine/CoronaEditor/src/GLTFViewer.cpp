/*
 *  Copyright 2019-2022 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#include <cmath>
#include <array>

#include "GLTFViewer.hpp"
#include "MapHelper.hpp"
#include "BasicMath.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "CommonlyUsedStates.h"
#include "ShaderMacroHelper.hpp"
#include "FileSystem.hpp"
#include "imgui.h"
#include "imGuIZMO.h"
#include "ImGuiUtils.hpp"
#include "CallbackWrapper.hpp"
#include "CommandLineParser.hpp"

namespace Diligent
{

#include "Shaders/Common/public/BasicStructures.fxh"
#include "Shaders/PostProcess/ToneMapping/public/ToneMappingStructures.fxh"

EngineBase* CreateEngine()
{
    return new GLTFViewer();
}

namespace
{

struct EnvMapRenderAttribs
{
    ToneMappingAttribs TMAttribs;

    float AverageLogLum;
    float MipLevel;
    float Unusued1;
    float Unusued2;
};

} // namespace

// clang-format off
const std::pair<const char*, const char*> GLTFViewer::GLTFModels[] =
{
//     {"Damaged Helmet",      "models/DamagedHelmet/DamagedHelmet.gltf"},
//     {"Metal Rough Spheres", "models/MetalRoughSpheres/MetalRoughSpheres.gltf"},
//     {"Flight Helmet",       "models/FlightHelmet/FlightHelmet.gltf"},
//     {"Cesium Man",          "models/CesiumMan/CesiumMan.gltf"},
//     {"Boom Box",            "models/BoomBoxWithAxes/BoomBoxWithAxes.gltf"},
//     {"Normal Tangent Test", "models/NormalTangentTest/NormalTangentTest.gltf"},
    {"Fox",                 "models/Fox/Fox.gltf"}
};
// clang-format on

void GLTFViewer::LoadModel(const char* Path)
{
    if (m_Model)
    {
        m_PlayAnimation  = false;
        m_AnimationIndex = 0;
        m_AnimationTimers.clear();
    }

    GLTF::Model::CreateInfo ModelCI;
    ModelCI.FileName   = Path;
    ModelCI.pCacheInfo = m_bUseResourceCache ? &m_CacheUseInfo : nullptr;
    m_Model.reset(new GLTF::Model{m_pDevice, m_pImmediateContext, ModelCI});

    m_ModelResourceBindings = m_GLTFRenderer->CreateResourceBindings(*m_Model, m_CameraAttribsCB, m_LightAttribsCB);

    // Center and scale model
    float3 ModelDim{m_Model->AABBTransform[0][0], m_Model->AABBTransform[1][1], m_Model->AABBTransform[2][2]};
    float  Scale     = (1.0f / std::max(std::max(ModelDim.x, ModelDim.y), ModelDim.z)) * 0.5f;
    auto   Translate = -float3(m_Model->AABBTransform[3][0], m_Model->AABBTransform[3][1], m_Model->AABBTransform[3][2]);
    Translate += -0.5f * ModelDim;
    float4x4 InvYAxis = float4x4::Identity();
    InvYAxis._22      = -1;

    auto ModelTransform = float4x4::Translation(Translate) * float4x4::Scale(Scale) * InvYAxis;
    m_Model->Transform(ModelTransform);

    if (!m_Model->Animations.empty())
    {
        m_AnimationTimers.resize(m_Model->Animations.size());
        m_AnimationIndex = 0;
        m_PlayAnimation  = true;
    }

    m_CameraId = 0;
    m_Cameras.clear();
    // TODO: Get skeleton from Node struct.
    for (const auto* node : m_Model->LinearNodes)
    {
        if (node->pCamera && node->pCamera->Type == GLTF::Camera::Projection::Perspective)
            m_Cameras.push_back(node->pCamera.get());
    }

    unsigned int requiredVerts = 0;
    //    std::vector<float3> globalTranslationForAllNodes;
    for (auto& skin : m_Model->Skins)
    {
//         for (auto joint : skin->Joints)
//         {
//             float3 translation = joint->Translation;
// 
//             auto parent = joint->Parent;
//             while (parent != nullptr)
//             {
//                 translation += parent->Translation;
//                 parent = parent->Parent;
//             }
//             // Multiple skins haven't been considered here.
//             translation += Translate;
//             translation *= Scale;
//             translation.y -= 2 * translation.y;
// 
//             globalTranslationForAllNodes.push_back(translation);
//         }
        for (auto joint : skin->Joints)
        {
            auto parent = joint->Parent;
            if (parent != nullptr)
            {
                requiredVerts += 2;
            }
        }
    }

    //    SkeletonVerts.resize(requiredVerts);

    for (auto& skin : m_Model->Skins)
    {
        for (auto joint : skin->Joints)
        {
            float3 translation = joint->Translation;
            Quaternion rotation    = joint->Rotation;
            float3     scale       = joint->Scale;
            float4x4   Transform   = rotation.ToMatrix() * float4x4::Scale(scale) * float4x4::Translation(translation);
        
            auto parent = joint->Parent;
            while (parent != nullptr)
            {
                translation = parent->Translation;
                rotation    = parent->Rotation;
                scale       = parent->Scale;
                Transform *= rotation.ToMatrix() * float4x4::Scale(scale) * float4x4::Translation(translation);
                parent = parent->Parent;
            }

            Transform *= ModelTransform;

            translation = float3(Transform.m30, Transform.m31, Transform.m32);
            SkeletonVerts.push_back(translation);

            auto joint_parent = joint->Parent;
            translation       = joint_parent->Translation;
            rotation          = joint_parent->Rotation;
            scale             = joint_parent->Scale;
            Transform         = rotation.ToMatrix() * float4x4::Scale(scale) * float4x4::Translation(translation);

            auto parent_parent = joint_parent->Parent;
            while (parent_parent != nullptr)
            {
                translation = parent_parent->Translation;
                rotation    = parent_parent->Rotation;
                scale       = parent_parent->Scale;
                Transform *= rotation.ToMatrix() * float4x4::Scale(scale) * float4x4::Translation(translation);
                parent_parent = parent_parent->Parent;
            }
            // Multiple skins haven't been considered here.

            Transform *= ModelTransform;

            translation = float3(Transform.m30, Transform.m31, Transform.m32);
            SkeletonVerts.push_back(translation);
        
        }

    }

    

//    SkeletonVerts = std::move(globalTranslationForAllNodes);


}

void GLTFViewer::GetGlobalTranslation()
{
    std::vector<float3> globalTranslationForAllNodes;

    // Center and scale model
    float3 ModelDim{m_Model->AABBTransform[0][0], m_Model->AABBTransform[1][1], m_Model->AABBTransform[2][2]};
    float  Scale     = (1.0f / std::max(std::max(ModelDim.x, ModelDim.y), ModelDim.z)) * 0.5f;
    auto   Translate = -float3(m_Model->AABBTransform[3][0], m_Model->AABBTransform[3][1], m_Model->AABBTransform[3][2]);
    Translate += -0.5f * ModelDim;
    float4x4 InvYAxis = float4x4::Identity();
    InvYAxis._22      = -1;

    for (auto& skin : m_Model->Skins)
    {
        for (auto joint : skin->Joints)
        {
            float3 translation = joint->Translation;

            auto parent = joint->Parent;
            while (parent != nullptr)
            {
                translation += parent->Translation;
                parent = parent->Parent;
            }
            // Multiple skins haven't been considered here.
            translation += Translate;
            translation *= Scale;

            globalTranslationForAllNodes.push_back(translation);
        }
    }

//    SkeletonVerts = std::move(globalTranslationForAllNodes);
}

// void GLTFViewer::DrawSkeleton()
// {
//     float3* buffer = new float3[SkeletonVerts.size()];
//     if (!SkeletonVerts.empty())
//     {
//         memcpy(buffer, &SkeletonVerts[0], SkeletonVerts.size() * sizeof(float3));
//     }
//     // Create a vertex buffer that stores cube vertices
//     BufferDesc VertBuffDesc;
//     VertBuffDesc.Name      = "Cube vertex buffer";
//     VertBuffDesc.Usage     = USAGE_IMMUTABLE;
//     VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
//     VertBuffDesc.Size      = sizeof(SkeletonVerts);
//     BufferData VBData;
//     VBData.pData    = buffer;
//     VBData.DataSize = sizeof(buffer);
//     m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_SkeletonVertexBuffer);
// }

void GLTFViewer::ResetView()
{
    m_CameraYaw      = 0;
    m_CameraPitch    = 0;
    m_ModelRotation  = Quaternion::RotationFromAxisAngle(float3{0.f, 1.0f, 0.0f}, -PI_F / 2.f);
    m_CameraRotation = Quaternion::RotationFromAxisAngle(float3{0.75f, 0.0f, 0.75f}, PI_F);
}

GLTFViewer::CommandLineStatus GLTFViewer::ProcessCommandLine(int argc, const char* const* argv)
{
    CommandLineParser ArgsParser{argc, argv};
    ArgsParser.Parse("use_cache", m_bUseResourceCache);
    ArgsParser.Parse("model", m_InitialModelPath);

    return CommandLineStatus::OK;
}

void GLTFViewer::CreateGLTFResourceCache()
{
    std::array<BufferSuballocatorCreateInfo, 3> Buffers = {};

    Buffers[0].Desc.Name      = "GLTF basic vertex attribs buffer";
    Buffers[0].Desc.BindFlags = BIND_VERTEX_BUFFER;
    Buffers[0].Desc.Usage     = USAGE_DEFAULT;
    Buffers[0].Desc.Size      = sizeof(GLTF::Model::VertexBasicAttribs) * 16 << 10;

    Buffers[1].Desc.Name      = "GLTF skin attribs buffer";
    Buffers[1].Desc.BindFlags = BIND_VERTEX_BUFFER;
    Buffers[1].Desc.Usage     = USAGE_DEFAULT;
    Buffers[1].Desc.Size      = sizeof(GLTF::Model::VertexSkinAttribs) * 16 << 10;

    Buffers[2].Desc.Name      = "GLTF index buffer";
    Buffers[2].Desc.BindFlags = BIND_INDEX_BUFFER;
    Buffers[2].Desc.Usage     = USAGE_DEFAULT;
    Buffers[2].Desc.Size      = sizeof(Uint32) * 8 << 10;

    std::array<DynamicTextureAtlasCreateInfo, 1> Atlases;
    Atlases[0].Desc.Name      = "GLTF texture atlas";
    Atlases[0].Desc.Type      = RESOURCE_DIM_TEX_2D_ARRAY;
    Atlases[0].Desc.Usage     = USAGE_DEFAULT;
    Atlases[0].Desc.BindFlags = BIND_SHADER_RESOURCE;
    Atlases[0].Desc.Format    = TEX_FORMAT_RGBA8_UNORM;
    Atlases[0].Desc.Width     = 4096;
    Atlases[0].Desc.Height    = 4096;
    Atlases[0].Desc.MipLevels = 6;

    GLTF::ResourceManager::CreateInfo ResourceMgrCI;
    ResourceMgrCI.BuffSuballocators    = Buffers.data();
    ResourceMgrCI.NumBuffSuballocators = static_cast<Uint32>(Buffers.size());
    ResourceMgrCI.TexAtlases           = Atlases.data();
    ResourceMgrCI.NumTexAtlases        = static_cast<Uint32>(Atlases.size());

    ResourceMgrCI.DefaultAtlasDesc.Desc.Type      = RESOURCE_DIM_TEX_2D_ARRAY;
    ResourceMgrCI.DefaultAtlasDesc.Desc.Usage     = USAGE_DEFAULT;
    ResourceMgrCI.DefaultAtlasDesc.Desc.BindFlags = BIND_SHADER_RESOURCE;
    ResourceMgrCI.DefaultAtlasDesc.Desc.Width     = 4096;
    ResourceMgrCI.DefaultAtlasDesc.Desc.Height    = 4096;
    ResourceMgrCI.DefaultAtlasDesc.Desc.MipLevels = 6;

    m_pResourceMgr = GLTF::ResourceManager::Create(m_pDevice, ResourceMgrCI);

    m_CacheUseInfo.pResourceMgr     = m_pResourceMgr;
    m_CacheUseInfo.VertexBuffer0Idx = 0;
    m_CacheUseInfo.VertexBuffer1Idx = 1;
    m_CacheUseInfo.IndexBufferIdx   = 2;

    m_CacheUseInfo.BaseColorFormat    = TEX_FORMAT_RGBA8_UNORM;
    m_CacheUseInfo.PhysicalDescFormat = TEX_FORMAT_RGBA8_UNORM;
    m_CacheUseInfo.NormalFormat       = TEX_FORMAT_RGBA8_UNORM;
    m_CacheUseInfo.OcclusionFormat    = TEX_FORMAT_RGBA8_UNORM;
    m_CacheUseInfo.EmissiveFormat     = TEX_FORMAT_RGBA8_UNORM;
}

void GLTFViewer::Initialize(const EngineInitInfo& InitInfo)
{
    EngineBase::Initialize(InitInfo);

    ResetView();

    RefCntAutoPtr<ITexture> EnvironmentMap;
    CreateTextureFromFile("textures/papermill.ktx", TextureLoadInfo{"Environment map"}, m_pDevice, &EnvironmentMap);
    m_EnvironmentMapSRV = EnvironmentMap->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

    auto BackBufferFmt  = m_pSwapChain->GetDesc().ColorBufferFormat;
    auto DepthBufferFmt = m_pSwapChain->GetDesc().DepthBufferFormat;

    GLTF_PBR_Renderer::CreateInfo RendererCI;
    RendererCI.RTVFmt          = BackBufferFmt;
    RendererCI.DSVFmt          = DepthBufferFmt;
    RendererCI.AllowDebugView  = true;
    RendererCI.UseIBL          = true;
    RendererCI.FrontCCW        = true;
    RendererCI.UseTextureAtlas = m_bUseResourceCache;
    m_GLTFRenderer.reset(new GLTF_PBR_Renderer(m_pDevice, m_pImmediateContext, RendererCI));

    CreateUniformBuffer(m_pDevice, sizeof(CameraAttribs), "Camera attribs buffer", &m_CameraAttribsCB);
    CreateUniformBuffer(m_pDevice, sizeof(LightAttribs), "Light attribs buffer", &m_LightAttribsCB);
    CreateUniformBuffer(m_pDevice, sizeof(EnvMapRenderAttribs), "Env map render attribs buffer", &m_EnvMapRenderAttribsCB);
    // clang-format off
    StateTransitionDesc Barriers [] =
    {
        {m_CameraAttribsCB,        RESOURCE_STATE_UNKNOWN, RESOURCE_STATE_CONSTANT_BUFFER, STATE_TRANSITION_FLAG_UPDATE_STATE},
        {m_LightAttribsCB,         RESOURCE_STATE_UNKNOWN, RESOURCE_STATE_CONSTANT_BUFFER, STATE_TRANSITION_FLAG_UPDATE_STATE},
        {m_EnvMapRenderAttribsCB,  RESOURCE_STATE_UNKNOWN, RESOURCE_STATE_CONSTANT_BUFFER, STATE_TRANSITION_FLAG_UPDATE_STATE},
        {EnvironmentMap,           RESOURCE_STATE_UNKNOWN, RESOURCE_STATE_SHADER_RESOURCE, STATE_TRANSITION_FLAG_UPDATE_STATE}
    };
    // clang-format on
    m_pImmediateContext->TransitionResourceStates(_countof(Barriers), Barriers);

    m_GLTFRenderer->PrecomputeCubemaps(m_pDevice, m_pImmediateContext, m_EnvironmentMapSRV);

    RefCntAutoPtr<IRenderStateNotationParser> pRSNParser;
    {
        RefCntAutoPtr<IShaderSourceInputStreamFactory> pStreamFactory;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory("render_states", &pStreamFactory);

        CreateRenderStateNotationParser({}, &pRSNParser);
        pRSNParser->ParseFile("RenderStates.json", pStreamFactory);
    }

    RefCntAutoPtr<IRenderStateNotationLoader> pRSNLoader;
    {
        RefCntAutoPtr<IShaderSourceInputStreamFactory> pStreamFactory;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory("shaders", &pStreamFactory);
        CreateRenderStateNotationLoader({m_pDevice, pRSNParser, pStreamFactory}, &pRSNLoader);
    }

    CreateEnvMapPSO(pRSNLoader);

    CreateBoundBoxPSO(pRSNLoader);

    CreateSkeletonPSO();

    m_LightDirection = normalize(float3(0.5f, -0.6f, -0.2f));

    if (m_bUseResourceCache)
        CreateGLTFResourceCache();

    LoadModel(!m_InitialModelPath.empty() ? m_InitialModelPath.c_str() : GLTFModels[m_SelectedModel].second);

    GetGlobalTranslation();
    float3 buffer[48];
    if (!SkeletonVerts.empty())
    {
        memcpy(buffer, &SkeletonVerts[0], SkeletonVerts.size() * sizeof(float3));
    }
    // Create a vertex buffer that stores cube vertices
    BufferDesc VertBuffDesc;
    VertBuffDesc.Name      = "Cube vertex buffer";
    VertBuffDesc.Usage     = USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
    VertBuffDesc.Size      = sizeof(buffer);
    BufferData VBData;
    VBData.pData    = buffer;
    VBData.DataSize = sizeof(buffer);
    m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_SkeletonVertexBuffer);
}

void GLTFViewer::UpdateUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        {
            const char* Models[_countof(GLTFModels)];
            for (size_t i = 0; i < _countof(GLTFModels); ++i)
                Models[i] = GLTFModels[i].first;
            if (ImGui::Combo("Model", &m_SelectedModel, Models, _countof(GLTFModels)))
            {
                LoadModel(GLTFModels[m_SelectedModel].second);
                /*GetGlobalTranslation();*/
            }
        }
#ifdef PLATFORM_WIN32
        if (ImGui::Button("Load model"))
        {
            FileDialogAttribs OpenDialogAttribs{FILE_DIALOG_TYPE_OPEN};
            OpenDialogAttribs.Title  = "Select GLTF file";
            OpenDialogAttribs.Filter = "glTF files\0*.gltf;*.glb\0";
            auto FileName            = FileSystem::FileDialog(OpenDialogAttribs);
            if (!FileName.empty())
            {
                LoadModel(FileName.c_str());
                /*GetGlobalTranslation();*/
            }
        }
#endif
        if (!m_Cameras.empty())
        {
            std::vector<std::pair<Uint32, std::string>> CamList;
            CamList.emplace_back(0, "default");
            for (Uint32 i = 0; i < m_Cameras.size(); ++i)
            {
                const auto& Cam = m_Cameras[i];
                CamList.emplace_back(i + 1, Cam->Name.empty() ? std::to_string(i) : Cam->Name);
            }
            ImGui::Combo("Camera", &m_CameraId, CamList.data(), static_cast<int>(CamList.size()));
        }

        if (m_CameraId == 0)
        {
            ImGui::gizmo3D("Model Rotation", m_ModelRotation, ImGui::GetTextLineHeight() * 10);
            ImGui::SameLine();
            ImGui::gizmo3D("Light direction", m_LightDirection, ImGui::GetTextLineHeight() * 10);

            if (ImGui::Button("Reset view"))
            {
                ResetView();
            }

            ImGui::SliderFloat("Camera distance", &m_CameraDist, 0.1f, 5.0f);
        }

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("Lighting"))
        {
            ImGui::ColorEdit3("Light Color", &m_LightColor.r);
            // clang-format off
            ImGui::SliderFloat("Light Intensity",    &m_LightIntensity,                 0.f, 50.f);
            ImGui::SliderFloat("Occlusion strength", &m_RenderParams.OcclusionStrength, 0.f,  1.f);
            ImGui::SliderFloat("Emission scale",     &m_RenderParams.EmissionScale,     0.f,  1.f);
            ImGui::SliderFloat("IBL scale",          &m_RenderParams.IBLScale,          0.f,  1.f);
            // clang-format on
            ImGui::TreePop();
        }

        if (!m_Model->Animations.empty())
        {
            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
            if (ImGui::TreeNode("Animation"))
            {
                ImGui::Checkbox("Play", &m_PlayAnimation);
                std::vector<const char*> Animations(m_Model->Animations.size());
                for (size_t i = 0; i < m_Model->Animations.size(); ++i)
                    Animations[i] = m_Model->Animations[i].Name.c_str();
                ImGui::Combo("Active Animation", reinterpret_cast<int*>(&m_AnimationIndex), Animations.data(), static_cast<int>(Animations.size()));
                ImGui::TreePop();
            }
            /*GetGlobalTranslation();*/
        }

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::TreeNode("Tone mapping"))
        {
            // clang-format off
            ImGui::SliderFloat("Average log lum",    &m_RenderParams.AverageLogLum,     0.01f, 10.0f);
            ImGui::SliderFloat("Middle gray",        &m_RenderParams.MiddleGray,        0.01f,  1.0f);
            ImGui::SliderFloat("White point",        &m_RenderParams.WhitePoint,        0.1f,  20.0f);
            // clang-format on
            ImGui::TreePop();
        }

        {
            std::array<const char*, static_cast<size_t>(BackgroundMode::NumModes)> BackgroundModes;
            BackgroundModes[static_cast<size_t>(BackgroundMode::None)]              = "None";
            BackgroundModes[static_cast<size_t>(BackgroundMode::EnvironmentMap)]    = "Environmen Map";
            BackgroundModes[static_cast<size_t>(BackgroundMode::Irradiance)]        = "Irradiance";
            BackgroundModes[static_cast<size_t>(BackgroundMode::PrefilteredEnvMap)] = "PrefilteredEnvMap";
            if (ImGui::Combo("Background mode", reinterpret_cast<int*>(&m_BackgroundMode), BackgroundModes.data(), static_cast<int>(BackgroundModes.size())))
            {
                CreateEnvMapSRB();
            }
        }

        ImGui::SliderFloat("Env map mip", &m_EnvMapMipLevel, 0.0f, 7.0f);

        {
            std::array<const char*, static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::NumDebugViews)> DebugViews;

            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::None)]            = "None";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::BaseColor)]       = "Base Color";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::Transparency)]    = "Transparency";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::NormalMap)]       = "Normal Map";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::Occlusion)]       = "Occlusion";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::Emissive)]        = "Emissive";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::Metallic)]        = "Metallic";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::Roughness)]       = "Roughness";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::DiffuseColor)]    = "Diffuse color";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::SpecularColor)]   = "Specular color (R0)";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::Reflectance90)]   = "Reflectance90";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::MeshNormal)]      = "Mesh normal";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::PerturbedNormal)] = "Perturbed normal";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::NdotV)]           = "n*v";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::DiffuseIBL)]      = "Diffuse IBL";
            DebugViews[static_cast<size_t>(GLTF_PBR_Renderer::RenderInfo::DebugViewType::SpecularIBL)]     = "Specular IBL";
            ImGui::Combo("Debug view", reinterpret_cast<int*>(&m_RenderParams.DebugView), DebugViews.data(), static_cast<int>(DebugViews.size()));
        }

        ImGui::Combo("Bound box mode", reinterpret_cast<int*>(&m_BoundBoxMode),
                     "None\0"
                     "Local\0"
                     "Global\0\0");
    }
    ImGui::End();
}

void GLTFViewer::CreateEnvMapPSO(IRenderStateNotationLoader* pRSNLoader)
{
    auto ModifyCI = MakeCallback([this](PipelineStateCreateInfo& PipelineCI) {
        auto& GraphicsPipelineCI{static_cast<GraphicsPipelineStateCreateInfo&>(PipelineCI)};
        GraphicsPipelineCI.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        GraphicsPipelineCI.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        GraphicsPipelineCI.GraphicsPipeline.NumRenderTargets = 1;
    });

    pRSNLoader->LoadPipelineState({"EnvMap PSO", PIPELINE_TYPE_GRAPHICS, true, ModifyCI, ModifyCI}, &m_EnvMapPSO);

    m_EnvMapPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "cbCameraAttribs")->Set(m_CameraAttribsCB);
    m_EnvMapPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "cbEnvMapRenderAttribs")->Set(m_EnvMapRenderAttribsCB);
    CreateEnvMapSRB();
}

void GLTFViewer::CreateEnvMapSRB()
{
    if (m_BackgroundMode != BackgroundMode::None)
    {
        m_EnvMapSRB.Release();
        m_EnvMapPSO->CreateShaderResourceBinding(&m_EnvMapSRB, true);
        ITextureView* pEnvMapSRV = nullptr;
        switch (m_BackgroundMode)
        {
            case BackgroundMode::EnvironmentMap:
                pEnvMapSRV = m_EnvironmentMapSRV;
                break;

            case BackgroundMode::Irradiance:
                pEnvMapSRV = m_GLTFRenderer->GetIrradianceCubeSRV();
                break;

            case BackgroundMode::PrefilteredEnvMap:
                pEnvMapSRV = m_GLTFRenderer->GetPrefilteredEnvMapSRV();
                break;

            default:
                UNEXPECTED("Unexpected background mode");
        }
        m_EnvMapSRB->GetVariableByName(SHADER_TYPE_PIXEL, "EnvMap")->Set(pEnvMapSRV);
    }
}

void GLTFViewer::CreateBoundBoxPSO(IRenderStateNotationLoader* pRSNLoader)
{
    auto ModifyCI = MakeCallback([this](PipelineStateCreateInfo& PipelineCI) {
        auto& GraphicsPipelineCI{static_cast<GraphicsPipelineStateCreateInfo&>(PipelineCI)};
        GraphicsPipelineCI.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        GraphicsPipelineCI.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        GraphicsPipelineCI.GraphicsPipeline.NumRenderTargets = 1;
    });
    pRSNLoader->LoadPipelineState({"BoundBox PSO", PIPELINE_TYPE_GRAPHICS, true, ModifyCI, ModifyCI}, &m_BoundBoxPSO);

    m_BoundBoxPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "cbCameraAttribs")->Set(m_CameraAttribsCB);
    m_BoundBoxPSO->CreateShaderResourceBinding(&m_BoundBoxSRB, true);
}

void GLTFViewer::CreateSkeletonPSO()
{
    // Pipeline state object encompasses configuration of all GPU stages

    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Skeleton PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]                = m_pSwapChain->GetDesc().ColorBufferFormat;
    // Use the depth buffer format from the swap chain
    PSOCreateInfo.GraphicsPipeline.DSVFormat                    = m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_LINE_LIST;
    // No back face culling for this tutorial
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;
    // Disable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // In this tutorial, we will load shaders from file. To be able to do that,
    // we need to create a shader source stream factory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Default VS";
        ShaderCI.FilePath        = "F:/work_space/Corona/Engine/CoronaEditor/assets/shaders/default.vsh";
        m_pDevice->CreateShader(ShaderCI, &pVS);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Default PS";
        ShaderCI.FilePath        = "F:/work_space/Corona/Engine/CoronaEditor/assets/shaders/default.psh";
        m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False}
    };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(LayoutElems);

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_SkeletonPSO);
    
    m_SkeletonPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "cbCameraAttribs")->Set(m_CameraAttribsCB);
    m_SkeletonPSO->CreateShaderResourceBinding(&m_SkeletonSRB, true);
}

GLTFViewer::~GLTFViewer()
{
}

// Render a frame
void GLTFViewer::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    // Clear the back buffer
    const float ClearColor[] = {0.032f, 0.032f, 0.032f, 1.0f};
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    float YFov  = PI_F / 4.0f;
    float ZNear = 0.1f;
    float ZFar  = 100.f;

    float4x4 CameraView;
    if (m_CameraId == 0)
    {
        CameraView = m_CameraRotation.ToMatrix() * float4x4::Translation(0.f, 0.0f, m_CameraDist);

        m_RenderParams.ModelTransform = m_ModelRotation.ToMatrix();
    }
    else
    {
        const auto* pCamera = m_Cameras[m_CameraId - 1];

        // GLTF camera is defined such that the local +X axis is to the right,
        // the lens looks towards the local -Z axis, and the top of the camera
        // is aligned with the local +Y axis.
        // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#cameras
        // We need to inverse the Z axis as our camera looks towards +Z.
        float4x4 InvZAxis = float4x4::Identity();
        InvZAxis._33      = -1;

        CameraView = pCamera->matrix.Inverse() * InvZAxis;
        YFov       = pCamera->Perspective.YFov;
        ZNear      = pCamera->Perspective.ZNear;
        ZFar       = pCamera->Perspective.ZFar;

        m_RenderParams.ModelTransform = float4x4::Identity();
    }

    // Apply pretransform matrix that rotates the scene according the surface orientation
    CameraView *= GetSurfacePretransformMatrix(float3{0, 0, 1});

    float4x4 CameraWorld = CameraView.Inverse();

    // Get projection matrix adjusted to the current screen orientation
    const auto CameraProj     = GetAdjustedProjectionMatrix(YFov, ZNear, ZFar);
    const auto CameraViewProj = CameraView * CameraProj;

    float3 CameraWorldPos = float3::MakeVector(CameraWorld[3]);

    {
        MapHelper<CameraAttribs> CamAttribs(m_pImmediateContext, m_CameraAttribsCB, MAP_WRITE, MAP_FLAG_DISCARD);
        CamAttribs->mProjT        = CameraProj.Transpose();
        CamAttribs->mViewProjT    = CameraViewProj.Transpose();
        CamAttribs->mViewProjInvT = CameraViewProj.Inverse().Transpose();
        CamAttribs->f4Position    = float4(CameraWorldPos, 1);

        if (m_BoundBoxMode != BoundBoxMode::None)
        {
            float4x4 BBTransform;
            if (m_BoundBoxMode == BoundBoxMode::Local)
            {
                BBTransform = m_Model->AABBTransform * m_RenderParams.ModelTransform;
            }
            else if (m_BoundBoxMode == BoundBoxMode::Global)
            {
                auto TransformedBB = BoundBox{m_Model->dimensions.min, m_Model->dimensions.max}.Transform(m_RenderParams.ModelTransform);
                BBTransform        = float4x4::Scale(TransformedBB.Max - TransformedBB.Min) * float4x4::Translation(TransformedBB.Min);
            }
            else
            {
                UNEXPECTED("Unexpected bound box mode");
            }

            for (int row = 0; row < 4; ++row)
                CamAttribs->f4ExtraData[row] = float4::MakeVector(BBTransform[row]);
        }
    }

    {
        MapHelper<LightAttribs> lightAttribs(m_pImmediateContext, m_LightAttribsCB, MAP_WRITE, MAP_FLAG_DISCARD);
        lightAttribs->f4Direction = m_LightDirection;
        lightAttribs->f4Intensity = m_LightColor * m_LightIntensity;
    }

    if (m_bUseResourceCache)
    {
        m_GLTFRenderer->Begin(m_pDevice, m_pImmediateContext, m_CacheUseInfo, m_CacheBindings, m_CameraAttribsCB, m_LightAttribsCB);
        m_GLTFRenderer->Render(m_pImmediateContext, *m_Model, m_RenderParams, nullptr, &m_CacheBindings);
    }
    else
    {
        m_GLTFRenderer->Begin(m_pImmediateContext);
        m_GLTFRenderer->Render(m_pImmediateContext, *m_Model, m_RenderParams, &m_ModelResourceBindings);
    }


    if (m_BoundBoxMode != BoundBoxMode::None)
    {
        m_pImmediateContext->SetPipelineState(m_BoundBoxPSO);
        m_pImmediateContext->CommitShaderResources(m_BoundBoxSRB, RESOURCE_STATE_TRANSITION_MODE_VERIFY);
        DrawAttribs DrawAttrs{24, DRAW_FLAG_VERIFY_ALL};
        m_pImmediateContext->Draw(DrawAttrs);
    }

    if (m_BackgroundMode != BackgroundMode::None)
    {
        {
            MapHelper<EnvMapRenderAttribs> EnvMapAttribs(m_pImmediateContext, m_EnvMapRenderAttribsCB, MAP_WRITE, MAP_FLAG_DISCARD);
            EnvMapAttribs->TMAttribs.iToneMappingMode     = TONE_MAPPING_MODE_UNCHARTED2;
            EnvMapAttribs->TMAttribs.bAutoExposure        = 0;
            EnvMapAttribs->TMAttribs.fMiddleGray          = m_RenderParams.MiddleGray;
            EnvMapAttribs->TMAttribs.bLightAdaptation     = 0;
            EnvMapAttribs->TMAttribs.fWhitePoint          = m_RenderParams.WhitePoint;
            EnvMapAttribs->TMAttribs.fLuminanceSaturation = 1.0;
            EnvMapAttribs->AverageLogLum                  = m_RenderParams.AverageLogLum;
            EnvMapAttribs->MipLevel                       = m_EnvMapMipLevel;
        }
        m_pImmediateContext->SetPipelineState(m_EnvMapPSO);
        m_pImmediateContext->CommitShaderResources(m_EnvMapSRB, RESOURCE_STATE_TRANSITION_MODE_VERIFY);
        DrawAttribs drawAttribs(3, DRAW_FLAG_VERIFY_ALL);
        m_pImmediateContext->Draw(drawAttribs);
    }

    if (!m_Model->Animations.empty())
    {
        // Bind vertex
        const Uint64 offset   = 0;
        IBuffer*     pBuffs[] = {m_SkeletonVertexBuffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

        // Set the pipeline state
        m_pImmediateContext->SetPipelineState(m_SkeletonPSO);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_SkeletonSRB, RESOURCE_STATE_TRANSITION_MODE_VERIFY);

        DrawAttribs drawAttrs;
        drawAttrs.NumVertices = (Uint32)SkeletonVerts.size();
        m_pImmediateContext->Draw(drawAttrs);
    }

}


void GLTFViewer::Update(double CurrTime, double ElapsedTime)
{
    if (m_CameraId == 0)
    {
        const auto& mouseState = m_InputController.GetMouseState();

        float MouseDeltaX = 0;
        float MouseDeltaY = 0;
        if (m_LastMouseState.PosX >= 0 && m_LastMouseState.PosY >= 0 &&
            m_LastMouseState.ButtonFlags != MouseState::BUTTON_FLAG_NONE)
        {
            MouseDeltaX = mouseState.PosX - m_LastMouseState.PosX;
            MouseDeltaY = mouseState.PosY - m_LastMouseState.PosY;
        }
        m_LastMouseState = mouseState;

        constexpr float RotationSpeed = 0.005f;

        float fYawDelta   = MouseDeltaX * RotationSpeed;
        float fPitchDelta = MouseDeltaY * RotationSpeed;
        if (mouseState.ButtonFlags & MouseState::BUTTON_FLAG_LEFT)
        {
            m_CameraYaw += fYawDelta;
            m_CameraPitch += fPitchDelta;
            m_CameraPitch = std::max(m_CameraPitch, -PI_F / 2.f);
            m_CameraPitch = std::min(m_CameraPitch, +PI_F / 2.f);
        }

        // Apply extra rotations to adjust the view to match Khronos GLTF viewer
        m_CameraRotation =
            Quaternion::RotationFromAxisAngle(float3{1, 0, 0}, -m_CameraPitch) *
            Quaternion::RotationFromAxisAngle(float3{0, 1, 0}, -m_CameraYaw) *
            Quaternion::RotationFromAxisAngle(float3{0.75f, 0.0f, 0.75f}, PI_F);

        if (mouseState.ButtonFlags & MouseState::BUTTON_FLAG_RIGHT)
        {
            auto CameraView  = m_CameraRotation.ToMatrix();
            auto CameraWorld = CameraView.Transpose();

            float3 CameraRight = float3::MakeVector(CameraWorld[0]);
            float3 CameraUp    = float3::MakeVector(CameraWorld[1]);
            m_ModelRotation =
                Quaternion::RotationFromAxisAngle(CameraRight, -fPitchDelta) *
                Quaternion::RotationFromAxisAngle(CameraUp, -fYawDelta) *
                m_ModelRotation;
        }

        m_CameraDist -= mouseState.WheelDelta * 0.25f;
        m_CameraDist = clamp(m_CameraDist, 0.1f, 5.f);

        if ((m_InputController.GetKeyState(InputKeys::Reset) & INPUT_KEY_STATE_FLAG_KEY_IS_DOWN) != 0)
            ResetView();
    }

    EngineBase::Update(CurrTime, ElapsedTime);
    UpdateUI();

    if (!m_Model->Animations.empty() && m_PlayAnimation)
    {
        float& AnimationTimer = m_AnimationTimers[m_AnimationIndex];
        AnimationTimer += static_cast<float>(ElapsedTime);
        AnimationTimer = std::fmod(AnimationTimer, m_Model->Animations[m_AnimationIndex].End);
        m_Model->UpdateAnimation(m_AnimationIndex, AnimationTimer);

        GetGlobalTranslation();
    }
}

} // namespace Diligent

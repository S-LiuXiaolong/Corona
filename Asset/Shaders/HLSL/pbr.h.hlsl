#ifndef __STDCBUFFER_H__
#define __STDCBUFFER_H__

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

// static const uint NumLights = 3;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04;

struct a2v_pbr
{
	float3 Position		: POSITION;
	float3 Normal		: NORMAL;
	float2 TextureUV	: TEXCOORD;
	float3 Tangent		: TANGENT;
};

struct Light
{
	float4   m_lightPosition;
	float4   m_lightColor;
};

struct Material
{
	float4 DiffuseAlbedo;
    float3 FresnelR0;
    float Shininess;
};

SamplerState samp0 : register(s0);
Texture2D colorMap : register(t0);
Texture2D physicsDescriptorMap: register(t1);
Texture2D normalMap : register(t2);
Texture2D AOMap : register(t3);
Texture2D emissiveMap : register(t4);

// ? it seems 4 float4x4 exceed the maximum of a constant buffer (i don't know)
cbuffer PerFrameConstants : register(b0)
{
    float4x4 m_worldMatrix;
	// float4x4 m_viewMatrix;
	// float4x4 m_projectionMatrix;
	float4x4 m_worldViewMatrix;
	float4x4 m_worldViewProjectionMatrix; // this 5 matrix cannot live together for the size limit

	// try multi-lights
	// Light    m_lights[MaxLights];
	float4   m_lightPosition;
	float4   m_lightColor;
	float4   m_cameraPosition;
};

cbuffer PerBatchConstants : register(b1)
{
	float4x4 m_objectMatrix;
};

#endif // !__STDCBUFFER_H__

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

#ifndef __VSOUTPUT_H__
#define __VSOUTPUT_H__

struct pbr_vert_output
{
    float4 Position      : SV_POSITION;
	float4 WorldPosition : POSITION;
    float2 TextureUV     : TEXCOORD0;
	float3 vNorm		 : TEXCOORD1;
	float3 vLightDir	 : TEXCOORD2;
};

#endif // !__VSOUTPUT_H__

// ----------------------------------------------------------------------------
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
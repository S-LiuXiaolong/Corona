#ifndef __STDCBUFFER_H__
#define __STDCBUFFER_H__

#define MaxLights 16

struct a2v_default
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
Texture2D emissivemap : register(t4);

// ? it seems 4 float4x4 exceed the maximum of a constant buffer (i don't know)
cbuffer PerFrameConstants : register(b0)
{
    float4x4 m_worldMatrix;
	// float4x4 m_viewMatrix;
	// float4x4 m_projectionMatrix;
	float4x4 m_worldViewMatrix;
	float4x4 m_worldViewProjectionMatrix; // this 5 matrix cannot live together

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

#ifndef __VSOUTPUT_H__
#define __VSOUTPUT_H__

struct default_vert_output
{
    float4 Position      : SV_POSITION;
	float4 WorldPosition : POSITION;
    float2 TextureUV     : TEXCOORD0;
	float3 vNorm		 : TEXCOORD1;
	float3 vLightDir	 : TEXCOORD2;
};

#endif // !__VSOUTPUT_H__


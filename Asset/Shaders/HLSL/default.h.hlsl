#ifndef __STDCBUFFER_H__
#define __STDCBUFFER_H__

struct a2v_default
{
	float3 Position		: POSITION;
	float3 Normal		: NORMAL;
	float2 TextureUV	: TEXCOORD;
    float2 MaterialUV   : MATCOORD;
};

cbuffer PerFrameConstants : register(b0)
{
    float4x4 m_worldMatrix;
	float4x4 m_viewMatrix;
	float4x4 m_projectionMatrix;
	float4   m_lightPosition;
	float4   m_lightColor;
};

cbuffer PerBatchConstants : register(b1)
{
    float3 ambientColor;
    float3 diffuseColor;
    float3 specularColor;
    float specularPower;
};

#endif // !__STDCBUFFER_H__

#ifndef __VSOUTPUT_H__
#define __VSOUTPUT_H__

struct default_vert_output
{
    float4 Position     : SV_POSITION;
    //float2 TextureUV    : TEXCOORD0;
	float3 vNorm		: TEXCOORD2;
	float3 vPosInView	: TEXCOORD3;
};

#endif // !__VSOUTPUT_H__


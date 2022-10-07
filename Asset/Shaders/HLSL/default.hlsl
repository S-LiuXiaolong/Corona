//#include "default.h.hlsl"

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



default_vert_output VS(a2v_default input)
{
    default_vert_output output;

	output.Position = mul(mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix), m_projectionMatrix);
	float3 vN = (mul(mul(float4(input.Normal, 0.0f), m_worldMatrix), m_viewMatrix)).xyz;
	output.vPosInView = (mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix)).xyz;

	output.vNorm = vN;

	//output.TextureUV = input.TextureUV;

	return output;
}

float4 PS(default_vert_output _entryPointOutput) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

#ifndef __STDCBUFFER_H__
#define __STDCBUFFER_H__

struct a2v_debug
{
	float3 Position		: POSITION;
    float3 Color        : COLOR;
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
	float4x4 m_worldViewProjectionMatrix; // this 5 matrix cannot live together
};

cbuffer PerBatchConstants : register(b1)
{
	float4x4 m_objectMatrix;
};

#endif // !__STDCBUFFER_H__

#ifndef __VSOUTPUT_H__
#define __VSOUTPUT_H__

struct debug_vert_output
{
    float4 Position      : SV_POSITION;
    float3 Color         : COLOR;
	float2 TextureUV     : TEXCOORD0;
};

#endif // !__VSOUTPUT_H__


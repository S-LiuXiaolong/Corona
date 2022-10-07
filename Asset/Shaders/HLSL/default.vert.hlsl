#include "default.h.hlsl"

default_vert_output default_vert_main(a2v_default input)
{
    default_vert_output output;

	output.Position = mul(mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix), m_projectionMatrix);
	float3 vN = (mul(mul(float4(input.Normal, 0.0f), m_worldMatrix), m_viewMatrix)).xyz;
	output.vPosInView = (mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix)).xyz;

	output.vNorm = vN;

	//output.TextureUV = input.TextureUV;

	return output;
}

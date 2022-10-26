#include "Common.h.hlsl"

debug_vert_output debug_vert_main(a2v_debug input)
{
    debug_vert_output output;

	// output.Position = mul(mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix), m_projectionMatrix);
	// float3 vN = (mul(mul(float4(input.Normal, 0.0f), m_worldMatrix), m_viewMatrix)).xyz;
	// output.vPosInView = (mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix)).xyz;

	output.Position = mul(float4(input.Position, 1.0f), m_worldViewProjectionMatrix);
	// output.Position = float4(input.Position, 1.0f);
	output.Color = input.Color;
	output.TextureUV = input.Position.xy;

	return output;
}

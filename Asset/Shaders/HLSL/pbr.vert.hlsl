#include "Common.h.hlsl"

pbr_vert_output pbr_vert_main(a2v_pbr input)
{
    pbr_vert_output output;

	// output.Position = mul(mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix), m_projectionMatrix);
	// float3 vN = (mul(mul(float4(input.Normal, 0.0f), m_worldMatrix), m_viewMatrix)).xyz;
	// output.vPosInView = (mul(mul(float4(input.Position.xyz, 1.0f), m_worldMatrix), m_viewMatrix)).xyz;

	float4 temp = mul(float4(input.Position, 1.0f), m_objectMatrix);
	output.WorldPosition = mul(temp, m_worldMatrix);
	output.Position = mul(temp, m_worldViewProjectionMatrix);
	float3 vN = mul((mul(float4(input.Normal, 0.0f), m_objectMatrix)), m_worldMatrix).xyz;
	float3 vT = mul((mul(float4(input.Tangent, 0.0f), m_objectMatrix)), m_worldMatrix).xyz;
	// output.vPosInView = (mul(float4(input.Position.xyz, 1.0f), m_worldViewMatrix)).xyz;

	output.vNorm = normalize(vN);
	output.vTangent = normalize(vT);

	output.TextureUV = input.TextureUV;

	return output;
}

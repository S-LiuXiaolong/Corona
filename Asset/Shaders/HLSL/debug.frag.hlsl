#include "debug.h.hlsl"

float4 debug_frag_main(debug_vert_output input) : SV_Target
{
	float3 albedo = pow(colorMap.Sample(samp0, input.TextureUV).rgb, 2.2);
	float metallic = physicsDescriptorMap.Sample(samp0, input.TextureUV).b;
	float roughness = physicsDescriptorMap.Sample(samp0, input.TextureUV).g;
    float3 normal = normalMap.Sample(samp0, input.TextureUV).rgb;
    float3 ao = AOMap.Sample(samp0, input.TextureUV).r;
    return float4(input.Color, 1.0f);
}
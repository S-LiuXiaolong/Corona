#include "Common.h.hlsl"
#include "LightingUtil.h.hlsl"

float4 pbr_frag_main(pbr_vert_output input) : SV_Target
{
    // used for DamagedHelmet (pbr metallic workflow)
	// float3 albedo = pow(colorMap.Sample(samp0, input.TextureUV).rgb, 2.2);
	// float metallic = physicsDescriptorMap.Sample(samp0, input.TextureUV).b;
	// float roughness = physicsDescriptorMap.Sample(samp0, input.TextureUV).g;
    // float3 normal = normalMap.Sample(samp0, input.TextureUV).rgb;
    // float3 ao = AOMap.Sample(samp0, input.TextureUV).rgb;

    // used for ABeautifulGame
	float3 albedo = pow(colorMap.Sample(samp0, input.TextureUV).rgb, 2.2);
	float metallic = physicsDescriptorMap.Sample(samp0, input.TextureUV).g;
	float roughness = physicsDescriptorMap.Sample(samp0, input.TextureUV).b;
    float3 normal = normalMap.Sample(samp0, input.TextureUV).rgb;
    float3 ao = AOMap.Sample(samp0, input.TextureUV).r;

	// Outgoing light direction (vector from world-space fragment position to the "camera").
	float3 V = normalize((m_cameraPosition - input.WorldPosition).xyz);
    // TODO: wrong here
    // float3 V = normalize((-m_cameraPosition + input.WorldPosition).xyz);

	// Get current fragment's normal and transform to world space (no normal map support yet).
	// float3 N = input.vNorm;
    float3 N = NormalSampleToWorldSpace(normal, input.vNorm, input.vTangent);

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	float3 F0 = lerp(Fdielectric, albedo, metallic);

	// Direct lighting calculation for analytical lights.
	float3 Lo = 0.0;
	for(uint i = 0; i < NumLights; ++i)
	{
        Light light = m_lights[i];
        float3 lightDir = normalize(input.WorldPosition.xyz - light.m_lightPosition);
		float3 L = -lightDir;
        
		// Half-vector between Li and Lo.
		float3 H = normalize(V + L);

        float distance = length(light.m_lightPosition - input.WorldPosition.xyz);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = light.m_lightColor * attenuation * 10;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        // float G   = GeometrySmith(N, V, L, roughness);      
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float temp = N * 2;
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float G = ggx1 * ggx2;
        float3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        float3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        float3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        float3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        float3 kD = 1.0 - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        // float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    float3 ambient = 0.03 * albedo * ao;
    float3 color = Lo + ambient;
    // HDR tonemapping
    color = color / (color + 1.0);
    // gamma correct
    color = pow(color, 1.0/2.2); 

    return float4(color + emissiveMap.Sample(samp0, input.TextureUV).rgb, 1.0);
    // return float4(N, 1.0) + float4(color + emissiveMap.Sample(samp0, input.TextureUV).rgb, 1.0);
}

#include "default.h.hlsl"
#include "LightingUtil.h.hlsl"

float4 default_frag_main(default_vert_output input) : SV_Target
{
    // 颜色对应三个坐标轴：朝上为绿色，朝右为蓝色，朝屏幕里面为蓝色（所以只有红绿两色没有蓝色）
    // return float4(input.vNorm, 1.0f);
    // return float4(-1.0f, -1.0f, 0.0f, 1.0f);
    
    // https://brooknovak.wordpress.com/2008/11/13/hlsl-per-pixel-point-light-using-phong-blinn-lighting-model/
    // Phong relfection is ambient + light-diffuse + spec highlights.
    // I = Ia*ka*Oda + fatt*Ip[kd*Od(N.L) + ks(R.V)^n]
    // Ref: http://www.whisqu.se/per/docs/graphics8.htm
    // and http://en.wikipedia.org/wiki/Phong_shading

    // Get light direction for this fragment
    float3 lightDir = normalize((m_lightPosition - input.WorldPosition).xyz);

    // Note: Non-uniform scaling not supported
    float diffuseLighting = saturate(dot(input.vNorm, -lightDir)); // per pixel diffuse lighting

    // Introduce fall-off of light intensity
    diffuseLighting *= ((length(lightDir) * length(lightDir)) / dot(m_lightPosition - input.WorldPosition, m_lightPosition - input.WorldPosition));

    // Using Blinn half angle modification for performance over correctness
    float3 h = normalize(normalize((m_cameraPosition - input.WorldPosition).xyz) - lightDir);
    float specLighting = pow(saturate(dot(h, input.vNorm)), 2.0f);

    return saturate(0.3f + (0.2f * diffuseLighting * 0.6f) + (specLighting * 0.5f));
}

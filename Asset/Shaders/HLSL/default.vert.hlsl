#include "vsoutput.h.hlsl"

struct a2v_default {
    float3 inputPosition : POSITION;
    float2 inputUV : TEXCOORD;
};

cbuffer PerFrameConstants : register(b0) {
    row_major float4x4 modelMatrix;        // 64 bytes
    row_major float4x4 viewMatrix;        // 64 bytes
    row_major float4x4 projectionMatrix;  // 64 bytes
};                                // totle 192 bytes

default_vert_output default_vert_main(a2v_default a)
{
    default_vert_output o;

    o.pos = mul(mul(mul(float4(a.inputPosition, 1.0f), modelMatrix), viewMatrix), projectionMatrix);
    o.uv = a.inputUV;

    return o;
}

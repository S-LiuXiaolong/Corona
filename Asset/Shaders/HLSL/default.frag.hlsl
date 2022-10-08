#include "default.h.hlsl"

float4 default_frag_main(default_vert_output _entryPointOutput) : SV_Target
{
    return float4(_entryPointOutput.vNorm, 1.0f);
}

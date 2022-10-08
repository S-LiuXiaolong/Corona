#include "default.h.hlsl"

float4 default_frag_main(default_vert_output _entryPointOutput) : SV_Target
{
    // 颜色对应三个坐标轴：朝上为绿色，朝右为蓝色，朝屏幕里面为蓝色（所以只有红绿两色没有蓝色）
    return float4(_entryPointOutput.vNorm, 1.0f);
    // return float4(-1.0f, -1.0f, 0.0f, 1.0f);
}

// TODO
struct CameraAttribs
{
    float4 f4Position; // Camera world position
    float4 f4ViewportSize; // (width, height, 1/width, 1/height)

    float2 f2ViewportOrigin; // (min x, min y)
    float fNearPlaneZ;
    float fFarPlaneZ; // fNearPlaneZ < fFarPlaneZ

#ifdef __cplusplus
    float4x4 mViewT;
    float4x4 mProjT;
    float4x4 mViewProjT;
    float4x4 mViewInvT;
    float4x4 mProjInvT;
    float4x4 mViewProjInvT;
#else
    matrix mView;
    matrix mProj;
    matrix mViewProj;
    matrix mViewInv;
    matrix mProjInv;
    matrix mViewProjInv;
#endif

    float4 f4ExtraData[5]; // Any appliation-specific data
    // Sizeof(CameraAttribs) == 256*2
};

cbuffer cbCameraAttribs
{
    CameraAttribs g_CameraAttribs;
}

// Vertex shader takes two inputs: vertex position and color.
// By convention, Diligent Engine expects vertex shader inputs to be 
// labeled 'ATTRIBn', where n is the attribute number.
struct VSInput
{
    float3 Pos : ATTRIB0;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
};

// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be identical.
void main(in VSInput VSIn,
          out PSInput PSIn)
{
    PSIn.Pos = mul(float4(VSIn.Pos, 1.0), g_CameraAttribs.mViewProj);
    //PSIn.Pos = float4(VSIn.Pos, 1.0);
}

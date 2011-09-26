// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D diffuseTexture : register(t0);
SamplerState linearSampler : register(s0);

//--------------------------------------------------------------------------------------
struct VertextShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 textureUV : TEXCOORD0;
};
struct PixelShaderOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 textureUV : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelShaderOutput main(VertextShaderInput input)
{
    PixelShaderOutput output = (PixelShaderOutput)0;

    output.position = float4(input.position,1.0);
    output.textureUV = input.textureUV;

    // compute view space normal
    output.normal = input.normal;

    return output;
}
//--------------------------------------------------------------------------------------

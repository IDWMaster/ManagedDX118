// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------

Texture2D diffuseTexture : register(t0);
SamplerState linearSampler : register(s0);
//The constant buffer on the GPU at register B0 (buffer 0)
cbuffer Matrices : register( b0 )
{
    matrix model;
    matrix view;
    matrix projection;
};
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
	//Model, view projection
	float4 pos = float4(input.position,1.0);
	pos = mul(pos,model);
	pos = mul(pos,view);
	
	pos = mul(pos,projection);
   
	output.position = pos;
    output.textureUV = input.textureUV;

    // compute view space normal
    output.normal = input.normal;

    return output;
}
//--------------------------------------------------------------------------------------

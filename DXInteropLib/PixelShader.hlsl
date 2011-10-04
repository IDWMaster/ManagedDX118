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


struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
	float2 textureUV : TEXCOORD0;
	float3 light:NORMAL1;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PixelShaderInput input) : SV_Target
{
    //return float4(1.0,1.0,1.0,1.0);
   return diffuseTexture.Sample(linearSampler, input.textureUV)*saturate(dot(input.light,input.normal));
}
//--------------------------------------------------------------------------------------

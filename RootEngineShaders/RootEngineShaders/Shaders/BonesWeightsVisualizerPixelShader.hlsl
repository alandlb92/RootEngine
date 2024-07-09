
#include "CBuffers.hlsl"

Texture2D mainTexture : register(t0); // Registrando a textura
SamplerState samplerState : register(s0); // Declara��o do sampler



struct PixelShaderInput
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

float4 BonesWeightsVisualizerPixelShader(PixelShaderInput IN) : SV_TARGET
{
    float3 mainColor = IN.color;   
    
    return float4(mainColor, 1);
}
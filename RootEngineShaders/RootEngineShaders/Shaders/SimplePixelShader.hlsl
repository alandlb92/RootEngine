
#include "CBuffers.hlsl"

Texture2D mainTexture : register(t0); // Registrando a textura
SamplerState samplerState : register(s0); // Declara��o do sampler



struct PixelShaderInput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
};

float4 SimplePixelShader(PixelShaderInput IN) : SV_TARGET
{
    float4 color;
    if (hasTexture == 1)
    {
        color = mainTexture.Sample(samplerState, IN.texCoord);
    }
    else
    {
        color = float4(1, 1, 1, 1);
    }

    color *= IN.color;
    return color;
}
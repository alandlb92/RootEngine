
#include "CBuffers.hlsl"

Texture2D mainTexture : register(t0); // Registrando a textura
SamplerState samplerState : register(s0); // Declara��o do sampler



struct PixelShaderInput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
    float3 normal : POSITION;
};

float4 SimplePixelShader(PixelShaderInput IN) : SV_TARGET
{
    //float4 mainColor;
    //if (hasTexture == 1)
    //{
    //    mainColor = mainTexture.Sample(samplerState, IN.texCoord);
    //}
    //else
    //{
    //    mainColor = float4(1, 1, 1, 1);
    //}

    //mainColor *= IN.color;
    
    //float4 ambientLight = float4(ambientLightColor, 1) * ambientLightStrength;
    //float4 finalColor = mainColor * ambientLight;
    float4 finalColor = float4(IN.normal, 1);
    
    return finalColor;
}
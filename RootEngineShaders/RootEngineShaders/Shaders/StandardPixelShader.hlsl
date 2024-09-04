
#include "CBuffers.hlsl"

Texture2D mainTexture : register(t0); // Registrando a textura
SamplerState samplerState : register(s0); // Declara��o do sampler



struct PixelShaderInput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

float4 SimplePixelShader(PixelShaderInput IN) : SV_TARGET
{
    float3 mainColor;
    if (hasTexture == 1)
    {
        mainColor = mainTexture.Sample(samplerState, IN.texCoord);
    }
    else
    {
        mainColor = float3(1, 1, 1);
    }
        
    mainColor *= IN.color;   

    float3 ambientLight = ambientLightColor * ambientLightStrength;    
            
    float3 appliedLight = ambientLight;
    
    float3 finalColor = mainColor * appliedLight;
    
    return float4(finalColor, 1);
}
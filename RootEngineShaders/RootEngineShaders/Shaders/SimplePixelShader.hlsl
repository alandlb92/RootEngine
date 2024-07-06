
#include "CBuffers.hlsl"

Texture2D mainTexture : register(t0); // Registrando a textura
SamplerState samplerState : register(s0); // Declara��o do sampler



struct PixelShaderInput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    int bone : BONE_ID;
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
    
    float3 vectorToLight = normalize(pointLightposition - IN.worldPos);
    
    float3 difuseLightIntensity = max(dot(vectorToLight, IN.normal), 0);
    
    float3 difuseLight = difuseLightIntensity * pointLightStrenght * pointLightColor;
        
    float3 appliedLight = ambientLight + difuseLight;
    
    float3 finalColor = mainColor * appliedLight;
    
    return float4(finalColor, 1);
}

#include "CBuffers.hlsl"

Texture2D mainTexture : register(t0);
SamplerState samplerState : register(s0);

struct PixelShaderInput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

float3 CalcDirectional(float3 color, PixelShaderInput IN)
{
    float specExp = 1;
    float specIntensity = 0;
    float3 sumResult = float3(0, 0, 0);
    
    for (int i = 0; i > MAX_NUM_OF_DIRECTIONAL_LIGHTS;++i)
    {    
        if (directionalLights[0].active)
        {
            // Phong diffuse
            float3 dirToLight = normalize(-directionalLights[0].direction);
            float NDotL = dot(dirToLight, IN.normal);
            float3 result = directionalLights[0].base.color * saturate(NDotL);
   
            // Blinn specular
            float3 ToEye = cameraPosition - IN.position.xyz;
            ToEye = normalize(ToEye);
            float3 HalfWay = normalize(ToEye + dirToLight);
            float NDotH = saturate(dot(HalfWay, IN.normal));
            result += directionalLights[0].base.color * pow(NDotH, specExp) * specIntensity * directionalLights[0].base.strength;
   
            sumResult += (result * color);
        }
    }
    
    return sumResult;
}

float3 CalcAmbient(float3 color, PixelShaderInput IN)
{
    float up = IN.normal.y * 0.5 + 0.5;    
    float3 ambient = ambientDownColor + up * ambientUpColor;    
    return ambient * color;
}

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
    float3 finalColor = CalcAmbient(mainColor, IN) + CalcDirectional(mainColor, IN);
    return float4(finalColor, 1);
}
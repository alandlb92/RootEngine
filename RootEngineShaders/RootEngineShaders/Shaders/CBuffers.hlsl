#define MAX_NUM_OF_BONES_PER_VERTEX 4

cbuffer PerApplication : register(b0)
{
    float4x4 projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    float4x4 viewMatrix;
}

cbuffer PerObject : register(b2)
{
    float4x4 worldMatrix;
}

cbuffer Globals : register(b3)
{
    int hasTexture;
    int boneSelectedId;
}

cbuffer LightBuffer : register(b4)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 pointLightColor;
    float offset;
    
    float3 pointLightposition;
    float pointLightStrenght;
}
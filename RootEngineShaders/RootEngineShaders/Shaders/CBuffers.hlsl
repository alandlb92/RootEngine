#include "LightData.hlsl"


#define MAX_NUM_OF_BONES_PER_VERTEX 4
#define MAX_NUM_OF_ANIMATION_CHANNELS 100

//Todo: this is not a cb maybe i need to put in another file 
struct BoneData
{
    int boneId[MAX_NUM_OF_BONES_PER_VERTEX];
    float weights[MAX_NUM_OF_BONES_PER_VERTEX];
};

cbuffer PerApplication : register(b0)
{
    float4x4 projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    float4x4 viewMatrix;
}

//todo: Make a cbuffer for animation data, because we don't need to send animMatrix for each object
cbuffer PerObject : register(b2)
{
    int hasTexture;
    int isSkinned;
    float4x4 worldMatrix;
    float4x4 animMatrix[MAX_NUM_OF_ANIMATION_CHANNELS];
}

cbuffer LightBuffer : register(b3)
{
    float3 ambientDownColor;
    float3 ambientUpColor;    
    DirectionalLight directionalLights[MAX_NUM_OF_DIRECTIONAL_LIGHTS];
    
}

//todo: this will be a struct and will be an array with max point light in the scene
//float3 pointLightColor;
//float offset;
    
//float3 pointLightposition;
//float pointLightStrength;



//cbuffer Globals : register(b3)
//{
//    int boneSelectedId;
//}
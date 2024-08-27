
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

cbuffer PerObject : register(b2)
{
    float4x4 worldMatrix;
    float4x4 animMatrix[MAX_NUM_OF_ANIMATION_CHANNELS];
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
    float pointLightStrength;
}
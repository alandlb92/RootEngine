
#include "CBuffers.hlsl"

struct AppData
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    BoneData boneData : BONEDATA;
};

struct VertexShaderOutput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

float4 CalculateAnimation(matrix mvp, AppData IN)
{
    float4x4 boneTransform = animMatrix[IN.boneData.boneId[0]] * IN.boneData.weights[0];
    boneTransform += animMatrix[IN.boneData.boneId[1]] * IN.boneData.weights[1];
    boneTransform += animMatrix[IN.boneData.boneId[2]] * IN.boneData.weights[2];
    boneTransform += animMatrix[IN.boneData.boneId[3]] * IN.boneData.weights[3];
    return mul(mvp, mul(boneTransform, float4(IN.position, 1.0f)));    
}

VertexShaderOutput SimpleVertexShader(AppData IN)
{
    VertexShaderOutput OUT;
    
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    
    if(isSkinned)
    {
        OUT.position = CalculateAnimation(mvp, IN);
    }
    else
    {
        OUT.position = mul(mvp, float4(IN.position, 1.0f));        
    }    
    
    OUT.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    OUT.texCoord = IN.texCoord;
    float3 normalTransformed = mul(worldMatrix, float4(IN.normal, 0.0));
    OUT.normal = normalize(normalTransformed);    
    OUT.worldPos = mul(worldMatrix, float4(IN.position, 1.0f));
    return OUT;
}
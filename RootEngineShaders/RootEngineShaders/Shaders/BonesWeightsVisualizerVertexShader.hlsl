
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
    float4 position : SV_POSITION;
};

float3 CalculateColorWeight(float weight)
{
    float3 color;
    
    if (weight <= 0.5f)
    {
        // Gradiente de azul para verde
        color.b = saturate(1.0f - 2.0f * weight);
        color.g = saturate(2.0f * weight);
        color.r = 0.0f;
    }
    else
    {
        // Gradiente de verde para vermelho
        color.b = 0.0f;
        color.g = saturate(1.0f - 2.0f * (weight - 0.5f));
        color.r = saturate(2.0f * (weight - 0.5f));
    }
    
    return color;
}

VertexShaderOutput BonesWeightsVisualizerVertexShader(AppData IN)
{
    VertexShaderOutput OUT;
            
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position = mul(mvp, float4(IN.position, 1.0f));
    
    bool containBone = false;
    int index = 0;
    for (int i = 0; i < MAX_NUM_OF_BONES_PER_VERTEX; i++)
    {
        if (IN.boneData.boneId[i] == boneSelectedId)
        {
            containBone = true;
            index = i;
            break;
        }
    }
    
    if (containBone)
    {
        OUT.color = float4(CalculateColorWeight(IN.boneData.weights[index]), 1.0f);
    }
    else
    {        
        OUT.color = float4(.0f, .0f, 1.0f, 1.0f);
    }   
    
    return OUT;
}
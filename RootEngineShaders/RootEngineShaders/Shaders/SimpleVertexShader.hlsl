
#include "CBuffers.hlsl"

struct AppData
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : POSITION;
};

struct VertexShaderOutput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
    float3 normal : POSITION;
};

VertexShaderOutput SimpleVertexShader(AppData IN)
{
    VertexShaderOutput OUT;
    
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position = mul(mvp, float4(IN.position, 1.0f));
    OUT.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    OUT.texCoord = IN.texCoord;
    OUT.normal = IN.normal;
    
    return OUT;
}
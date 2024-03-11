cbuffer PerApplication : register(b0)
{
    matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
    matrix worldMatrix;
}

struct AppData
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float4 position : SV_POSITION;
};

VertexShaderOutput SimpleVertexShader(AppData IN)
{
    VertexShaderOutput OUT;
    
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position = mul(mvp, float4(IN.position, 1.0f));
    OUT.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    OUT.texCoord = IN.texCoord;
    
    return OUT;
}
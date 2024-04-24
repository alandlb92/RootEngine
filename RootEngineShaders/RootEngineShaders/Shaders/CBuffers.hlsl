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

cbuffer Globals : register(b3)
{
    int hasTexture;
}

cbuffer LightBuffer : register(b4)
{
    float3 ambientLightColor;
    float ambientLightStrength;
}
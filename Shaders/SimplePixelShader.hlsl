Texture2D mainTexture : register(t0); // Registrando a textura
SamplerState samplerState : register(s0); // Declaração do sampler

struct PixelShaderInput
{
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
};

float4 SimplePixelShader(PixelShaderInput IN) : SV_TARGET
{
    float4 color = mainTexture.Sample(samplerState, IN.texCoord);
    color *= IN.color;
    return color;
}
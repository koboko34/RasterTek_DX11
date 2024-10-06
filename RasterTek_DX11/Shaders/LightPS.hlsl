Texture2D ShaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer
{
    float4 DiffuseColor;
    float3 LightDirection;
    float Padding;
};

struct PS_In
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
};

float4 main(PS_In p) : SV_TARGET
{
    float4 TextureColor;
    float4 Color;
    float LightIntensity;
    float3 LightDir;
    
    TextureColor = ShaderTexture.Sample(SampleType, p.TexCoord);
    
    LightDir = -LightDirection;
    LightIntensity = saturate(dot(p.Normal, LightDir));
    
    Color = saturate(DiffuseColor * LightIntensity);
    Color *= TextureColor;
    
    return Color;
}
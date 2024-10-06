Texture2D ShaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer
{
    float4 AmbientColor;
    float4 DiffuseColor;
    float3 LightDirection;
    float SpecularPower;
    float4 SpecularColor;
};

struct PS_In
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 ViewDirection : TEXCOORD1;
};

float4 main(PS_In p) : SV_TARGET
{
    float4 TextureColor;
    float4 Color;
    float LightIntensity;
    float3 LightDir;
    float3 Reflection;
    float4 Specular;
    
    TextureColor = ShaderTexture.Sample(SampleType, p.TexCoord);
    
    Color = AmbientColor;
    
    Specular = float4(0.f, 0.f, 0.f, 0.f);
    
    LightDir = -LightDirection;
    LightIntensity = saturate(dot(p.Normal, LightDir));
    
    if (LightIntensity > 0.f)
    {
        Color += (DiffuseColor * LightIntensity);
        Color = saturate(Color);
        
        Reflection = normalize(2.f * LightIntensity * p.Normal - LightDir);
        
        Specular = pow(saturate(dot(Reflection, p.ViewDirection)), SpecularPower);
    }
    
    Color *= TextureColor;
    
    Specular *= SpecularColor;
    
    Color = saturate(Color + Specular);
    
    return Color;
}
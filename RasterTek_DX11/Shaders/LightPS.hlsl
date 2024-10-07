#define NUM_LIGHTS 4

Texture2D ShaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightColorBuffer
{
	float4 DiffuseColor[NUM_LIGHTS];
};

struct PS_In
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 LightDir[NUM_LIGHTS] : TEXCOORD1;
};

float4 main(PS_In p) : SV_TARGET
{
	float4 TextureColor;
	float LightIntensity[NUM_LIGHTS];
	float4 ColorArray[NUM_LIGHTS];
	float4 ColorSum;
	float4 Color;
	
	TextureColor = ShaderTexture.Sample(SampleType, p.TexCoord);
	
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		LightIntensity[i] = saturate(dot(p.LightDir[i], p.Normal));
		
		ColorArray[i] = DiffuseColor[i] * LightIntensity[i];
	}
	
	ColorSum = float4(0.f, 0.f, 0.f, 1.f);
	
	for (i = 0; i < NUM_LIGHTS; i++)
	{
		ColorSum.r += ColorArray[i].r;
		ColorSum.g += ColorArray[i].g;
		ColorSum.b += ColorArray[i].b;
	}
	
	Color = saturate(ColorSum) * TextureColor;
	
	return Color;
}
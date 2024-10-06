cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

cbuffer CameraBuffer
{
    float3 CameraPos;
    float Padding;
};

struct VS_In
{
	float4 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct VS_Out
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL;
    float3 ViewDirection : TEXCOORD1;
};

VS_Out main(VS_In v)
{
	VS_Out o;
    float4 WorldPos;
	
	v.Pos.w = 1.f;

	o.Pos = mul(v.Pos, WorldMatrix);
	o.Pos = mul(o.Pos, ViewMatrix);
	o.Pos = mul(o.Pos, ProjectionMatrix);

	o.TexCoord = v.TexCoord;
	o.Normal = mul(v.Normal, (float3x3)WorldMatrix);
	o.Normal = normalize(o.Normal);
	
    WorldPos = mul(v.Pos, WorldMatrix);
    o.ViewDirection = CameraPos.xyz - WorldPos.xyz;
    o.ViewDirection = normalize(o.ViewDirection);
	
	return o;
}
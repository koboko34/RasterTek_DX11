cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VS_In
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_Out
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VS_Out main(VS_In v)
{
	VS_Out o;
	v.Pos.w = 1.f;

	o.Pos = mul(v.Pos, WorldMatrix);
	o.Pos = mul(o.Pos, ViewMatrix);
	o.Pos = mul(o.Pos, ProjectionMatrix);

	o.Color = v.Color;

	return o;
}
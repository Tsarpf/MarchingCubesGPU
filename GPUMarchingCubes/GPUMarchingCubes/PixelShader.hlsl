struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};
float4 main(VS_OUTPUT input) : SV_TARGET 
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	return input.Color;
}
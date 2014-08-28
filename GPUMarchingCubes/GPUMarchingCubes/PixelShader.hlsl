struct PS_INPUT 
{
	float4 Color : COLOR0;
	float4 Pos : SV_POSITION;
};
float4 main(PS_INPUT input) : SV_TARGET 
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	return float4(input.Color.xyz, 1);
}
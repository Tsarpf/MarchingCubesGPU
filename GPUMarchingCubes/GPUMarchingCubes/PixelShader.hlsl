struct PS_INPUT 
{
	float4 Color : COLOR0;
	float4 Pos : SV_POSITION;
	float4 WorldPos : TEXCOORD;
	float3 Normal : NORMAL;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 LightPosition;
}

float4 main(PS_INPUT input) : SV_TARGET 
{
	float3 lightDir = normalize(LightPosition.xyz - input.WorldPos.xyz);
	float4 lightIntensity = dot(lightDir, input.Normal);
	//float reflectance = 0.4;
	float4 color = saturate(lightIntensity * input.Color);
	//float4 color = lightIntensity * input.Color;
	color = float4(color.xyz, 1);
	return color;

	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	//return float4(input.Color.xyz, 1);
}
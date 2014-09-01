struct PS_INPUT 
{
	float4 Color : COLOR0;
	float4 Pos : SV_POSITION;
	float4 WorldPos : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 LightPosition;
}

cbuffer cbVertexDecals : register (b1)
{
	float4 decal[8];
	float4 dataStep;
}


SamplerState samplerPoint : register(s0);
Texture3D<float> densityTex : register(t0);

float getTexValue(float3 position)
{
	return densityTex.Sample(samplerPoint, position);
	//return densityTex.SampleLevel(samplerPoint, position, 0);
}

/*
float cubeValue(int i, float4 position)
{
float3 cubeposition = cubePos(i, position);
return densityTex.SampleLevel(samplerPoint, cubeposition, 0);
}
*/
float4 main(PS_INPUT input) : SV_TARGET 
{
	//float3 leftx1 = 

	float3 gradient = float3(
		getTexValue((input.WorldPos.xyz + float3(dataStep.x, 0, 0) + 1.0f) / 2.0f) - getTexValue((input.WorldPos.xyz + float3(-dataStep.x, 0, 0) + 1.0f) / 2.0f),
		getTexValue((input.WorldPos.xyz + float3(0, dataStep.y, 0) + 1.0f) / 2.0f) - getTexValue((input.WorldPos.xyz + float3(0, -dataStep.y, 0) + 1.0f) / 2.0f),
		getTexValue((input.WorldPos.xyz + float3(0, 0, dataStep.z) + 1.0f) / 2.0f) - getTexValue((input.WorldPos.xyz + float3(0, 0, -dataStep.z) + 1.0f) / 2.0f)
	);

	float3 lightDir = normalize(LightPosition.xyz - input.WorldPos.xyz);
	float3 normal = normalize(gradient);
	normal = -normal;

	//float3 lightDir = normalize(LightPosition.xyz - input.WorldPos.xyz);
	//float4 lightIntensity = dot(lightDir, input.Normal);
	//float4 lightIntensity = dot(lightDir, normal);
	float4 lightIntensity = dot(lightDir, normal);
	//float reflectance = 0.4;
	float4 color = saturate(lightIntensity * input.Color);
	//float4 color = lightIntensity * input.Color;
	color = float4(color.xyz, 1);
	return color;

	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	//return float4(input.Color.xyz, 1);
}
//PIxel shader input description
struct PS_INPUT 
{
	float4 Color : COLOR0;
	float4 Pos : SV_POSITION;
	float4 WorldPos : TEXCOORD;
};

/*
Per frame constant buffer
*/
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 LightPosition;
}

//Once per app run constant buffer
cbuffer cbVertexDecals : register (b1)
{
	float4 decal[8];
	float4 dataStep;
}


//The samplerstate used to sample textures
SamplerState samplerPoint : register(s0);
//The texture containing the densities
Texture3D<float> densityTex : register(t0);

//Samples the texture and gets a value at the specific poitn
float getTexValue(float3 position)
{
	return densityTex.Sample(samplerPoint, position);
}

//Pixel shader main function
float4 main(PS_INPUT input) : SV_TARGET 
{
	//Gradient, AKA where in which direction within the scalar field the scalars change the most from this point. Here it is used to get an approximation of the surface's normal at this point.
	float3 gradient = float3(
		getTexValue((input.WorldPos.xyz + float3(dataStep.x, 0, 0) + 1.0f) / 2.0f) - getTexValue((input.WorldPos.xyz + float3(-dataStep.x, 0, 0) + 1.0f) / 2.0f),
		getTexValue((input.WorldPos.xyz + float3(0, dataStep.y, 0) + 1.0f) / 2.0f) - getTexValue((input.WorldPos.xyz + float3(0, -dataStep.y, 0) + 1.0f) / 2.0f),
		getTexValue((input.WorldPos.xyz + float3(0, 0, dataStep.z) + 1.0f) / 2.0f) - getTexValue((input.WorldPos.xyz + float3(0, 0, -dataStep.z) + 1.0f) / 2.0f)
	);

	float3 lightDir = normalize(LightPosition.xyz - input.WorldPos.xyz);
	float3 normal = normalize(gradient);
	normal = -normal;

	//Simplified lighting calculation
	float4 lightIntensity = dot(lightDir, normal);
	//float reflectance = 0.4;
	float4 color = saturate(lightIntensity * input.Color);
	//float4 color = lightIntensity * input.Color;
	color = float4(color.xyz, 1);
	return color;
}
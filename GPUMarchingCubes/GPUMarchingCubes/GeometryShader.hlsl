struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

struct GS_OUTPUT
{
	float4 Color : COLOR0;
	float4 Pos : SV_POSITION;
	float4 WorldPos : TEXCOORD;
};

SamplerState samplerPoint : register(s0);
Texture3D<float> densityTex : register(t1);
Texture2D<int> tritableTex : register (t0);


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
	float4 dataSize;
}

float3 cubePos(int i, float4 position)
{
	return position.xyz + decal[i].xyz;
}

float cubeValue(int i, float4 position)
{
	float3 cubeposition = cubePos(i, position);
	//float3 cubepositionfixed = (cubeposition + 1.0f) / 2.0f;
	return densityTex.SampleLevel(samplerPoint, cubeposition, 0);
}

float3 vertexInterp(float isoLevel, float3 v0, float l0, float3 v1, float l1)
{
	float lerper = (isoLevel - l0) / (l1 - l0);
	return lerp(v0, v1, lerper);
}


int triTableValue(int i, int j)
{
	if (i >= 256 || j >= 16)
	{
		return -1;
	}

	return tritableTex.Load(int3(j, i, 0));
}

float4 getProjectionPos(float4 position)
{
	position = mul(position, World);
	position = mul(position, View);
	position = mul(position, Projection);
	return position;
}

[maxvertexcount(18)]
void main(point GS_INPUT input[1], inout TriangleStream<GS_OUTPUT> triStream)
{
	float isolevel = 0.50f;

	float4 position = float4(input[0].Pos.xyz, 1);

	float3 cubePoses[8];
	float cubeVals[8];
	for (int i = 0; i < 8; i++)
	{
		cubePoses[i] = position.xyz + decal[i].xyz;
		cubeVals[i] = densityTex.SampleLevel(samplerPoint, (cubePoses[i] + 1.0f) / 2.0f, 0);
	}

	int cubeindex = 0;
	cubeindex = int(cubeVals[0] < isolevel);
	cubeindex += int(cubeVals[1] < isolevel) * 2;
	cubeindex += int(cubeVals[2] < isolevel) * 4;
	cubeindex += int(cubeVals[3] < isolevel) * 8;
	cubeindex += int(cubeVals[4] < isolevel) * 16;
	cubeindex += int(cubeVals[5] < isolevel) * 32;
	cubeindex += int(cubeVals[6] < isolevel) * 64;
	cubeindex += int(cubeVals[7] < isolevel) * 128;
	
	if (cubeindex != 0 && cubeindex != 255)
	{
		float3 vertlist[12];

		//Find the vertices where the surface intersects the cube
		vertlist[0] =  vertexInterp(isolevel, cubePoses[0], cubeVals[0], cubePoses[1], cubeVals[1]);
		vertlist[1] =  vertexInterp(isolevel, cubePoses[1], cubeVals[1], cubePoses[2], cubeVals[2]);
		vertlist[2] =  vertexInterp(isolevel, cubePoses[2], cubeVals[2], cubePoses[3], cubeVals[3]);
		vertlist[3] =  vertexInterp(isolevel, cubePoses[3], cubeVals[3], cubePoses[0], cubeVals[0]);
		vertlist[4] =  vertexInterp(isolevel, cubePoses[4], cubeVals[4], cubePoses[5], cubeVals[5]);
		vertlist[5] =  vertexInterp(isolevel, cubePoses[5], cubeVals[5], cubePoses[6], cubeVals[6]);
		vertlist[6] =  vertexInterp(isolevel, cubePoses[6], cubeVals[6], cubePoses[7], cubeVals[7]);
		vertlist[7] =  vertexInterp(isolevel, cubePoses[7], cubeVals[7], cubePoses[4], cubeVals[4]);
		vertlist[8] =  vertexInterp(isolevel, cubePoses[0], cubeVals[0], cubePoses[4], cubeVals[4]);
		vertlist[9] =  vertexInterp(isolevel, cubePoses[1], cubeVals[1], cubePoses[5], cubeVals[5]);
		vertlist[10] = vertexInterp(isolevel, cubePoses[2], cubeVals[2], cubePoses[6], cubeVals[6]);
		vertlist[11] = vertexInterp(isolevel, cubePoses[3], cubeVals[3], cubePoses[7], cubeVals[7]);

		GS_OUTPUT output;
		output.Color = float4(1, 0.2, 0.2, 1);
		//output.Color = input[0].Color;


		GS_OUTPUT point1;
		GS_OUTPUT point2;
		GS_OUTPUT point3;

		float4 worldPos1;
		float4 worldPos2;
		float4 worldPos3;

		float3 vector1;
		float3 vector2;
		for (int i = 0; triTableValue(cubeindex, i) != -1; i += 3)
		{
			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 0)], 1);
			//worldPos1 = mul(output.Pos, World);
			worldPos1 = output.Pos;
			output.Pos = getProjectionPos(output.Pos);
			point1 = output;
			
			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 1)], 1);
			//worldPos2 = mul(output.Pos, World);
			worldPos2 = output.Pos;
			output.Pos = getProjectionPos(output.Pos);
			point2 = output;

			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 2)], 1);
			//worldPos3 = mul(output.Pos, World);
			worldPos3 = output.Pos;
			output.Pos = getProjectionPos(output.Pos);
			point3 = output;

			//vector1 = worldPos1 - worldPos2;
			//vector2 = worldPos1 - worldPos3;

			//float3 normalVec = normalize(cross(vector1, vector2));

			//point1.Normal = normalVec;
			//point2.Normal = normalVec;
			//point3.Normal = normalVec;

			point1.WorldPos = worldPos1;
			point2.WorldPos = worldPos2;
			point3.WorldPos = worldPos3;

			triStream.Append(point1);
			triStream.Append(point2);
			triStream.Append(point3);
			triStream.RestartStrip();
		}
	}
}
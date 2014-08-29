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
	float3 Normal : NORMAL;

};

SamplerState samplerPoint : register(s0);
Texture2D<int> tritableTex : register (t0);
//Texture3D<float> densityTex : register(t1);
Texture3D<float> densityTex : register(t1);


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
}

//static float isolevel = 0.2;
//static float4 Position = float4(0,0,0,0);

float3 cubePos(int i, float4 position)
{
	return position.xyz + decal[i].xyz;
}

float cubeValue(int i, float4 position)
{
	float3 cubeposition = cubePos(i, position);
	float3 cubepositionfixed = (cubeposition + 1.0f) / 2.0f;
	return densityTex.SampleLevel(samplerPoint, cubeposition, 0);
		//return densityTex.Load(int4(cubepositionfixed.xyz, 0));
}

float3 vertexInterp(float isoLevel, float3 v0, float l0, float3 v1, float l1)
{
	float lerper = (isoLevel - l0) / (l1 - l0);
	float3 lerpper = float3(lerper, lerper, lerper);
	return lerp(v0, v1, lerpper);
}

int triTableValue(int i, int j)
{
	//return triTable[i][j];
	if (i >= 256 || j >= 16)
	//if (i >= 16 || j >= 256)
	{
		return -1;
	}

	return tritableTex.Load(int3(j, i, 0));
}

float4 getProjectionPos(float4 position)
{
	position = mul(position, World);
	position = mul(position, View);
	//position.x -= 0.5f;
	//position.y -= 0.5f;
	position = mul(position, Projection);
	return position;
}

[maxvertexcount(18)]
//void main(point GS_INPUT input[1], inout PointStream<GS_OUTPUT> triStream)
void main(point GS_INPUT input[1], inout TriangleStream<GS_OUTPUT> triStream)
{
	//Position = input[0].Pos;
	//float isolevel = 0.0005;
	float isolevel = 0.50f;

	float4 position = float4(input[0].Pos.xyz, 1);

	//position = (position + 1.0f) / 2.0f;
	//position = position * 2.0f - (float4(1.0f,1.0f,1.0f,0.0f));
	

		float3 cubePoses[8] =
	{
		(position.xyz + decal[0].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[1].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[2].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[3].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[4].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[5].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[6].xyz),// + 1.0f) / 2.0f,
		(position.xyz + decal[7].xyz),// + 1.0f) / 2.0f
	};


	//float3 cubePosition = (position.xyz + 1.0f) / 2.0f + decal[0].xyz;
	//float3 cubePosition = position.xyz + decal[0].xyz;
	//cubePosition = (cubePosition + 1.0) / 2.0;
	float dens0 = densityTex.SampleLevel(samplerPoint, (cubePoses[0] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[1].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens1 = densityTex.SampleLevel(samplerPoint, (cubePoses[1] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[2].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens2 = densityTex.SampleLevel(samplerPoint, (cubePoses[2] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[3].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens3 = densityTex.SampleLevel(samplerPoint, (cubePoses[3] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[4].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens4 = densityTex.SampleLevel(samplerPoint, (cubePoses[4] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[5].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens5 = densityTex.SampleLevel(samplerPoint, (cubePoses[5] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[6].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens6 = densityTex.SampleLevel(samplerPoint, (cubePoses[6] + 1.0f) / 2.0f, 0);

	//cubePosition = position.xyz + decal[7].xyz;
	//cubePosition = (cubePosition + 1.0f) / 2.0f;
	float dens7 = densityTex.SampleLevel(samplerPoint, (cubePoses[7] + 1.0f) / 2.0f, 0);


		float cubeVals[8] =
	{
		dens0,
		dens1,
		dens2,
		dens3,
		dens4,
		dens5,
		dens6,
		dens7,
	};

	//	float cubeVals[8] =
	//{
	//	cubeValue(0, position),
	//	cubeValue(1, position),
	//	cubeValue(2, position),
	//	cubeValue(3, position),
	//	cubeValue(4, position),
	//	cubeValue(5, position),
	//	cubeValue(6, position),
	//	cubeValue(7, position)
	//};

	int cubeindex = 0;
	cubeindex = int(cubeVals[0] < isolevel);
	cubeindex += int(cubeVals[1] < isolevel) * 2;
	cubeindex += int(cubeVals[2] < isolevel) * 4;
	cubeindex += int(cubeVals[3] < isolevel) * 8;
	cubeindex += int(cubeVals[4] < isolevel) * 16;
	cubeindex += int(cubeVals[5] < isolevel) * 32;
	cubeindex += int(cubeVals[6] < isolevel) * 64;
	cubeindex += int(cubeVals[7] < isolevel) * 128;
	
	//GS_OUTPUT derp;
	//derp.Color = float4(1, 0.2, 0.2, 1);
	//derp.Pos = getProjectionPos(input[0].Pos);
	//triStream.Append(derp);
	
	//Cube is entirely in/out of the surface
	//if (cubeindex == 255)
	//if (cubeindex == 0 || cubeindex == 255)
	if (cubeindex != 0 && cubeindex != 255)
	{
		float3 vertlist[12];
		//output.Pos = mul(Pos, World);
		//output.Pos = mul(output.Pos, View);
		//output.Pos = mul(output.Pos, Projection)

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
		//output.Color = input[0].Color;
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
			worldPos1 = mul(output.Pos, World);
			output.Pos = getProjectionPos(output.Pos);
			point1 = output;
			
			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 1)], 1);
			worldPos2 = mul(output.Pos, World);
			output.Pos = getProjectionPos(output.Pos);
			point2 = output;

			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 2)], 1);
			worldPos3 = mul(output.Pos, World);
			output.Pos = getProjectionPos(output.Pos);
			point3 = output;

			//Transform to world space, but not view and projection space
			//point1.Pos = mul(point1.Pos, World);
			//point2.Pos = mul(point1.Pos, World);
			//point3.Pos = mul(point1.Pos, World);

			//vector1 = point1.Pos - point2.Pos;
			vector1 = worldPos1 - worldPos2;
			//vector2 = point1.Pos - point3.Pos;
			vector2 = worldPos1 - worldPos3;

			float3 normalVec = normalize(cross(vector1, vector2));

			point1.Normal = normalVec;
			point2.Normal = normalVec;
			point3.Normal = normalVec;

			//point1.WorldPos = point1.Pos;
			point1.WorldPos = worldPos1;
			//point2.WorldPos = point2.Pos;
			point2.WorldPos = worldPos2;
			//point3.WorldPos = point3.Pos;
			point3.WorldPos = worldPos3;
			

			triStream.Append(point1);
			triStream.Append(point2);
			triStream.Append(point3);
			triStream.RestartStrip();
		}
	}
}
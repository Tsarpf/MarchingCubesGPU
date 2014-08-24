struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

SamplerState samplerPoint : register(s0);
Texture3D densityTex : register(t0);
Texture2D<int> tritableTex : register (t1);


cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer cbVertexDecals : register (b1)
{
	float4 decal[8];
}

//static float isolevel = 0.2;
//static float4 Position = float4(0,0,0,0);

float3 cubePos(int i, float4 position)
{
	return position.xyz + decal[i];
}

float cubeVal(int i, float4 position)
{
	return densityTex.SampleLevel(samplerPoint, (cubePos(i, position) + 1.0f) / 2.0f, 0);
}

float3 vertexInterp(float isoLevel, float3 v0, float l0, float3 v1, float l1)
{
	return lerp(v0, v1, (isoLevel - l0) / (l1 - l0));
}

int triTableValue(int i, int j)
{
	//return triTable[i][j];
	return tritableTex.Load(int3(i, j, 1));
}

float4 getProjectionPos(float4 position)
{
	position = mul(position, World);
	position = mul(position, View);
	position = mul(position, Projection);
	return position;
}

[maxvertexcount(12)]
void main(point VS_OUTPUT input[1], inout PointStream<VS_OUTPUT> triStream)
{
	//Position = input[0].Pos;
	float isolevel = 5000000000.5;
	int cubeindex = 0;

	float4 position = input[0].Pos;

	float cubeVal0 = cubeVal(0, position);
	float cubeVal1 = cubeVal(1, position);
	float cubeVal2 = cubeVal(2, position);
	float cubeVal3 = cubeVal(3, position);
	float cubeVal4 = cubeVal(4, position);
	float cubeVal5 = cubeVal(5, position);
	float cubeVal6 = cubeVal(6, position);
	float cubeVal7 = cubeVal(7, position);

	//Determine the index into the edge table which
	//tells us which vertices are inside of the surface
	cubeindex = int(cubeVal0 < isolevel);
	cubeindex += int(cubeVal1 < isolevel) * 2;
	cubeindex += int(cubeVal2 < isolevel) * 4;
	cubeindex += int(cubeVal3 < isolevel) * 8;
	cubeindex += int(cubeVal4 < isolevel) * 16;
	cubeindex += int(cubeVal5 < isolevel) * 32;
	cubeindex += int(cubeVal6 < isolevel) * 64;
	cubeindex += int(cubeVal7 < isolevel) * 128;

	//Cube is entirely in/out of the surface
	//if (cubeindex == 255)
	if (cubeindex == 0 || cubeindex == 255)
	//if (cubeindex == 0) //|| cubeindex == 255)
		return;

	float3 vertlist[12];
	
	//output.Pos = mul(Pos, World);
	//output.Pos = mul(output.Pos, View);
	//output.Pos = mul(output.Pos, Projection)

	//Find the vertices where the surface intersects the cube
	vertlist[0] = vertexInterp(isolevel, cubePos(0, position), cubeVal0, cubePos(1, position), cubeVal1);
	vertlist[1] = vertexInterp(isolevel, cubePos(1, position), cubeVal1, cubePos(2, position), cubeVal2);
	vertlist[2] = vertexInterp(isolevel, cubePos(2, position), cubeVal2, cubePos(3, position), cubeVal3);
	vertlist[3] = vertexInterp(isolevel, cubePos(3, position), cubeVal3, cubePos(0, position), cubeVal0);
	vertlist[4] = vertexInterp(isolevel, cubePos(4, position), cubeVal4, cubePos(5, position), cubeVal5);
	vertlist[5] = vertexInterp(isolevel, cubePos(5, position), cubeVal5, cubePos(6, position), cubeVal6);
	vertlist[6] = vertexInterp(isolevel, cubePos(6, position), cubeVal6, cubePos(7, position), cubeVal7);
	vertlist[7] = vertexInterp(isolevel, cubePos(7, position), cubeVal7, cubePos(4, position), cubeVal4);
	vertlist[8] = vertexInterp(isolevel, cubePos(0, position), cubeVal0, cubePos(4, position), cubeVal4);
	vertlist[9] = vertexInterp(isolevel, cubePos(1, position), cubeVal1, cubePos(5, position), cubeVal5);
	vertlist[10] = vertexInterp(isolevel, cubePos(2, position), cubeVal2, cubePos(6, position), cubeVal6);
	vertlist[11] = vertexInterp(isolevel, cubePos(3, position), cubeVal3, cubePos(7, position), cubeVal7);

	// Create the triangle
	//gl_FrontColor = vec4(cos(isolevel*10.0 - 0.5), sin(isolevel*10.0 - 0.5), cos(1.0 - isolevel), 1.0);
	int i = 0;
	//for (i=0; triTableValue(cubeindex, i)!=-1; i+=3) { //Strange bug with this way, uncomment to test
	VS_OUTPUT output;
	//output.Color = input[0].Color;
	output.Color = float4(1, 0.2, 0.2, 1);

	output.Pos = input[0].Pos;

	output.Pos = getProjectionPos(output.Pos);

	triStream.Append(output);
	output.Pos += float4(1, 0, 0, 0);
	triStream.Append(output);
	output.Pos += float4(1, 0, 0, 0);
	triStream.Append(output);
	triStream.RestartStrip();
	//triStream.Append(output);


	//for (int i = 0; triTableValue(cubeindex, i) != -1; i += 3)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		output.Pos = float4(vertlist[triTableValue(cubeindex, i + j)], 1);
	//		output.Pos = getProjectionPos(output.Pos);
	//		triStream.Append(output);
	//	}
	//	triStream.RestartStrip();
	//}
}
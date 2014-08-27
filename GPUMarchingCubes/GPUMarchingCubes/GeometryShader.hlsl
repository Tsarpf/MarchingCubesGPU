struct GS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

struct GS_OUTPUT
{
	float4 Color : COLOR0;
	float4 Pos : SV_POSITION;
};

SamplerState samplerPoint : register(s0);
Texture2D<int> tritableTex : register (t0);
Texture3D<float> densityTex : register(t1);


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
	return position.xyz + decal[i].xyz;
}

float cubeValue(int i, float4 position)
{
	float3 cubeposition = cubePos(i, position);
		float3 cubepositionfixed = (cubeposition + 1.0f) / 2.0f;
		//return densityTex.SampleLevel(samplerPoint, cubeposition, 0);
		return densityTex.Load(int4(cubepositionfixed.xyz, 0));
}

float3 vertexInterp(float isoLevel, float3 v0, float l0, float3 v1, float l1)
{
	return lerp(v0, v1, (isoLevel - l0) / (l1 - l0));
}

int triTableValue(int i, int j)
{
	//return triTable[i][j];
	return tritableTex.Load(int3(i, j, 0));
}

float4 getProjectionPos(float4 position)
{
	position = mul(position, World);
	position = mul(position, View);
	position = mul(position, Projection);
	return position;
}

[maxvertexcount(22)]
//void main(point GS_INPUT input[1], inout PointStream<GS_OUTPUT> triStream)
void main(point GS_INPUT input[1], inout TriangleStream<GS_OUTPUT> triStream)
{
	//Position = input[0].Pos;
	//This doesn't draw anything
	float isolevel = 0.3;
	//This draws all cubes (when cubeindex == 255 commented out below)
	//float isolevel = 5000000000.5;
	int cubeindex = 0;

	float4 position = float4(input[0].Pos.xyz, 1);
		position.x += 0.1;
		position.y += 0.1;

		float cubeVals[8] =
	{
		cubeValue(0, position),
		cubeValue(1, position),
		cubeValue(2, position),
		cubeValue(3, position),
		cubeValue(4, position),
		cubeValue(5, position),
		cubeValue(6, position),
		cubeValue(7, position)
	};
	//float cubeValueZero= cubeValue(0, position);
	//float cubeValueOne= cubeValue(1, position);
	//float cubeValueTwo= cubeValue(2, position);
	//float cubeValueThree= cubeValue(3, position);
	//float cubeValueFour= cubeValue(4, position);
	//float cubeValueFive = cubeValue(5, position);
	//float cubeValueSix= cubeValue(6, position);
	//float cubeValueSeven = cubeValue(7, position);

	//float cubeVal0 = cubeVal(0, position);
	//float cubeVal1 = cubeVal(1, position);
	//float cubeVal2 = cubeVal(2, position);
	//float cubeVal3 = cubeVal(3, position);
	//float cubeVal4 = cubeVal(4, position);
	//float cubeVal5 = cubeVal(5, position);
	//float cubeVal6 = cubeVal(6, position);
	//float cubeVal7 = cubeVal(7, position);

	//Determine the index into the edge table which
	//tells us which vertices are inside of the surface
	//cubeindex = int(cubeVals[0] < isolevel);
	//cubeindex += 1 * 2;
	//cubeindex += 1 * 4;
	//cubeindex += 1 * 8;
	//cubeindex += 1 * 16;

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
		vertlist[0] = vertexInterp(isolevel, cubePos(0, position), cubeVals[0], cubePos(1, position), cubeVals[1]);
		vertlist[1] = vertexInterp(isolevel, cubePos(1, position), cubeVals[1], cubePos(2, position), cubeVals[2]);
		vertlist[2] = vertexInterp(isolevel, cubePos(2, position), cubeVals[2], cubePos(3, position), cubeVals[3]);
		vertlist[3] = vertexInterp(isolevel, cubePos(3, position), cubeVals[3], cubePos(0, position), cubeVals[0]);
		vertlist[4] = vertexInterp(isolevel, cubePos(4, position), cubeVals[4], cubePos(5, position), cubeVals[5]);
		vertlist[5] = vertexInterp(isolevel, cubePos(5, position), cubeVals[5], cubePos(6, position), cubeVals[6]);
		vertlist[6] = vertexInterp(isolevel, cubePos(6, position), cubeVals[6], cubePos(7, position), cubeVals[7]);
		vertlist[7] = vertexInterp(isolevel, cubePos(7, position), cubeVals[7], cubePos(4, position), cubeVals[4]);
		vertlist[8] = vertexInterp(isolevel, cubePos(0, position), cubeVals[0], cubePos(4, position), cubeVals[4]);
		vertlist[9] = vertexInterp(isolevel, cubePos(1, position), cubeVals[1], cubePos(5, position), cubeVals[5]);
		vertlist[10] = vertexInterp(isolevel, cubePos(2, position), cubeVals[2], cubePos(6, position), cubeVals[6]);
		vertlist[11] = vertexInterp(isolevel, cubePos(3, position), cubeVals[3], cubePos(7, position), cubeVals[7]);

		/*vertlist[0] = vertexInterp(isolevel, cubePos(0, position), cubeValueZero, cubePos(1, position), cubeValueOne);
		vertlist[1] = vertexInterp(isolevel, cubePos(1, position), cubeVals[1], cubePos(2, position), cubeValueTwo);
		vertlist[2] = vertexInterp(isolevel, cubePos(2, position), cubeVals[2], cubePos(3, position), cubeValueThree);
		vertlist[3] = vertexInterp(isolevel, cubePos(3, position), cubeVals[3], cubePos(0, position), cubeVals[0]);
		vertlist[4] = vertexInterp(isolevel, cubePos(4, position), cubeVals[4], cubePos(5, position), cubeVals[5]);
		vertlist[5] = vertexInterp(isolevel, cubePos(5, position), cubeVals[5], cubePos(6, position), cubeVals[6]);
		vertlist[6] = vertexInterp(isolevel, cubePos(6, position), cubeVals[6], cubePos(7, position), cubeVals[7]);
		vertlist[7] = vertexInterp(isolevel, cubePos(7, position), cubeVals[7], cubePos(4, position), cubeVals[4]);
		vertlist[8] = vertexInterp(isolevel, cubePos(0, position), cubeVals[0], cubePos(4, position), cubeVals[4]);
		vertlist[9] = vertexInterp(isolevel, cubePos(1, position), cubeVals[1], cubePos(5, position), cubeVals[5]);
		vertlist[10] = vertexInterp(isolevel, cubePos(2, position), cubeVals[2], cubePos(6, position), cubeVals[6]);
		vertlist[11] = vertexInterp(isolevel, cubePos(3, position), cubeVals[3], cubePos(7, position), cubeVals[7]); */

		// Create the triangle
		//gl_FrontColor = vec4(cos(isolevel*10.0 - 0.5), sin(isolevel*10.0 - 0.5), cos(1.0 - isolevel), 1.0);
		//for (i=0; triTableValue(cubeindex, i)!=-1; i+=3) { //Strange bug with this way, uncomment to test

		GS_OUTPUT output;
		//output.Color = input[0].Color;
		output.Color = float4(1, 0.2, 0.2, 1);

		//output.Pos = input[0].Pos;
		//output.Pos = getProjectionPos(output.Pos);
		//triStream.Append(output);
		//output.Pos += float4(1, 0, 0, 0);
		//triStream.Append(output);
		//output.Pos += float4(1, 0, 0, 0);
		//triStream.Append(output);
		//triStream.RestartStrip();


		for (int i = 0; triTableValue(cubeindex, i) != -1; i += 3)
		{
			for (int j = 0; j < 3; j++)
			{
				output.Pos = float4(vertlist[triTableValue(cubeindex, i + j)], 1);
				output.Pos = getProjectionPos(output.Pos);
				triStream.Append(output);
			}
			triStream.RestartStrip();
		}
	}
}
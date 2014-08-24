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

//cbuffer kek : register (b2)
//{
//	int edgeTable[256];
//}


static float isolevel = 0.5;
static float4 Position = float4(0,0,0,0);

//int triTable[256][16];
//int edgeTable[256];
//static int edgeTable[256] = {
//	0x0, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
//	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
//	0x190, 0x99, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
//	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
//	0x230, 0x339, 0x33, 0x13a, 0x636, 0x73f, 0x435, 0x53c,
//	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
//	0x3a0, 0x2a9, 0x1a3, 0xaa, 0x7a6, 0x6af, 0x5a5, 0x4ac,
//	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
//	0x460, 0x569, 0x663, 0x76a, 0x66, 0x16f, 0x265, 0x36c,
//	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
//	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff, 0x3f5, 0x2fc,
//	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
//	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55, 0x15c,
//	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
//	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc,
//	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
//	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
//	0xcc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
//	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
//	0x15c, 0x55, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
//	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
//	0x2fc, 0x3f5, 0xff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
//	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
//	0x36c, 0x265, 0x16f, 0x66, 0x76a, 0x663, 0x569, 0x460,
//	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
//	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa, 0x1a3, 0x2a9, 0x3a0,
//	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
//	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33, 0x339, 0x230,
//	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
//	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99, 0x190,
//	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
//	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };

float3 cubePos(int i)
{
	return Position.xyz + decal[i];
}

float cubeVal(int i)
{
	return densityTex.SampleLevel(samplerPoint, (cubePos(i) + 1.0f) / 2.0f, 0);
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
void main(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triStream)
{
	int cubeindex = 0;

	float cubeVal0 = cubeVal(0);
	float cubeVal1 = cubeVal(1);
	float cubeVal2 = cubeVal(2);
	float cubeVal3 = cubeVal(3);
	float cubeVal4 = cubeVal(4);
	float cubeVal5 = cubeVal(5);
	float cubeVal6 = cubeVal(6);
	float cubeVal7 = cubeVal(7);

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
	if (cubeindex == 0 || cubeindex == 255)
		return;

	float3 vertlist[12];
	
	//output.Pos = mul(Pos, World);
	//output.Pos = mul(output.Pos, View);
	//output.Pos = mul(output.Pos, Projection)

	//Find the vertices where the surface intersects the cube
	vertlist[0] = vertexInterp(isolevel, cubePos(0), cubeVal0, cubePos(1), cubeVal1);
	vertlist[1] = vertexInterp(isolevel, cubePos(1), cubeVal1, cubePos(2), cubeVal2);
	vertlist[2] = vertexInterp(isolevel, cubePos(2), cubeVal2, cubePos(3), cubeVal3);
	vertlist[3] = vertexInterp(isolevel, cubePos(3), cubeVal3, cubePos(0), cubeVal0);
	vertlist[4] = vertexInterp(isolevel, cubePos(4), cubeVal4, cubePos(5), cubeVal5);
	vertlist[5] = vertexInterp(isolevel, cubePos(5), cubeVal5, cubePos(6), cubeVal6);
	vertlist[6] = vertexInterp(isolevel, cubePos(6), cubeVal6, cubePos(7), cubeVal7);
	vertlist[7] = vertexInterp(isolevel, cubePos(7), cubeVal7, cubePos(4), cubeVal4);
	vertlist[8] = vertexInterp(isolevel, cubePos(0), cubeVal0, cubePos(4), cubeVal4);
	vertlist[9] = vertexInterp(isolevel, cubePos(1), cubeVal1, cubePos(5), cubeVal5);
	vertlist[10] = vertexInterp(isolevel, cubePos(2), cubeVal2, cubePos(6), cubeVal6);
	vertlist[11] = vertexInterp(isolevel, cubePos(3), cubeVal3, cubePos(7), cubeVal7);

	// Create the triangle
	//gl_FrontColor = vec4(cos(isolevel*10.0 - 0.5), sin(isolevel*10.0 - 0.5), cos(1.0 - isolevel), 1.0);
	int i = 0;
	//for (i=0; triTableValue(cubeindex, i)!=-1; i+=3) { //Strange bug with this way, uncomment to test
	VS_OUTPUT output;
	output.Color = input[0].Pos;
	while (true)
	{
		if (triTableValue(cubeindex, i) != -1)
		{
			//Generate first vertex of triangle//
			//Fill position varying attribute for fragment shader
			output.Pos = float4(vertlist[triTableValue(cubeindex, i)], 1);
			//Fill gl_Position attribute for vertex raster space position
			//gl_Position = gl_ModelViewProjectionMatrix* position;
			output.Pos = getProjectionPos(output.Pos);
			triStream.Append(output);
			//EmitVertex();

			//Generate second vertex of triangle//
			//Fill position varying attribute for fragment shader
			//position = float4(vertlist[triTableValue(cubeindex, i + 1)], 1);
			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 1)], 1);
			//Fill gl_Position attribute for vertex raster space position
			//gl_Position = gl_ModelViewProjectionMatrix* position;
			output.Pos = getProjectionPos(output.Pos);
			triStream.Append(output);
			//EmitVertex();

			//Generate last vertex of triangle//
			//Fill position varying attribute for fragment shader
			//position = float4(vertlist[triTableValue(cubeindex, i + 2)], 1);
			output.Pos = float4(vertlist[triTableValue(cubeindex, i + 2)], 1);
			//Fill gl_Position attribute for vertex raster space position
			//gl_Position = gl_ModelViewProjectionMatrix* position;
			output.Pos = getProjectionPos(output.Pos);
			//EmitVertex();
			triStream.Append(output);

			//End triangle strip at firts triangle
			//EndPrimitive();
			triStream.RestartStrip();
		}
		else
		{
			break;
		}

		i = i + 3; //Comment it for testing the strange bug
	}
}
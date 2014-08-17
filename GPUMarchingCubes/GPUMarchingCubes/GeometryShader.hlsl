struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

[maxvertexcount(3)]
void main(triangle VS_OUTPUT input[3], inout TriangleStream<VS_OUTPUT> triStream)
{
	VS_OUTPUT v;
	v = input[0];
	v.Pos.x += 1;
	triStream.Append(v);
	//triStream.Append(input[0]);
	triStream.Append(input[1]);
	triStream.Append(input[2]);
}

/*
[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i];
		output.Append(element);
	}
}
*/
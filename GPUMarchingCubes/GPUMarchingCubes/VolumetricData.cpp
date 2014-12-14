#include "VolumetricData.h"
#include <random>
#include <cstdlib>
#include <time.h>


VolumetricData::VolumetricData(int Height, int Width, int Depth, XMFLOAT3 CubeSize, XMFLOAT3 CubeStep) : 
m_height(Height), m_width(Width), m_depth(Depth), m_cubeSize(CubeSize), m_cubeStep(CubeStep)
{
	srand(time(NULL));
}
VolumetricData::~VolumetricData()
{
	delete[] m_data;
	//todo delete other stuff as well...
}

//Simple getter
ID3D11ShaderResourceView* VolumetricData::GetShaderResource()
{
	return m_shaderResourceView;
}

//Simple getter
ID3D11ShaderResourceView* VolumetricData::GetTriTableShaderResource()
{
	return m_triTableSRV;
}

/*
Initializes volumetric data stuff
*/
HRESULT VolumetricData::CreateTestData(const char type)
{


	double result = rand() % 10000;

	/***********
	Use one of these to set up the data 	
	************/
	switch (type)
	{
	case '1':
		createRandomNoise(result);
		break;
	case '2':
		createBumpySphere(result);
		break;
	case '3':
		createSphere();
		break;
	case '4':
		createDataArray();
		break;

	}


	createTextureDesc();
	createSubresourceData();
	HRESULT hr;
	if (FAILED(hr = createTexture()))
		return hr;

	if (FAILED(hr = createShaderResourceView()))
		return hr;
	
	return S_OK;
}

//Populates the list of cube corner offsets from the center
void VolumetricData::GetDecals(OnceBuffer& buffer)
{
	ZeroMemory(&buffer, sizeof(buffer));
	buffer.decal[0] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1);
	buffer.decal[1] = XMFLOAT4(m_cubeStep.x, 0.0f, 0.0f, 1);
	buffer.decal[2] = XMFLOAT4(m_cubeStep.x, m_cubeStep.y, 0.0f, 1);
	buffer.decal[3] = XMFLOAT4(0.0f, m_cubeStep.y, 0.0f, 1);
	buffer.decal[4] = XMFLOAT4(0.0f, 0.0f, m_cubeStep.z, 1);
	buffer.decal[5] = XMFLOAT4(m_cubeStep.x, 0.0f, m_cubeStep.z, 1);
	buffer.decal[6] = XMFLOAT4(m_cubeStep.x, m_cubeStep.y, m_cubeStep.z, 1);
	buffer.decal[7] = XMFLOAT4(0.0f, m_cubeStep.y, m_cubeStep.z, 1);
}

/*
Creates the shader view that the GPU will use as a look-up table to find out triangle configurations for different cases of the cube
*/
HRESULT VolumetricData::CreateTriTableResource()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Height = 256;
	desc.Width = 16;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_SINT;
	desc.SampleDesc = { 1, 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.SysMemPitch = 16 * sizeof(int);
	initData.SysMemSlicePitch = 0;

	initData.pSysMem = g_TriTable;

	ID3D11Texture2D* texture = NULL;
	HRESULT hr = g_d3dDevice->CreateTexture2D(&desc, &initData, &texture);
	if (FAILED(hr))
		return hr;

	hr = g_d3dDevice->CreateShaderResourceView(texture, NULL, &m_triTableSRV);
	return hr;
}

/*
Creates the 3d texture using the data we've generated so it can be into a shader resource and then be used and sampled by the GPU
*/
HRESULT VolumetricData::createTexture()
{
	return g_d3dDevice->CreateTexture3D(&m_texDesc, &m_subData, &m_texture);
}

/*
Creates the shader resource that the GPU can use directly
*/
HRESULT VolumetricData::createShaderResourceView()
{
	return g_d3dDevice->CreateShaderResourceView(m_texture, NULL, &m_shaderResourceView);
}

/*
Describes the texture type, contents, size, etc.
*/
void VolumetricData::createTextureDesc()
{
	m_texDesc.Width = m_width;
	m_texDesc.Height = m_height;
	m_texDesc.Depth = m_depth;
	m_texDesc.MipLevels = 1;
	m_texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	m_texDesc.Usage = D3D11_USAGE_DEFAULT;
	m_texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_texDesc.CPUAccessFlags = 0;
	m_texDesc.MiscFlags = 0;
}

/*
Sets up the data well put into the resource
*/
void VolumetricData::createSubresourceData()
{
	ZeroMemory(&m_subData, sizeof(m_subData));

	//Size of a 1d line. Eg. distance between adjacent values
	m_subData.SysMemPitch = m_width * sizeof(float);

	//Size of a 2D slice
	m_subData.SysMemSlicePitch = m_width * m_height * sizeof(float);

	//The actual data
	m_subData.pSysMem = m_data;
}

//Creates a simple surface with sane iso values
void VolumetricData::createDataArray()
{
	
	m_data = new float[m_depth*m_height*m_width];
	for (UINT z = 0; z < m_depth; z++)
	{
		for (UINT y = 0; y < m_height; y++)
		{
			for (UINT x = 0; x < m_width; x++)
			{
				//float result = (float)y / (float)m_height;
				//float result = ((float)y / (float)m_height + (float)z / (float)m_depth + (float)x / (float)m_width) / 3.0f; //+ 0.1f;
				//float result = ((float)y / (float)m_height + (float)z / (float)m_depth) / 2.0f; //+ 0.1f;
				float result = ((float)y / (float)m_height + (float)x / (float)m_width) / 2.0f; //+ 0.1f;
				//float result = ((float)x / (float)m_depth) / 1.0f; //+ 0.1f;
				//float result = 0;
				//if (y >= (float)m_height / 2.0f)
					//result = 1.0f;

				int idx = getIdx(x, y, z);
				m_data[idx] = result;

				//float result = (float)y / (float)m_height * 2.0f - 1.0f;
				//m_data[getIdx(x, y, z)] = 1.0f;
			}
		}
	}
}

//Simply samples coherent noise at every point of the array
void VolumetricData::createRandomNoise(double offset)
{
	m_data = new float[m_depth*m_height*m_width];
	for (UINT z = 0; z < m_depth; z++)
	{
		for (UINT y = 0; y < m_height; y++)
		{
			for (UINT x = 0; x < m_width; x++)
			{
				XMFLOAT3 pos(x, y, z);

				//Take distance's complement so the nearer to the center the bigger the density
				//float maxDistance = m_width / 2;
				//float result = 1.0f - (getDistance(pos, center) / maxDistance);
				//int idx = getIdx(x, y, z);
				//m_data[idx] = result;
				float valueX = (float)x / (float)m_width * 3.0f;
				float valueY = (float)y / (float)m_height * 3.0f;
				float valueZ = (float)z / (float)m_depth * 3.0f;

				double result = m_perlinNoise.GetValue(valueX + offset, valueY + offset, valueZ + offset);
				int idx = getIdx(x, y, z);
				m_data[idx] = result;
			}
		}
	}
}

//Creates a sphere but adds offsets using noise to make it bumpy
void VolumetricData::createBumpySphere(double offset)
{
	XMFLOAT3 center = XMFLOAT3(m_width / 2.0f, m_height / 2.0f, m_depth / 2.0f);
	m_data = new float[m_depth*m_height*m_width];
	for (UINT z = 0; z < m_depth; z++)
	{
		for (UINT y = 0; y < m_height; y++)
		{
			for (UINT x = 0; x < m_width; x++)
			{
				XMFLOAT3 pos(x, y, z);

				//Take distance's complement so the nearer to the center the bigger the density
				float maxDistance = m_width / 2.0f;
				float result = 1.0f - (getDistance(pos, center) / maxDistance);

				float xdivide = (float)m_width / 8.0f;
				//float xdivide = (float)m_width;
				float ydivide = (float)m_height / 8.0f;
				//float ydivide = (float)m_height;
				float zdivide = (float)m_depth / 8.0f;
				//float zdivide = (float)m_depth;
				float bump = (float)m_perlinNoise.GetValue(
					(float)x / xdivide + offset,
					(float)y / ydivide + offset,
					(float)z / zdivide + offset);

				//result += bump / 10.0f;
				result += bump / 6.5f;

				int idx = getIdx(x, y, z);
				m_data[idx] = result;
			}
		}
	}
}

/*
Creates a pretty smooth sphere
*/
void VolumetricData::createSphere()
{
	XMFLOAT3 center = XMFLOAT3(m_width / 2.0f, m_height / 2.0f, m_depth / 2.0f);
	m_data = new float[m_depth*m_height*m_width];
	for (UINT z = 0; z < m_depth; z++)
	{
		for (UINT y = 0; y < m_height; y++)
		{
			for (UINT x = 0; x < m_width; x++)
			{
				XMFLOAT3 pos(x, y, z);

				//Take distance's complement so the nearer to the center the bigger the density
				float maxDistance = m_width / 2.0f;
				float result = 1.0f - (getDistance(pos, center) / maxDistance);
				int idx = getIdx(x, y, z);
				m_data[idx] = result;
			}
		}
	}
}



/*
Gets the distance between two vectors
*/
float VolumetricData::getDistance(XMFLOAT3 p1, XMFLOAT3 p2)
{
	XMVECTOR vec1 = XMLoadFloat3(&p1);
	XMVECTOR vec2 = XMLoadFloat3(&p2);
	XMVECTOR length = XMVector3Length(vec1 - vec2);

	//Length is put into every component of the vector, lets just return x
	return XMVectorGetX(length);
}

//Simple getter
int VolumetricData::GetVertexCount()
{
	return m_vertexCount;
}

/*Creates a cube full of points placed at m_cubeStep.x/y/z intervals*/
int VolumetricData::GetVertices(SimpleVertex** outVertices)
{
	int size = int(2.0f / m_cubeStep.x);
	size = size * size * size;
	m_vertexCount = size;
	
	*outVertices = new SimpleVertex[size];
	int idx = 0;
	for (float z = -1; z < 1.0f; z += m_cubeStep.z)
	{
		for (float y = -1; y < 1.0f; y += m_cubeStep.y)
		{
			for (float x = -1; x < 1.0f; x += m_cubeStep.x)
			{
				//SimpleVertex* vertex = new SimpleVertex;
				//vertex->Color = XMFLOAT4((y + 1.0f) / 2.0f, 0.0f, 0.0f, 1.0f);

				(*outVertices)[idx] =
					{ XMFLOAT4(x, y, z, 1), XMFLOAT4((y + 1.0f) / 2.0f, 0.0f, 0.0f, 1.0f) };

				//SimpleVertex vertex =
				//	(*outVertices)[idx];




				idx++;
			}
		}
	}

	return size;
}



/*
Gets the correct index when we've flattened a 3d array to 1d array1
*/
int VolumetricData::getIdx(int x, int y, int z)
{
	//return x + (y * m_width) + (z * m_width * m_depth);
	return x + m_width * (y + m_height * z);
}
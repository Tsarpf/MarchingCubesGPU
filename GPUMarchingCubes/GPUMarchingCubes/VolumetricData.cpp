#include "VolumetricData.h"


VolumetricData::VolumetricData(int Height, int Width, int Depth, XMFLOAT3 CubeSize, XMFLOAT3 CubeStep) : 
m_height(Height), m_width(Width), m_depth(Depth), m_cubeSize(CubeSize), m_cubeStep(CubeStep)
{
}
VolumetricData::~VolumetricData()
{
	delete[] m_data;
	//todo delete other stuff as well...
}
ID3D11ShaderResourceView* VolumetricData::GetShaderResource()
{
	return m_shaderResourceView;
}

ID3D11ShaderResourceView* VolumetricData::GetTriTableShaderResource()
{
	return m_triTableSRV;
}

HRESULT VolumetricData::CreateTestData()
{
	createDataArray();
	createTextureDesc();
	createSubresourceData();
	HRESULT hr;
	if (FAILED(hr = createTexture()))
		return hr;

	if (FAILED(hr = createShaderResourceView()))
		return hr;
	
	return S_OK;
}

void VolumetricData::GetDecals(DecalBuffer& buffer)
{
	ZeroMemory(&buffer, sizeof(DecalBuffer));
	buffer.decal[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	buffer.decal[1] = XMFLOAT3(m_cubeStep.x, 0.0f, 0.0f);
	buffer.decal[2] = XMFLOAT3(m_cubeStep.x, m_cubeStep.y, 0.0f);
	buffer.decal[3] = XMFLOAT3(0.0f, m_cubeStep.y, 0.0f);
	buffer.decal[4] = XMFLOAT3(0.0f, 0.0f, m_cubeStep.z);
	buffer.decal[5] = XMFLOAT3(m_cubeStep.x, 0.0f, m_cubeStep.z);
	buffer.decal[6] = XMFLOAT3(m_cubeStep.x, m_cubeStep.y, m_cubeStep.z);
	buffer.decal[7] = XMFLOAT3(0.0f, m_cubeStep.y, m_cubeStep.z);
}

HRESULT VolumetricData::CreateTriTableResource()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Height = 256;
	desc.Width = 16;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R32_SINT;
	desc.SampleDesc = { 1, 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.SysMemPitch = 16 * sizeof(float);
	initData.SysMemSlicePitch = 16 * 256 * sizeof(float);
	initData.pSysMem = g_TriTable;

	ID3D11Texture2D* texture;
	HRESULT hr = g_d3dDevice->CreateTexture2D(&desc, &initData, &texture);
	if (FAILED(hr))
		return hr;

	hr = g_d3dDevice->CreateShaderResourceView(texture, NULL, &m_triTableSRV);
	return hr;
}

HRESULT VolumetricData::createTexture()
{
	return g_d3dDevice->CreateTexture3D(&m_texDesc, &m_subData, &m_texture);
}

HRESULT VolumetricData::createShaderResourceView()
{
	return g_d3dDevice->CreateShaderResourceView(m_texture, NULL, &m_shaderResourceView);
}

void VolumetricData::createTextureDesc()
{
	m_texDesc.Width = m_width;
	m_texDesc.Height = m_height;
	m_texDesc.Depth = m_depth;
	m_texDesc.MipLevels = 1;
	//m_texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	m_texDesc.Usage = D3D11_USAGE_DEFAULT;
	m_texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_texDesc.CPUAccessFlags = 0;
	m_texDesc.MiscFlags = 0;
}

void VolumetricData::createSubresourceData()
{
	ZeroMemory(&m_subData, sizeof(m_subData));
	m_subData.SysMemPitch = m_width * sizeof(float);
	m_subData.SysMemSlicePitch = m_width * m_height * sizeof(float);
	m_subData.pSysMem = m_data;
}

void VolumetricData::createDataArray()
{
	m_data = new float[m_depth*m_height*m_width];
	for (int z = 0; z < m_depth; z++)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				m_data[getIdx(x, y, z)] = y / m_height * 2 - 1;
			}
		}
	}
}

int VolumetricData::GetVertexCount()
{
	return m_vertexCount;
}

int VolumetricData::GetVertices(SimpleVertex** outVertices)
{
	int size = 2.0f / m_cubeStep.x;
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
					{ XMFLOAT3(x, y, z), XMFLOAT4((y + 1.0f) / 4.0f, 0.0f, 0.0f, 1.0f) };

				idx++;
			}
		}
	}

	return size;
}



int VolumetricData::getIdx(int x, int y, int z)
{
	return x + (y * m_width) + (z * m_width * m_height);
}
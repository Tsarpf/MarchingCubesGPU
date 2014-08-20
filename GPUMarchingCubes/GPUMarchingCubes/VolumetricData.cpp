#include "VolumetricData.h"


VolumetricData::VolumetricData(int Height, int Width, int Depth) : m_height(Height), m_width(Width), m_depth(Depth)
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
	m_texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
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

int VolumetricData::getIdx(int x, int y, int z)
{
	return x + (y * m_width) + (z * m_width * m_height);
}
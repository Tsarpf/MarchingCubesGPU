#include "VolumetricData.h"


VolumetricData::VolumetricData(int Height, int Width, int Depth) : m_height(Height), m_width(Width), m_depth(Depth)
{
}


VolumetricData::~VolumetricData()
{
}


void VolumetricData::generateTestData()
{
	createTextureDesc();
	createSubresourceData();
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
	//m_subData.pSysMem
	m_subData.SysMemPitch = m_width * sizeof(float);
	m_subData.SysMemSlicePitch = m_width * m_height * sizeof(float);
	//m_subData.pSysMem
	//D3D11_SUBRESOURCE_DATA initData;
	//ZeroMemory(&initData, sizeof(initData));
	//initData.pSysMem = vertices;
}

void VolumetricData::createDataArray()
{
		
}
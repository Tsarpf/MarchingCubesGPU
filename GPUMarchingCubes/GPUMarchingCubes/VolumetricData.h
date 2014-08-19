#pragma once

#include <D3D11.h>
//#include <D3DX11.h>

extern ID3D11Device* g_d3dDevice;

class VolumetricData
{
public:
	VolumetricData(int Width, int Height, int Depth);
	~VolumetricData();
private:
	void generateTestData();
	void createTextureDesc();
	void createSubresourceData();
	void createDataArray();


	float* data;
	ID3D11Texture3D* m_Texture;
	UINT m_width, m_height, m_depth;
	D3D11_TEXTURE3D_DESC m_texDesc;
	D3D11_SUBRESOURCE_DATA  m_subData;

};


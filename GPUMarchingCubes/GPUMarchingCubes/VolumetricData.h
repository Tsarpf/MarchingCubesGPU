#pragma once

#include <D3D11.h>
#include <xnamath.h>
//#include "DirectXApp.h"
//#include <D3DX11.h>

extern ID3D11Device* g_d3dDevice;

/*vertex decals description*/
struct DecalBuffer 
{
	XMFLOAT3 decal0;
	XMFLOAT3 decal1;
	XMFLOAT3 decal2;
	XMFLOAT3 decal3;
	XMFLOAT3 decal4;
	XMFLOAT3 decal5;
	XMFLOAT3 decal6;
	XMFLOAT3 decal7;
};

class VolumetricData
{
public:
	VolumetricData(int Width, int Height, int Depth, XMFLOAT3 CubeSize, XMFLOAT3 CubeStep);
	~VolumetricData();

	HRESULT CreateTestData();
	ID3D11ShaderResourceView* GetShaderResource();
	void GetDecals(DecalBuffer&);
	
private:
	//Copy construction disabled by making the copy constructor private.
	VolumetricData(const VolumetricData&);

	HRESULT createTexture();
	HRESULT createShaderResourceView();
	void createTextureDesc();
	void createSubresourceData();
	void createDataArray();
	int getIdx(int,int,int);


	XMFLOAT3 m_cubeSize;
	XMFLOAT3 m_cubeStep;
	float* m_data;
	ID3D11Texture3D* m_texture;
	ID3D11ShaderResourceView* m_shaderResourceView;
	UINT m_width, m_height, m_depth;
	D3D11_TEXTURE3D_DESC m_texDesc;
	D3D11_SUBRESOURCE_DATA  m_subData;
};




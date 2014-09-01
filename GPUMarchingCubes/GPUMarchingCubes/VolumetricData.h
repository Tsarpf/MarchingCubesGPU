#pragma once

#include <D3D11.h>
#include <xnamath.h>
#include <noise\noise.h>
//#include "DirectXApp.h"
//#include <D3DX11.h>

extern ID3D11Device* g_d3dDevice;
//extern int g_TriTable[256][16];
extern int g_TriTable[][16];

/*vertex decals description*/
struct OnceBuffer 
{
	XMFLOAT4 decal[8];
	XMFLOAT4 dataStep;
	//XMFLOAT3 decal1;
	//XMFLOAT3 decal2;
	//XMFLOAT3 decal3;
	//XMFLOAT3 decal4;
	//XMFLOAT3 decal5;
	//XMFLOAT3 decal6;
	//XMFLOAT3 decal7;
};


/*
Vertex description
*/
struct SimpleVertex
{
	XMFLOAT4 Pos;
	XMFLOAT4 Color;
};



class VolumetricData
{
public:
	VolumetricData(int Width, int Height, int Depth, XMFLOAT3 CubeSize, XMFLOAT3 CubeStep);
	~VolumetricData();

	HRESULT CreateTestData();
	ID3D11ShaderResourceView* GetShaderResource();
	void GetDecals(OnceBuffer&);
	int GetVertices(SimpleVertex** outVertices);
	int GetVertexCount();
	ID3D11ShaderResourceView* GetTriTableShaderResource();
	HRESULT CreateTriTableResource();
	
private:
	//Copy construction disabled by making the copy constructor private.
	VolumetricData(const VolumetricData&);

	HRESULT createTexture();
	HRESULT createShaderResourceView();
	void createTextureDesc();
	void createSubresourceData();

	//Functions for creating noise data
	void createDataArray();
	void createSphere();
	void createRandomNoise();
	void createBumpySphere();

	float getDistance(XMFLOAT3, XMFLOAT3);
	int getIdx(int,int,int);


	noise::module::Perlin m_perlinNoise;
	int m_vertexCount;
	XMFLOAT3 m_cubeSize;
	XMFLOAT3 m_cubeStep;
	float* m_data;
	ID3D11Texture3D* m_texture;
	ID3D11ShaderResourceView* m_shaderResourceView = NULL;
	ID3D11ShaderResourceView* m_triTableSRV = NULL;
	UINT m_width, m_height, m_depth;
	D3D11_TEXTURE3D_DESC m_texDesc;
	D3D11_SUBRESOURCE_DATA  m_subData;
};




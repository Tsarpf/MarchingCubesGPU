#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <d3dcompiler.h>

#include "VolumetricData.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


class DirectXApp
{
public:
    DirectXApp();
    ~DirectXApp();
    bool Init(HINSTANCE, HINSTANCE, LPWSTR, int);
    bool Run();
private:
    HRESULT createWindow(HINSTANCE, int);
    bool initDX();
    void render();
	HRESULT compileAndEnableShaders();
	HRESULT compileShaderFromFile(WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** OutBlob);
	HRESULT setupConstantBuffer();
	HRESULT createDepthStencil();
	HRESULT setupVertexAndIndexAndSOBuffer();
	HRESULT setupVisualizationData();

	//Window and viewport size
	UINT m_width = 640;
	UINT m_height = 480;
};
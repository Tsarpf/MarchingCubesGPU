#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


class DirectXApp
{
public:
    DirectXApp();
    ~DirectXApp();
    bool init(HINSTANCE, HINSTANCE, LPWSTR, int);
    bool run();
private:
    HRESULT createWindow(HINSTANCE, int);
    bool initDX();
    void render();
	bool compileAndEnableShaders();
};


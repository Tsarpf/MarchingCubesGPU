#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
};


#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE   g_hInst = NULL;
HWND        g_hWnd = NULL;

class DirectXApp
{
public:
    DirectXApp();
    ~DirectXApp();
    bool init();
    bool run();
private:
    bool createWindow(HINSTANCE, int);
};


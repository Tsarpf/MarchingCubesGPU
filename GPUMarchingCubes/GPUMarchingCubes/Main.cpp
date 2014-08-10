#define WIN32_LEAN_AND_MEAN
#include <Windows.h>    // include the basic windows header file
#include "DirectXApp.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//    MessageBox(NULL, L"Hello World!", L"Testing one two three!", MB_ICONEXCLAMATION | MB_OK);
//    return 0;
//}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

    DirectXApp dxapp;
    
    if(dxapp.init(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
        dxapp.run();
    
    //return (int)msg.
}
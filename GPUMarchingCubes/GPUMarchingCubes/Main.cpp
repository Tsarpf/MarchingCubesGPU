#define WIN32_LEAN_AND_MEAN //This define strips out a lot of unneeded stuff
#include <Windows.h>    //Basic windows header file
#include "DirectXApp.h" //Our dx app



//Application entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    DirectXApp dxapp;
    
    if(dxapp.Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
        dxapp.Run();
}
#include <windows.h>    // include the basic windows header file

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    MessageBox(NULL, L"Hello World!", L"Testing one two three!", MB_ICONEXCLAMATION | MB_OK);
    return 0;
}
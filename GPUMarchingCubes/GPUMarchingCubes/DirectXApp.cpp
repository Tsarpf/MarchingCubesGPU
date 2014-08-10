#include "DirectXApp.h"

//HINSTANCE g_hInst = NULL;
//HWND g_hWnd = NULL;
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

DirectXApp::DirectXApp()
{
}


DirectXApp::~DirectXApp()
{
}

bool DirectXApp::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    /*
    UINT32 AdapterOrdinal = 0;
    D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
    D3DCAPS9 caps;
    m_pD3D->GetDeviceCaps(AdapterOrdinal, DeviceType, &caps); // caps bits

    D3DPRESENT_PARAMETERS params;
    ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));

    // Swap chain parameters:
    params.hDeviceWindow = m_hWnd;
    params.AutoDepthStencilFormat = D3DFMT_D24X8;
    params.BackBufferFormat = D3DFMT_X8R8G8B8;
    params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    params.MultiSampleType = D3DMULTISAMPLE_NONE;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.Windowed = true;
    params.PresentationInterval = 0;
    params.BackBufferCount = 2;
    params.BackBufferWidth = 0;
    params.BackBufferHeight = 0;
    params.EnableAutoDepthStencil = true;
    params.Flags = 2;

    m_pD3D->CreateDevice(
        0,
        D3DDEVTYPE_HAL,
        m_hWnd,
        64,
        &params,
        &m_pd3dDevice
        );
    */

	if(FAILED(createWindow(hInstance, nCmdShow)))
		return false;

    return true;
}

bool DirectXApp::run()
{
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}


HRESULT DirectXApp::createWindow(HINSTANCE hInstance, int nCmdShow)
{

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    //wcex.hIcon = LoadIcon(0, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TestWindowClass";
    wcex.hIconSm = NULL;
    
    if (!RegisterClassEx(&wcex))
        return FALSE;


	g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindow(L"TestWindowClass", L"Hello world", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);

	if(!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
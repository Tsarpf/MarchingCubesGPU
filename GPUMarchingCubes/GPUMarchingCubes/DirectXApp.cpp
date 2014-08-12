#include "DirectXApp.h"

//HINSTANCE g_hInst = NULL;
//HWND g_hWnd = NULL;
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;
IDXGISwapChain* g_SwapChain = NULL;
ID3D11Device* g_d3dDevice = NULL;
ID3D11DeviceContext* g_ImmediateContext = NULL;
ID3D11RenderTargetView* g_RenderTargetView = NULL;

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

    if (FAILED(initDX()))
        return false;

	if (FAILED(compileAndEnableShaders()))
		return false;

    return true;
}

bool DirectXApp::compileAndEnableShaders()
{

	return true;
}
bool DirectXApp::initDX()
{

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };

    UINT width = 640;
    UINT height = 480;

    UINT featureLevelCount = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;


    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, featureLevelCount,
        D3D11_SDK_VERSION, &sd, &g_SwapChain, &g_d3dDevice, NULL, &g_ImmediateContext);
    if (FAILED(hr))
        return hr;

    ID3D11Texture2D* backBuffer = NULL;

    //this saves the position of swap chain back buffer to backBuffer
    hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr))
        return hr;


    
    hr = g_d3dDevice->CreateRenderTargetView(backBuffer, NULL, &g_RenderTargetView);
    //release memory and close threads used by COM object. Only closes texture object, doesn't destroy the actual back buffer
    backBuffer->Release();
    if (FAILED(hr))
        return hr;

    g_ImmediateContext->OMSetRenderTargets(1, &g_RenderTargetView, NULL);

    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height= (float)height;

    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_ImmediateContext->RSSetViewports(1, &vp);


    return S_OK;
}

bool DirectXApp::run()
{
    MSG msg = { 0 };
    //while (GetMessage(&msg, NULL, 0, 0
    while(msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            render();
        }
    }

    return true;
}

void DirectXApp::render()
{
    float clearColor[4] = { 0.0f, 0.125f, 0.2f, 1.0f };
    g_ImmediateContext->ClearRenderTargetView(g_RenderTargetView, clearColor);
    g_SwapChain->Present(0, 0);
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
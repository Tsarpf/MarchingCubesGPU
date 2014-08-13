#include "DirectXApp.h"

//HINSTANCE g_hInst = NULL;
//HWND g_hWnd = NULL;
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;
IDXGISwapChain* g_SwapChain = NULL;
ID3D11Device* g_d3dDevice = NULL;
ID3D11DeviceContext* g_ImmediateContext = NULL;
ID3D11RenderTargetView* g_RenderTargetView = NULL;

ID3D11VertexShader* g_VertexShader = NULL;
ID3D11InputLayout* g_VertexLayout = NULL;

ID3D11PixelShader* g_PixelShader = NULL;

ID3D11Buffer* g_VertexBuffer = NULL;

DirectXApp::DirectXApp()
{
}


DirectXApp::~DirectXApp()
{
}

bool DirectXApp::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  	if(FAILED(createWindow(hInstance, nCmdShow)))
		return false;

    if (FAILED(initDX()))
        return false;

	if (FAILED(compileAndEnableShaders()))
		return false;

    return true;
}

HRESULT DirectXApp::compileAndEnableShaders()
{
	HRESULT hr = S_OK;

	//Vertex shader
	ID3DBlob* vsBlob = NULL;
	hr = compileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &vsBlob);

	if (FAILED(hr))
	{
		vsBlob->Release();
		return hr;
	}

	hr = g_d3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), NULL, &g_VertexShader);

	if (FAILED(hr))
	{
		vsBlob->Release();
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//unsigned int numElements 
	unsigned int numElements = ARRAYSIZE(layout);

	hr = g_d3dDevice->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(),
										vsBlob->GetBufferSize(), &g_VertexLayout);

	vsBlob->Release();

	if (FAILED(hr))
		return hr;

	//set input layout
	g_ImmediateContext->IASetInputLayout(g_VertexLayout);


	ID3DBlob* psBlob = NULL;

	hr = compileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &psBlob);

	if (FAILED(hr))
		return hr;

	hr = g_d3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &g_PixelShader);
	psBlob->Release();


	if (FAILED(hr))
		return hr;


	struct SimpleVertex
	{
		XMFLOAT3 Pos;
	};

	SimpleVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	hr = g_d3dDevice->CreateBuffer(&bd, &initData, &g_VertexBuffer);

	if (FAILED(hr))
		return hr;


	unsigned int stride = sizeof(SimpleVertex);
	unsigned int offset = 0;
	g_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

HRESULT DirectXApp::compileShaderFromFile(WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** OutBlob)
{
	HRESULT hr = S_OK;

	/*
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	*/

	ID3DBlob* errorBlob;
	//DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;


	hr = D3DX11CompileFromFile(FileName, NULL, NULL, EntryPoint, ShaderModel,
		NULL, NULL, NULL, OutBlob, &errorBlob, NULL);

	if (FAILED(hr))
	{
		if (errorBlob != NULL)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		if (errorBlob)
		{
			errorBlob->Release();
		}
		return hr;
	}

	if (errorBlob) errorBlob->Release();

	return S_OK;

}

bool DirectXApp::initDX()
{

	//Only allow directx 11
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };

	//Window and viewport size
    UINT width = 640;
    UINT height = 480;

    UINT featureLevelCount = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Refresh rate
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
	//Multisampling
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

bool DirectXApp::Run()
{
    MSG msg = { 0 };
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

/* Renders a frame */
void DirectXApp::render()
{

	//Background color
    float clearColor[4] = { 0.0f, 0.125f, 0.2f, 1.0f };

	//Draw background color
    g_ImmediateContext->ClearRenderTargetView(g_RenderTargetView, clearColor);

	//Switch back buffer and front buffer to show what we've drawn. 
	//ie. present it.
    g_SwapChain->Present(0, 0);
}


/* Create a basic window using windows api*/
HRESULT DirectXApp::createWindow(HINSTANCE hInstance, int nCmdShow)
{

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;

	//lpfn == long pointer to a function, WndProc == Windows Procedure 
	//Long Pointer to the Windows Procedure function
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

/* Windows Procedure function used in showing and drawing the window*/
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
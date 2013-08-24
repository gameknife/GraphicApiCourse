#include "stableheader.h"
#include "GtPresenter.h"

// d3d引入
#include "d3d9.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxsdk/lib/x86/d3dx9.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

GtPresenter::GtPresenter(void)
{
}


GtPresenter::~GtPresenter(void)
{
}

bool GtPresenter::Init(const GtPresenterParam& initParam )
{
	// 注册窗口类
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= initParam.hInst;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= "GtFramework Window Class";

	wcex.hIcon			= NULL;
	wcex.hIconSm		= NULL;

	RegisterClassEx(&wcex);

	// 创建窗口
	m_hWnd = CreateWindow("GtFramework Window Class", initParam.windowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, initParam.width, initParam.height, NULL, NULL, initParam.hInst, NULL);

	// 调整尺寸
	RECT realRect;
	GetClientRect(m_hWnd, &realRect);

	int width = realRect.right - realRect.left;
	int height = realRect.bottom - realRect.top;
	width = initParam.width * 2 - width;
	height = initParam.height * 2 - height;

	MoveWindow(m_hWnd, GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2, width, height, FALSE);

	if (!m_hWnd)
	{
		return false;
	}

	// 显示窗口
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);


	//////////////////////////////////////////////////////////////////////////
	// create d3d device for Show Soft Buffer
	if( NULL == ( m_d3d9 = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return false;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// bpp 32 XRGB
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.BackBufferWidth = initParam.width;
	d3dpp.BackBufferHeight = initParam.height;

	// Create the D3DDevice
	if( FAILED( m_d3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_hwDevice ) ) )
	{
		MessageBox(m_hWnd,"无法创建DX设备","",MB_OK);
		return false;
	}

}

bool GtPresenter::Swap()
{
	m_hwDevice->Present(NULL, NULL, NULL, NULL);
	return true;
}

bool GtPresenter::Shutdown()
{
	if (m_hwDevice)
	{
		m_hwDevice->Release();
	}
	
	if (m_d3d9)
	{
		m_d3d9->Release();
	}

	return true;
}

bool GtPresenter::ProcessMsg()
{
	MSG msg;
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message != WM_QUIT)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
//
// yikaiming (C) 2013
// 
//
// Name:   	main.cpp
// Desc:		hello world 3d 事例程序
// 	
// 
// Author:	YiKaiming
// Date:		2013/8/11
// 
//////////////////////////////////////////////////////////////////////////

// 预编译头文件
#include "stableheader.h"
// presenter
#include "GtPresenter.h"

#define WIN_WIDTH 512
#define WIN_HEIGHT 512

const char* helloWorldShader = "\
struct app2vert\
{\
	float3 pos : POSITION;\
};\
struct vert2frag\
{\
	float4 pos : POSITION;\
};\
struct fragout\
{\
	float4 color : COLOR0;\
};\
vert2frag vs_main( app2vert IN )\
{\
	vert2frag OUT = (vert2frag)0;\
	OUT.pos = float4(IN.pos.xyz, 1.0);\
	return OUT;\
}\
fragout ps_main( vert2frag IN )\
{\
	fragout OUT;\
	OUT.color = 1;\
	return OUT;\
}";

class CVertexBuffer
{
public:
	CVertexBuffer( IDirect3DDevice9* device, int bufferSize ):m_device(device)
	{
		m_device->CreateVertexBuffer( bufferSize, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &m_vertexBuffer, NULL );
	}

	~CVertexBuffer()
	{
		m_vertexBuffer->Release();
	}

	void Update(void* data, int length)
	{
		void* gpuData = NULL;
		if( D3D_OK == m_vertexBuffer->Lock( 0, sizeof(float) * 9, &gpuData, NULL ) )
		{
			memcpy( gpuData, data, length );
			m_vertexBuffer->Unlock();
		}
		else
		{
			MessageBox(NULL, "vb create failed", "shader compile failed", MB_OK);
		}
	}

	void Use()
	{
		m_device->SetStreamSource( 0, m_vertexBuffer, 0, sizeof(float) * 3);
	}

	IDirect3DDevice9* m_device;
	IDirect3DVertexBuffer9* m_vertexBuffer;
};

class CShader
{
public:
	CShader( IDirect3DDevice9* device, const char* shader, const char* vs_main, const char* ps_main ):m_device(device), m_vertexShader(NULL), m_pixelShader(NULL)
	{
		// 编译shader
		LPD3DXBUFFER vShader = NULL;
		LPD3DXBUFFER pShader = NULL;
		LPD3DXBUFFER vShaderError = NULL;
		if( D3D_OK != D3DXCompileShader( shader, strlen(shader), NULL, NULL, vs_main, "vs_3_0", NULL, &vShader, &vShaderError, NULL ) )
		{
			MessageBox(NULL, (char*)vShaderError->GetBufferPointer(), "shader compile failed", MB_OK);
		}
		if( D3D_OK != D3DXCompileShader( shader, strlen(shader), NULL, NULL, ps_main, "ps_3_0", NULL, &pShader, &vShaderError, NULL ) )
		{
			MessageBox(NULL, (char*)vShaderError->GetBufferPointer(), "shader compile failed", MB_OK);
		}

		// 编译成功了，创建shader
		if (vShader)
		{
			m_device->CreateVertexShader( (DWORD*)vShader->GetBufferPointer(), &m_vertexShader );
		}
		if (pShader)
		{
			m_device->CreatePixelShader( (DWORD*)pShader->GetBufferPointer(), &m_pixelShader );
		}
	}

	~CShader()
	{
		m_vertexShader->Release();
		m_pixelShader->Release();
	}

	void Use()
	{
		m_device->SetVertexShader( m_vertexShader );
		m_device->SetPixelShader( m_pixelShader );
	}

	IDirect3DDevice9* m_device;
	IDirect3DVertexShader9* m_vertexShader;
	IDirect3DPixelShader9* m_pixelShader;
};

IDirect3DDevice9* g_device = NULL;
CShader* g_shader = NULL;
CVertexBuffer* g_vb = NULL;

const float g_PI = 3.14159f;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	char*    lpCmdLine,
	int       nCmdShow)
{
#if (defined(DEBUG) || defined(_DEBUG))
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	GtPresenterParam param;
	param.hInst = hInstance;
	param.windowName = "Hello World 3D";
	param.width = WIN_WIDTH;
	param.height = WIN_HEIGHT;
	param.bpp = 32;

	GtPresenter presenter;
	presenter.Init( param );

	g_device = presenter.m_hwDevice;

	//////////////////////////////////////////////////////////////////////////
	// 1. 创建三角形的三个顶点
	float triData[] = {
		-0.5, 0.5, 1.0,
		0.5, 0.5, 1.0,
		0.0, -0.5, 1.0, 
	};

	// 创建一个显卡中的BUFFER	
	g_vb = new CVertexBuffer( g_device, sizeof(triData) );
	// 把数据更新进去
	g_vb->Update( triData, sizeof(triData) );

	//////////////////////////////////////////////////////////////////////////
	// 3. 创建显卡变换顶点以及着色的方法，这里叫做shader
	g_shader = new CShader( g_device, helloWorldShader, "vs_main", "ps_main" );

	float time = 0;

	//////////////////////////////////////////////////////////////////////////
	// 4. 渲染循环
	while(1)
	{
		time += 0.033;

		if (presenter.ProcessMsg())
		{
			break;
		}

		// 所有的调用必须嵌套在BEGIN END之间
		if( D3D_OK == g_device->BeginScene() )
		{
			// 调用D3D DEVICE清屏
			g_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 33, 33, 33), 1.0f, 0 );

			// 设置着色器
			g_shader->Use();

			// 设置三角形的顶点格式
			g_device->SetFVF(D3DFVF_XYZ);

			// 修改一下顶点？
// 			triData[0] = sinf( time + 0 * g_PI*2.f/3.f ) * 0.5;
// 			triData[1] = cosf( time + 0 * g_PI*2.f/3.f ) * 0.5;
// 
// 			triData[3] = sinf( time + 1 * g_PI*2.f/3.f ) * 0.5;
// 			triData[4] = cosf( time + 1 * g_PI*2.f/3.f ) * 0.5;
// 
// 			triData[6] = sinf( time + 2 * g_PI*2.f/3.f ) * 0.5;
// 			triData[7] = cosf( time + 2 * g_PI*2.f/3.f ) * 0.5;
// 
// 			g_vb->Update(triData, sizeof(triData));

			// 应用顶点BUFFER
			g_vb->Use();

			// 利用以上的设置，绘制 三角形列表 - 1个
			g_device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

			g_device->EndScene();
		}

		presenter.Swap();
	}

	//////////////////////////////////////////////////////////////////////////
	// 5.销毁工作
	delete g_shader;
	delete g_vb;

	//////////////////////////////////////////////////////////////////////////
	// 6.presenter销毁
	presenter.Shutdown();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// yikaiming (C) 2013
// 
//
// Name:   	main.cpp
// Desc:		hello world 3d ��������
// 	
// 
// Author:	YiKaiming
// Date:		2013/8/11
// 
//////////////////////////////////////////////////////////////////////////

// Ԥ����ͷ�ļ�
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
		// ����shader
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

		// ����ɹ��ˣ�����shader
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
	// 1. ���������ε���������
	float triData[] = {
		-0.5, 0.5, 1.0,
		0.5, 0.5, 1.0,
		0.0, -0.5, 1.0, 
	};

	// ����һ���Կ��е�BUFFER	
	g_vb = new CVertexBuffer( g_device, sizeof(triData) );
	// �����ݸ��½�ȥ
	g_vb->Update( triData, sizeof(triData) );

	//////////////////////////////////////////////////////////////////////////
	// 3. �����Կ��任�����Լ���ɫ�ķ������������shader
	g_shader = new CShader( g_device, helloWorldShader, "vs_main", "ps_main" );

	float time = 0;

	//////////////////////////////////////////////////////////////////////////
	// 4. ��Ⱦѭ��
	while(1)
	{
		time += 0.033;

		if (presenter.ProcessMsg())
		{
			break;
		}

		// ���еĵ��ñ���Ƕ����BEGIN END֮��
		if( D3D_OK == g_device->BeginScene() )
		{
			// ����D3D DEVICE����
			g_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 33, 33, 33), 1.0f, 0 );

			// ������ɫ��
			g_shader->Use();

			// ���������εĶ����ʽ
			g_device->SetFVF(D3DFVF_XYZ);

			// �޸�һ�¶��㣿
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

			// Ӧ�ö���BUFFER
			g_vb->Use();

			// �������ϵ����ã����� �������б� - 1��
			g_device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

			g_device->EndScene();
		}

		presenter.Swap();
	}

	//////////////////////////////////////////////////////////////////////////
	// 5.���ٹ���
	delete g_shader;
	delete g_vb;

	//////////////////////////////////////////////////////////////////////////
	// 6.presenter����
	presenter.Shutdown();

	return 0;
}

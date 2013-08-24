/**
  @file GtPresenter.h
  
  @brief ���ڴ��е���ɫ�������Դ��еĿ�����

  @author Kaiming

  ������־ history
  ver:1.0
   
 */

#ifndef GtPresenter_h__
#define GtPresenter_h__

#include "prerequisite.h" ///< ����ȫ��ǰ��

struct GtPresenterParam
{
	HINSTANCE hInst;
	const char* windowName;

	int width;
	int height;
	int bpp;
};

class GtPresenter
{
public:
	GtPresenter(void);
	~GtPresenter(void);

	bool Init(const GtPresenterParam& initParam);
	bool Swap();
	bool Shutdown();
	bool ProcessMsg();

public:
	// DXӲ������
	struct IDirect3D9* m_d3d9;
	struct IDirect3DDevice9* m_hwDevice;

	HWND m_hWnd;
};

#endif



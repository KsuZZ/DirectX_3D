//////////////////////////////
//	/*F- Fon -F*/			//
//	/*S- Svet -S*/			//
//	/*T - Tuman -T*/		//
//
//
////////////////////////////
int kub(35),ver(36*kub),tri(12*kub);
float Range(-25),Speed(0),Spd(1),Angle(1),Density(0.07f);
bool	LEn(false), //����
		FEn(true),	//�����
		CEn(false);	//������
short ROTATE_NUM(0);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//���������� ����������
#pragma comment(lib, "d3d9.lib"		)	// ������ �� d3d9.lib
#pragma comment(lib, "d3dx9.lib"	)	// ������ �� d3dx9.lib
#pragma comment(lib, "winmm.lib"	)	// ������ �� winmm.lib

//���������� ������������ �����
#include <windows.h>					// ������������ ���� ��� Windows
#include <d3d9.h>						// ������������ ���� ��� DirectX3D
#include <d3dx9.h>						// ������������ ���� ��� ��������� ������� DirectX3D
#include "Mi.h"

//������ ��� ������������ �����������
#define SAFE_RELEASE(x) if(x != NULL){ x->Release(); x = NULL; }


#define APPNAME "\"5LabIVSem: �����\" - Andrey Demakov aka Demon"		// �������� ����
#define CLASSNAME "DIRECT3D"									// �������� ������ ����
bool	isProcess=true;											// ���� �������� ����������

RECT	WindowRect;

int		Width=800, Height=600;	// ������ � ������ ����
bool	fullscreen = false;		// �������(true) / �������������(false) �����
bool	keys[256];				// ������ ��� ������ � �����������

HWND hWnd;						// ����� �������� ��������� ������ ����

//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL;	// Direct3D. ������������ ��� �������� ���������� ����������
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;	// ���������� ����������
D3DPRESENT_PARAMETERS   d3dpp;					// ��������� ��� �������� Direct3D
//---------------------------------------------------------------------------


LPDIRECT3DVERTEXBUFFER9 MyVertexBuffer;			// ������� ����� ��� ��������� ����
LPDIRECT3DTEXTURE9		MyTexture_1;				// 1 �������� ��� ����� ����
LPDIRECT3DTEXTURE9		MyTexture_2;				// 2 �������� ��� ����� ����
LPDIRECT3DTEXTURE9		MyTexture_3;				// 2 �������� ��� ����� ����

struct MyVertex
{
    float	x,y,z;
    DWORD	color;
	float	tx,ty;	// ���������� ��������
//	float	a,b,c;
};

DWORD MyVertex_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 /*| D3DFVF_NORMAL*/;

DWORD FogMode	= D3DFOG_LINEAR;					// ��� ������ (�� ��������� LINEAR)
DWORD FogColor	= D3DCOLOR_ARGB(0, 0, 0, 250);	// ��������� ���� ������
DWORD BColor	= D3DCOLOR_ARGB(0, 100, 100, 120);	// ��������� ���� ������
//-----------------------------------------------------------------------------
// Name: SetupPixelFog(DWORD Color, DWORD Mode)
// Desc: ������������� ���������� �����
//-----------------------------------------------------------------------------
void SetupPixelFog(DWORD Color, DWORD Mode)
{
							// ��� ��������� ������
	float Start   = 5.0f;	// ���������� �� ������ �� ������� 
							// ������ ��������� �����
	float End     = 35.0f;	// ���������� �� ������ �� ������� 
							// ����� ����� ��������� �������� �������

	//float Density = 0.16f;	// ��������� (������ ��� exp'�������) ������

	// �������� ������������� ������
	g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FEn);

	// ������������� ���� ������
	g_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	// Set fog parameters.
	if( Mode == D3DFOG_LINEAR )
	{
		// ������������� �������� ��� ������ (D3DFOG_LINEAR)
		g_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		// ������ �������� � �������� ���������� ��� ������
		g_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		g_pd3dDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
	}
	else
	{
		// ������������� �������� ��� ������ (D3DFOG_EXP ��� D3DFOG_EXP2)
		g_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		// ������������� ��������� ������
		g_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
	}
}

//-----------------------------------------------------------------------------
// Name: ReSizeD3DScene(int width, int height)
// Desc: ��� ��������� ������� ���� ������ ������� �����
//-----------------------------------------------------------------------------
void ReSizeD3DScene(int width, int height)
{
	if (height==0)										// ������������� ������� �� ����
		height=1;


	D3DXMATRIXA16 matProjection;						// ������� ������������ �������
	D3DXMatrixPerspectiveFovLH(&matProjection, 45.0f, (float)width/(float)height, 0.1f, 100.0f );

	// ������������� ������������ �������
	if(g_pd3dDevice)
		g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProjection );
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: ����� ����������� ��� Direct3d �������, 
//       ������� ��������� � ������������ ��� ������ ����������
//-----------------------------------------------------------------------------
bool Cleanup()
{
	// ���� �����
	return true;
}

//-----------------------------------------------------------------------------
// Name: ReInit()
// Desc: ����� ��������� ��� Direct3d ������� � ������, 
//       ������� ��������� � ������������ ��� ������ ����������
//-----------------------------------------------------------------------------
bool ReInit()
{
	ReSizeD3DScene(Width, Height);

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// ��������� ���� ������
																// (������������ ��� ������� ���������)
/*L */	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ���� �� ������������
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// �������� ������������� Z-������

	return true;
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: ������������� ���������� ���������� Direct3D
//-----------------------------------------------------------------------------
bool InitD3D(int width, int height, bool fscreen)
{
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		// ���� �� ������� ������� Direct3D ������� ��������� � ������
		MessageBox(NULL,"�� ���� ������� Direct3D9!","������",MB_OK|MB_ICONEXCLAMATION);

    ZeroMemory( &d3dpp, sizeof(d3dpp) ); // ������� ������ ��-��� d3dpp


	// ���� ������������ ����������
	if(fscreen)
	{
		d3dpp.BackBufferWidth = width;								// ������������� ������ ������� ������
		d3dpp.BackBufferHeight = height;							// ������������� ������ ������� ������
		d3dpp.BackBufferCount = 1;									// ������������� ���-�� ������� �������
																	// ������� �����������
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ������������� ������ ������
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// ������������ �������������
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz=0;
		d3dpp.PresentationInterval=0;
	};

	d3dpp.hDeviceWindow		   = hWnd;

	d3dpp.BackBufferFormat = D3DFMT_R5G6B5;		// ������ ������� ������

	d3dpp.EnableAutoDepthStencil = true;		// ������������ ����� ������� � ������� �����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// ������ ������ ������� � ������� ������

	d3dpp.Windowed = !fscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;


	// ������� ���������� ����������

	// �������� ������� ���������� ���������� � �������������� ������������ ����������
	if(FAILED( g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		// ���� �� ����������
		// �������� ������� ���������� ���������� ��� ������������� ������������ ����������

		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
		{

			// ����� ������� ��������� �� ������
			MessageBox(NULL,"�� ���� ������� ���������� ����������!","������",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}

    return true;
}


bool ResetWindow(int width, int height, bool fscreen)
{
	DWORD WindowStyle, WindowExStyle;

	if(fscreen)
	{
		WindowExStyle=WS_EX_APPWINDOW;								// ����������� ����� ����
		WindowStyle=WS_POPUP;										// ����� ����
	}
	else
	{
		WindowExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ����������� ����� ����
		WindowStyle=WS_OVERLAPPEDWINDOW;							// ����� ����
	};

	WindowRect.left=(long)0;			// ������� ���� �� ��� X
	WindowRect.right=(long)Width;		// ������ ����
	WindowRect.top=(long)0;				// ������� ���� �� ��� X
	WindowRect.bottom=(long)Height;		// ������ ����

	AdjustWindowRectEx(&WindowRect, WindowStyle, FALSE, WindowExStyle);	// Adjust Window To True Requested Size


	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	if(fscreen)
	{
		d3dpp.BackBufferWidth	= Width;	// ������ ������� ������
		d3dpp.BackBufferHeight	= Height;	// ������ ������� ������
		d3dpp.BackBufferCount	= 1;		// ���-�� ������ �������
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ������� ������
	}
	else
		d3dpp.FullScreen_RefreshRateInHz=0;


	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// ������������ �������������
	d3dpp.hDeviceWindow				= hWnd;
	d3dpp.BackBufferFormat			= D3DFMT_R5G6B5;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.Windowed					= !fscreen;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;

	SetWindowLong( hWnd, GWL_STYLE, WindowStyle );


	Cleanup();
	g_pd3dDevice->Reset(&d3dpp);
	ReInit();

    if( !fscreen )
    {
		SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, WindowRect.right- WindowRect.left , WindowRect.bottom - WindowRect.top , SWP_SHOWWINDOW );
    }

    return true;
}

bool AllInit()
{
	if(!InitD3D(Width, Height, fullscreen))
		return false;

	if( FAILED( g_pd3dDevice->CreateVertexBuffer(
		ver*sizeof(MyVertex),	// ������ ������ (� ����� ������ ver ������
								// ������ �������� � ��������� MyVertex)
		D3DUSAGE_WRITEONLY,		// ���������� ����� ������ ��� ������ (�������)
		MyVertex_FVF,			// ������ ����� ������
		D3DPOOL_MANAGED,		// DirectX ��� ��������� ������������� ������
		&MyVertexBuffer,		// ��� ��������� �� �����
		NULL					// ������� NULL
		) ) )
		return false;			// ���� ��������� ������ �������� �� ����

	// ������� ��� ����
	MyVertex v[] =
	{
//////////////////////////////////////////////////////////////////////////////////
/////////////�////////////////////////////////////////////////////////////////////
//1///////////////////////////////////////////////////////////////////////////////
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //1
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //2
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //3
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //4
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //5
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //6
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
			
//2/////////////////////////////////////////////////////////////////////////////
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //7
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //8
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //9
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //10
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //11
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //12
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//3///////////////////////////////////////////////////////////////////////////
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //13
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //14
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //15
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //16
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //17
		{ -3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //18
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//4///////////////////////////////////////////////////////////////////////////
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //19
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //20
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //21
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //22
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //23
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //24
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//5/////////////////////////////////////////////////////////////////////////////
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //25
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //26
		{-13.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-13.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //27
		{-13.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //28
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //29
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //30
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-13.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },	

//6///////////////////////////////////////////////////////////////////////////

		{-13.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //31
		{-11.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-13.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //32
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-13.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //33
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-13.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //34
		{-11.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //35
		{-11.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-13.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //36
		{-13.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-13.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },	

//7///////////////////////////////////////////////////////////////////////////

		{ -5.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //37
		{ -3.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -5.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //38
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -3.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //39
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //40
		{ -3.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -3.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //41
		{ -3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -3.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },	

//8///////////////////////////////////////////////////////////////////////////
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//9///////////////////////////////////////////////////////////////////////////
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//10////////////////////////////////////////////////////////////////////////////
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//11////////////////////////////////////////////////////////////////////////////
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//12////////////////////////////////////////////////////////////////////////////
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//13////////////////////////////////////////////////////////////////////////////
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//14////////////////////////////////////////////////////////////////////////////
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{-11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{-11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{-11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//15////////////////////////////////////////////////////////////////////////////
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//16///////////////////////////////////////////////////////////////////////////////

		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB( 255,   0,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB( 255,   0,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////�////////////////////////////////////////////////////////////////////////
//17////////////////////////////////////////////////////////////////////////////////////////

		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
			
//18////////////////////////////////////////////////////////////////////////////
		{  3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //7
		{  5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //8
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //9
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //10
		{  5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //11
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //12
		{  3.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },

//19////////////////////////////////////////////////////////////////////////////
		{ -1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },

//20////////////////////////////////////////////////////////////////////////////
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//21//////////////////////////////////////////////////////////////////////////
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//22////////////////////////////////////////////////////////////////////////////
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//23////////////////////////////////////////////////////////////////////////////
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//24////////////////////////////////////////////////////////////////////////////
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//25////////////////////////////////////////////////////////////////////////////
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//26////////////////////////////////////////////////////////////////////////////
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ -1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ -1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ -1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//27////////////////////////////////////////////////////////////////////////////
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  5.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//28///////////////////////////////////////////////////////////////////////////////

		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  3.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0, 255,   0), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  1.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0, 255,   0), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////�/////////////////////////////////////////////////////////////////////////////////////////////////////////
//29/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{  7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //19
		{  9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, //20
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //21
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //22
		{  9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
	

		{  9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //23
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, //24
		{  7.0f, -4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//30////////////////////////////////////////////////////////////////////////////////
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f, -2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//31////////////////////////////////////////////////////////////////////////////
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  0.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//32////////////////////////////////////////////////////////////////////////////
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  2.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//33////////////////////////////////////////////////////////////////////////////
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  7.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
//34////////////////////////////////////////////////////////////////////////////
		{ 13.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ 13.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 13.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 13.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
//35///////////////////////////////////////////////////////////////////////////////

		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 

		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f,-1.0f, */ }, 
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f,-1.0f, */ },

		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{ 11.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ },
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  4.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 0.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f,  1.0f, D3DCOLOR_XRGB(   0,   0, 255), 1.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ }, 
		{  9.0f,  6.0f, -1.0f, D3DCOLOR_XRGB(   0,   0, 255), 0.0f, 1.0f,  /* 0.0f, 0.0f, 0.0f, */ },

	};

	void * pBuf;
	// ����������� ��������� �����
	MyVertexBuffer->Lock( 0, ver * sizeof(MyVertex), &pBuf, 0 );
	memcpy( pBuf, v, ver * sizeof(MyVertex));
	MyVertexBuffer->Unlock();

	// ��������� �������� �� �����
	D3DXCreateTextureFromFile(g_pd3dDevice, "Texture1.bmp", &MyTexture_1);
	D3DXCreateTextureFromFile(g_pd3dDevice, "Texture2.bmp", &MyTexture_2);
	D3DXCreateTextureFromFile(g_pd3dDevice, "Texture3.bmp", &MyTexture_3);

	ReInit();
	return true;
}


void AllShutdown()
{
	Cleanup();

	SAFE_RELEASE(MyTexture_1);		// ����������� ������ ��-��� ��������
	SAFE_RELEASE(MyTexture_2);		// ����������� ������ ��-��� ��������
	SAFE_RELEASE(MyTexture_3);		// ����������� ������ ��-��� ��������
	SAFE_RELEASE(MyVertexBuffer);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pD3D);
}
/*L 
//----------------------------------------------------------------------------------
//  �������
//  LightMaterial()
//  �������������� ����
//-----------------------------------------------------------------------------------
VOID Light()
{
    D3DLIGHT9     Light;    // ����
    D3DXVECTOR3 VectorDir;
    //  ��������� ����
    ZeroMemory( &Light, sizeof(D3DLIGHT9) );
	Light.Type       = D3DLIGHT_SPOT;

    Light.Diffuse.r  = 1.3f;
    Light.Diffuse.g  = 1.0f;
    Light.Diffuse.b  = 1.0f;
	Light.Diffuse.a  = 1.0f;

	Light.Specular.r = 1.0f;
	Light.Specular.g = 1.0f;
	Light.Specular.b = 1.0f;
	Light.Specular.a = 1.0f;

    Light.Range      = 200.0f;
	Light.Position.x = 0.0f;
	Light.Position.y = 0.0f;
	Light.Position.z = -35.0f;
	Light.Direction.x = 0.0f;
	Light.Direction.y = 0.0f;
	Light.Direction.z = 0.0f;
	Light.Attenuation0 = 0.07f;
	Light.Attenuation1 = 0.07f;
	Light.Attenuation2 = 0.07f;
	Light.Phi          = 3.0f;
	Light.Theta        = 3.0f;	
  //  ��������� �������

    VectorDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
    D3DXVec3Normalize( (D3DXVECTOR3*)&Light.Direction, &VectorDir );

    g_pd3dDevice->SetLight( 0, &Light );
    g_pd3dDevice->LightEnable( 0, LEn );

    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0 );
	//////////////////////////////////////////////////////////////////////
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
		g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );

		g_pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
		g_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		g_pd3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DBLEND_ONE);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}
VOID Material1() {
    D3DMATERIAL9  Material; // ��������
    //  ��������� ��������
	ZeroMemory( &Material, sizeof(D3DMATERIAL9) );
    Material.Diffuse.r  = 1.0f;
    Material.Diffuse.g  = 0.0f;
    Material.Diffuse.b  = 0.0f;
	Material.Diffuse.a  = 1.0f;

	Material.Specular.r = 0.0f;
	Material.Specular.g = 0.0f;
	Material.Specular.b = 0.6f;
	Material.Specular.a = 1.0f;


	Material.Ambient.r = 0.0f;
	Material.Ambient.g = 0.0f;
	Material.Ambient.b = 0.0f;
	Material.Ambient.a = 1.0f;

	Material.Power = 0.1f;

    g_pd3dDevice->SetMaterial( &Material );
}
VOID Material2() {
    D3DMATERIAL9  Material; // ��������
    //  ��������� ��������
	ZeroMemory( &Material, sizeof(D3DMATERIAL9) );
    Material.Diffuse.r  = 0.0f;
    Material.Diffuse.g  = 1.0f;
    Material.Diffuse.b  = 0.0f;
	Material.Diffuse.a  = 1.0f;


	Material.Specular.r = 0.0f;
	Material.Specular.g = 0.0f;
	Material.Specular.b = 0.6f;
	Material.Specular.a = 1.0f;

	Material.Ambient.r = 0.0f;
	Material.Ambient.g = 0.0f;
	Material.Ambient.b = 1.0f;
	Material.Ambient.a = 1.0f;

	Material.Power = 0.1f;

    g_pd3dDevice->SetMaterial( &Material );
}
VOID Material3() {
    D3DMATERIAL9  Material; // ��������
    //  ��������� ��������
	ZeroMemory( &Material, sizeof(D3DMATERIAL9) );
    Material.Diffuse.r  = 0.0f;
    Material.Diffuse.g  = 0.0f;
    Material.Diffuse.b  = 1.0f;
	Material.Diffuse.a  = 1.0f;

	Material.Specular.r = 1.0f;
	Material.Specular.g = 0.0f;
	Material.Specular.b = 1.0f;
	Material.Specular.a = 1.0f;
	
	Material.Ambient.r = 0.0f;
	Material.Ambient.g = 0.0f;
	Material.Ambient.b = 0.0f;
	Material.Ambient.a = 1.0f;

	Material.Power = 0.1f;
    g_pd3dDevice->SetMaterial( &Material );
}
/*L*/
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
void Render()
{

	HRESULT hr;

	// ��������� Direct3dDevice �� ������������
	hr = g_pd3dDevice->TestCooperativeLevel();

	// ���� ���� �� � ������ ������� �� ����� ����������
	if(hr==D3DERR_DEVICELOST)
		return; 

	// ���� ���� ����� � ������ �������� ������������ ���������� ����������
	if(hr==D3DERR_DEVICENOTRESET)
	{
		Cleanup();
		
		g_pd3dDevice->Reset(&d3dpp); // ���� ���� ����� � ������ �������� ������������ ��� g_pd3dDevice

		if( g_pd3dDevice )
			ReInit();
	}

    if( NULL == g_pd3dDevice )
		return;


    // �������� ��������� ����� �����
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
/*L	*///	Light();
		// ���������� ��� �������� ����������� ���� (��� �������)...
		float time=GetTickCount()/2048.0f;

		// ������� ������ ����� � ���� ������ � ����� �������
		g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BColor, 1.0f, 0L);
/*F
		// ������� ������ ����� � ����� �������
		g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(
			int((cos(time*2.0f)+1.0f)/2.0f*255),
			int((cos(time     )+1.0f)/2.0f*255),
			int((sin(time*2.0f)+1.0f)/2.0f*255)), 1.0f, 0L);
F*/
		//--------------------------//
		// ����� �� ����� ��������! //
		//--------------------------//
		D3DXMATRIX matWorld;

		UINT  Time  = /*timeGetTime()*/GetTickCount() % 5000;
		Speed = Angle*Spd*(Time * (2.0f * D3DX_PI) / 5000.0f);
		switch (ROTATE_NUM)
	{
//��� ��������
		case 0:	D3DXMatrixRotationY( &matWorld, Speed );						break;
		case 1:	D3DXMatrixRotationX( &matWorld, Speed);							break;
		case 2:	D3DXMatrixRotationY( &matWorld, Speed );						break;
		case 3: D3DXMatrixRotationZ( &matWorld, Speed );						break;
		case 4: D3DXMatrixRotationYawPitchRoll( &matWorld, Speed,Speed,0 );		break;
		case 5: D3DXMatrixRotationYawPitchRoll( &matWorld, Speed,0,Speed );		break;
		case 6: D3DXMatrixRotationYawPitchRoll( &matWorld, 0,Speed,Speed );		break;
		case 7: D3DXMatrixRotationYawPitchRoll( &matWorld, Speed,Speed,Speed );	break;
		default: D3DXMatrixRotationY( &matWorld, Speed );
	}

		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);


		D3DXMATRIX matView;
		D3DXMatrixLookAtLH(&matView,
			&D3DXVECTOR3(0.0f, 0.0f, Range),	// ����� �� ������� �� �������
												// ����� ��������� ������
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ����� � ������� �������
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// ��� ����?
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// ������������� ��������� �����
		g_pd3dDevice->SetStreamSource( 0, MyVertexBuffer, 0, sizeof(MyVertex) );
		// ������������� ������ ������
		g_pd3dDevice->SetFVF( MyVertex_FVF );

		// ���� �������� ��������� �����������
/*L	*///	Material1();
		if( MyTexture_1 != NULL )
			g_pd3dDevice->SetTexture(0, MyTexture_1); // ������ ��������
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 192/*tri*/);
		if( MyTexture_2 != NULL )
			g_pd3dDevice->SetTexture(0, MyTexture_2); // ������ ��������
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 576, 144);
		if( MyTexture_3 != NULL )
			g_pd3dDevice->SetTexture(0, MyTexture_3); // ������ ��������
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 1008, 84);


		// ������������� ���������� �������
		// ������������������� - ������� �������:
		//
		// D3DTEXF_NONE			- ���������� �� �����������
		// D3DTEXF_POINT		- �������� ����������
		// D3DTEXF_LINEAR		- �������� ����������
		// D3DTEXF_ANISOTROPIC	- ������������ ����������	
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		SetupPixelFog(FogColor, FogMode);

/*
		// ���������� ���������
		if( MyTexture_1 != NULL )
			g_pd3dDevice->SetTexture(0, MyTexture_1); // ������ �������� 1
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 192tri);
		if( MyTexture_2 != NULL )
			g_pd3dDevice->SetTexture(1, MyTexture_2); // ������ �������� 2
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 576, 144);
		if( MyTexture_3 != NULL )
			g_pd3dDevice->SetTexture(2, MyTexture_3); // ������ �������� 3
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 1008, 84);
*/

		g_pd3dDevice->EndScene();							// ��������� ��������
		g_pd3dDevice->Present( NULL, NULL, NULL, NULL );	// ���������� ������ ����� �� �����
    }

}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------

LRESULT  WINAPI  MsgProc(HWND hWnd,			// ��������� �� ����
						 UINT msg,			// ��������� ��� ����� ����
						 WPARAM wParam,		// ��������� ���������
						 LPARAM lParam)		// ��������� ���������
{
	switch (wParam)
	{
//��������
		case VK_UP:		Range-=0.5f; break;	//�����������
		case VK_DOWN:	Range+=0.5f; break;	//��������
		case VK_ADD:		Density+=0.001f; break;	//���������� ��������� ������
		case VK_SUBTRACT:	Density-=0.001f; break;	//���������� ��������� ������
		case VK_F2:	if(msg==WM_KEYUP)	
						FEn=!FEn;	 break;	//������� ������
	/*	case VK_F3:	if(CEn)				//������� ������
							CEn=false;
						else
							CEn=true;
						break;
		case VK_F4:	if(LEn)				//������� ������
							LEn=false;
						else
							LEn=true;
						break;*/
//��� ��������
		case '0': ROTATE_NUM=0;		break;//Default Y
		case '1': ROTATE_NUM=1;		break;// X
		case '2': ROTATE_NUM=2;		break;// Y
		case '3': ROTATE_NUM=3;		break;// Z
		case '4': ROTATE_NUM=4;		break;// XY
		case '5': ROTATE_NUM=5;		break;// XZ
		case '6': ROTATE_NUM=6;		break;// YZ
		case '7': ROTATE_NUM=7;		break;// XYZ
//����� ��������
		case VK_LEFT:	Spd+=0.5f;	break;// - 
		case VK_RIGHT:	Spd-=0.5f;	break;// +
		case 'I':		MiWnd();	break;//Info
	}
	switch (msg)// �������� ��������� ����
	{
	case WM_SYSCOMMAND:						// ������������ ��������� �������
		{
			switch (wParam)					// �������� ������ �������
			{
			case SC_SCREENSAVE:				// �������� ���������� �����������?
			case SC_MONITORPOWER:			// ������� �������� �������������� � ����� ���������� �������?
				return 0;					// �� ������ ����� ���������
			}
			break;							// �����
		}

	case WM_KEYDOWN:						// ���� ������ �������
	{
		keys[wParam] = TRUE;
		return 0;
	}

	case WM_KEYUP:							// ���� ������� ��������
	{
		keys[wParam] = FALSE;
		return 0;
	}

	case WM_SIZE:							// ������ ������ ����
	{
		if(!fullscreen)
			ReSizeD3DScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	case WM_DESTROY:						// �� �������� ��������� � �������� ���������?
	case WM_CLOSE:
		{
			isProcess=false;
			return 0;						// ������������
		}

	}
	// ���������� ��� ������ ��������� � DefWindowProc
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: ��������� ����� ����������
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ��������������� ������ ����
	WNDCLASSEX wc;

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_CLASSDC;
	wc.lpfnWndProc		= MsgProc;
	wc.cbClsExtra		= 0L;
	wc.cbWndExtra		= 0L;
	wc.hInstance		= GetModuleHandle(NULL);
	wc.hIcon			= LoadIcon(NULL, IDI_HAND);					// ��������� ����������� ������ ����
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);				// ��������� ����������� ������
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= CLASSNAME;
	wc.hIconSm	        = NULL;

    RegisterClassEx( &wc );	// ������������ ����� ����

	DWORD WindowStyle, WindowExStyle;

	if(fullscreen)
	{
		WindowExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		WindowStyle=WS_POPUP;										// Windows Style
	}
	else
	{
		
		WindowExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		WindowStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	};

	WindowRect.left=(long)0;			// ������������� ������� ���� �� ��� X
	WindowRect.right=(long)Width;		// ������������� ������ ����
	WindowRect.top=(long)0;				// ������������� ������� ���� �� ��� X
	WindowRect.bottom=(long)Height;		// ������������� ������ ����
	
	AdjustWindowRectEx(&WindowRect, WindowStyle, FALSE, WindowExStyle);	// ������������� ������� ���� �� ������

	// ������� ����
	if (!(hWnd=CreateWindowEx(	WindowExStyle,						// ����������� ����� ������ ����
								CLASSNAME,							// ��� ������
								APPNAME,							// ��������� ����
								WindowStyle |						// ��� ����� ����
								WS_CLIPSIBLINGS |					// ������� �����
								WS_CLIPCHILDREN,					// ������� �����
								0, 0,								// ������� ����
								WindowRect.right-WindowRect.left,	// ������� ������ ����
								WindowRect.bottom-WindowRect.top,	// ������� ������ ����
								NULL,								// ��� ������������� ����
								NULL,								// �� ���� ��� ����
								wc.hInstance,						// Instance
								NULL)))								// ������ �� ���������
	{
		MessageBox(NULL,"�� ���� ������� ����!","������",MB_OK|MB_ICONEXCLAMATION);
	}

	
	ShowCursor(false);					// �������� ������

	ShowWindow( hWnd, SW_SHOWNORMAL );	// ���������� ����
	SetFocus(hWnd);						// ������������� ����� ����
	UpdateWindow( hWnd );				// ��������� ����
	SetForegroundWindow(hWnd);			// ������� ������� ���������

    // �������������
	if( AllInit() )
    {
        MSG msg;
		//�������� ������� ������� ����������
		while (isProcess)
		{
			// ��������� ���� ���������
			if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (GetMessage(&msg, NULL, 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				// ���� ����� ESCAPE �� ������� �� �����
				if(keys[VK_ESCAPE])
					isProcess=false;

				if(keys[VK_F1])				// ���� ����� F1
				{
					fullscreen=!fullscreen;	//������������� � �������/������������� �����
					ResetWindow(Width, Height, fullscreen);
					CEn=!CEn;
				}

		//		if (keys[VK_F2])	
		//		{
		//			FEn=!FEn;
		//		}

				if(keys[VK_F5])
					FogMode=D3DFOG_EXP;
				if(keys[VK_F6])
					FogMode=D3DFOG_EXP2;
				if(keys[VK_F7])
					FogMode=D3DFOG_LINEAR;

				if(IsIconic(hWnd))
					WaitMessage();
				else
					Sleep(1);
					Render();	// ������������� �����
			}



        }
	}

	AllShutdown();			// �������� ������� �������� ����� ��� �������
	ShowCursor(CEn);		// ���������� ������

    UnregisterClass( CLASSNAME, wc.hInstance );
    return 1;
}
#include "stdafx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager()
	: m_pD3D(NULL)
	, m_pD3DDevice(NULL)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	stCaps;
	int			nVertexProcessing;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);

	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24X8;

	HRESULT hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice);

	if (hr != D3D_OK)
	{
		// D3DERR_INVALIDCALL �޼����� ȣ���� ��ȿ�̴�. ���� ���, �޼����� �Ķ���Ϳ� ��ȿ�� ���� �����Ǿ� �ִ� ��� ���̴�.  
		// D3DERR_NOTAVAILABLE �� ��ġ��, ������ ��ũ���� ���� �ϰ� ���� �ʴ�.  
		// D3DERR_OUTOFVIDEOMEMORY Direct3D �� ó���� �ǽ��ϴµ� ����� ���÷��� 
		switch (hr)
		{
		case D3DERR_INVALIDCALL:
			//MessageBox(g_hWnd, "�޼����� ȣ���� ��ȿ�̴�. ���� ���, �޼����� �Ķ���Ϳ� ��ȿ�� ���� �����Ǿ� �ִ� ��� ���̴�.");
			break;
		case D3DERR_NOTAVAILABLE:
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			break;
		}
	}
	
	if ( FAILED( D3DXCreateSprite( m_pD3DDevice, &m_pD3DSprite ) ))
	{
		MessageBox( 
			GetFocus(),
			"Failed to invoke D3DXCreateSprite.",
			"WARNING!",
			MB_OK | MB_ICONEXCLAMATION
		);
	}

	this->SetupLight( );
}


cDeviceManager::~cDeviceManager()
{
	SAFE_RELEASE( m_pD3DSprite );
	SAFE_RELEASE( m_pD3D );

	if ( m_pD3DDevice )
	{
		ULONG ul = m_pD3DDevice->Release( );

		assert( ul == 0 && "����̽��� �̿��Ͽ� ������ ��ü �� �Ҹ���� ���� ��ü�� �ֽ��ϴ�." );
	}
}

void cDeviceManager::SetupLight( )
{
	D3DXVECTOR3 vDir( 1, -1, 1 );
	D3DXVec3Normalize( &vDir, &vDir );

	D3DLIGHT9 stLight;
	ZeroMemory( &stLight, sizeof( D3DLIGHT9 ) );
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(  0.8f, 0.8f, 0.8f, 1.0f );
	stLight.Diffuse = D3DXCOLOR(  0.8f, 0.8f, 0.8f, 1.0f );
	stLight.Specular = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	stLight.Direction = vDir;
	
	g_pD3DDevice->SetLight( 0, &stLight );
	g_pD3DDevice->LightEnable( 0, true );
}

LPDIRECT3DDEVICE9 cDeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

#include "stdafx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager()
	: m_pD3D(NULL)
	, m_pD3DDevice(NULL)
{
}


cDeviceManager::~cDeviceManager()
{
	//SAFE_RELEASE(m_pD3DDevice);


	if ( m_pD3DDevice )
	{
		ULONG ul = m_pD3DDevice->Release( );

		/*#ifdef _DEBUG*/
		assert( ul == 0 && "����̽��� �̿��Ͽ� ������ ��ü �� �Ҹ���� ���� ��ü�� �ֽ��ϴ�." );
		

		/*#endif*/

	}
}

void cDeviceManager::Setup( 
	HWND wndHandle )
{
	assert( !m_pD3D && "cDeviceManager::Setup already invoked" );

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DCAPS9	stCaps;
	int			nVertexProcessing;
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps );

	if ( stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory( &stD3DPP, sizeof( D3DPRESENT_PARAMETERS ) );
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.hDeviceWindow = wndHandle;
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24X8;

	HRESULT hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wndHandle,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice );

	if ( hr != D3D_OK )
	{
		// D3DERR_INVALIDCALL �޼����� ȣ���� ��ȿ�̴�. ���� ���, �޼����� �Ķ���Ϳ� ��ȿ�� ���� �����Ǿ� �ִ� ��� ���̴�.  
		// D3DERR_NOTAVAILABLE �� ��ġ��, ������ ��ũ���� ���� �ϰ� ���� �ʴ�.  
		// D3DERR_OUTOFVIDEOMEMORY Direct3D �� ó���� �ǽ��ϴµ� ����� ���÷��� 
		switch ( hr )
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
}

LPDIRECT3DDEVICE9 cDeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

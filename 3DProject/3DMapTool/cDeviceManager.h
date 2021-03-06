#pragma once
#include "Singleton.h"

#define g_pDeviceManager cDeviceManager::Get()
#define g_pD3DDevice cDeviceManager::Get()->GetDevice()

class cDeviceManager : 
	public TSingleton<cDeviceManager>
{
public:
	void Setup( HWND wndHandle );
	
public:
	LPDIRECT3DDEVICE9 GetDevice();

protected:
	cDeviceManager( );
	virtual ~cDeviceManager( );

private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

};


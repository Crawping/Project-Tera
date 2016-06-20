#include "stdafx.h"
#include "cMainGame.h"

#include "cCollisionManager.h"
#include "cSceneManager.h"
#include "cShaderManager.h"
#include "TestScene.h"
#include "DesertScene.h"

cPlayer* g_player = nullptr;
cLightObject* g_lightObject = nullptr;

cMainGame::cMainGame( )
{
	SetupManagers();

	g_player = new cPlayer;
	g_lightObject = new cLightObject;
	cCamera::Get( )->SetFollowingTarget( g_player );

	/*SOUNDMANAGER->addSound("배경음", "./BGM/War_Start_00.ogg");
	SOUNDMANAGER->play("배경음", 1.f);*/
	cSceneManager::Get( )->LoadScene<TestScene>( );
}

cMainGame::~cMainGame( )
{
	SAFE_DELETE( g_player );
	SAFE_DELETE( g_lightObject );
}

void cMainGame::Update( )
{
	cCamera::Get( )->Update( );
	
	cSceneManager::Get( )->Update( );
	cGameObjectManager::Get( )->Update( );
	cCollisionManager::Get( )->Update( );
	
	g_pTimeManager->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255),
		1.0f, 0
	);
	
	g_pD3DDevice->BeginScene();
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	cGameObjectManager::Get( )->Render( );
	cSceneManager::Get( )->Render( );
	
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cCamera::Get()->WndProc(hWnd, message, wParam, lParam);
}

void cMainGame::SetupManagers( )
{
	cDeviceManager::Get( );
	// Must be called after cDeviceManager initialized
	cTextureManager::Get( );
	cShaderManager::Get( );
	cCamera::Get( );
	// Etc
	cRandomUtil::Setup();
	cTimeManager::Get( );
	cKeyManager::Get( );
	cCollisionManager::Get( );
	cGameObjectManager::Get( );
	cSoundManager::Get();
	cSceneManager::Get();
}
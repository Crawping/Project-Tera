#include "stdafx.h"
#include "TestScene.h"
#include "cPlayer.h"
#include "cBoundingBox.h"
#include "cGameObjectManager.h"
#include "cGroup.h"
#include "cNpcManager.h"
#include "cSkyBox.h"
#include "cArgoniteKallashGuardLeader.h"
#include "cGrid.h"
#include "TestScenePlane.h"
#include "cLightObject.h"
#include "cKeyManager.h"
#include "DesertScene.h"
#include "cSceneManager.h"
#include "Console.h"
#include "cSprite.h"

namespace
{
	void AdditionalWork( TestScenePlane** param )
	{
		*param = new TestScenePlane( "Plane0" );
	}
}

TestScene::TestScene( ) :
	m_plane( nullptr ),
	m_loadThread( ReadXML, "Scene/SCENE.xml", &m_loadSuccess,
		std::function<void( )>( std::bind( AdditionalWork, &m_plane )) ),
	m_loadSuccess( 0 ),
	m_loadingSprite( new cSprite( "CH/LoadingImage/LoadingImage63_Tex.tga" ))
{
	SOUNDMANAGER->addSound("����", "./Music/BGM(����).ogg", true, true);
	SOUNDMANAGER->play("����", 1.f);

	cGameObjectManager::Get( )->AddObject( "SkyBox", new cSkyBox(0) );
	cGameObjectManager::Get( )->AddObject( "Grid", new cGrid );
	cGameObjectManager::Get()->AddObject("Npc", new cNpcManager);
}

TestScene::~TestScene( )
{
	m_loadThread.join( );

	SAFE_DELETE( m_plane );
	SAFE_DELETE( m_loadingSprite );
	SOUNDMANAGER->stop("����");
}

void TestScene::Render( )
{
	if ( !m_loadSuccess )
	{
		Log( "�ε���_�˸���_�޽���" );
		if ( m_loadingSprite )
		{
			m_loadingSprite->Render( );
		}
		return;
	}

	if ( m_plane )
	{
		m_plane->Render( );
	}

	//�÷��̾� ����
	if (g_player)
	{
		g_player->Render();
	}
}

void TestScene::Update( )
{
	if ( !m_loadSuccess )
	{
		return;
	}

	if ( KEYMANAGER->isOnceKeyDown( VK_SPACE ) )
	{
		cSceneManager::Get( )->LoadScene<DesertScene>( );
		return;
	}

	if ( m_plane )
	{
		m_plane->Update( );
	}

	if ( g_player )
	{
		g_player->Update( );
	}
}

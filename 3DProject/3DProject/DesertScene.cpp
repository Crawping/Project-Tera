#include "stdafx.h"
#include "DesertScene.h"
#include "cEnemy.h"
#include "cLightObject.h"
#include "DesertScenePlane.h"
#include "cSkyBox.h"
#include "cArgoniteKallashGuardLeader.h"
#include "cGiantStone.h"
#include "cMadmadDuo.h"
#include "cArgoniteFemaleMagician.h"
#include "cPixie.h"
#include "DesertSceneOasis.h"
#include "cSprite.h"
#include "Console.h"

namespace
{
	void AdditionalWork( 
		DesertScenePlane** param,
		DesertSceneOasis** param2 )
	{
		*param = new DesertScenePlane;
		*param2 = new DesertSceneOasis;
	}
}

DesertScene::DesertScene( ) :
	m_plane( nullptr ),
	m_oasis( nullptr ),
	m_loadThread( ReadXML, "Scene/Desert_scene.xml", &m_loadSuccess,
		std::function<void( )>( std::bind( AdditionalWork, &m_plane, &m_oasis ))),
	m_loadSuccess( 0 ),
	m_loadingSprite( new cSprite( "CH/LoadingImage/LoadingImage1.tga" ))
{
	SOUNDMANAGER->addSound("������", "./Music/BGM(�縷).ogg", true, true);
	
	SOUNDMANAGER->addSound("ArgoniteFemaleMagician����", "./Music/ArgoniteFemaleMagician(Attack).ogg", false, false);
	SOUNDMANAGER->addSound("ArgoniteFemaleMagician����", "./Music/ArgoniteFemaleMagician(Death).ogg", false, false);
	
	SOUNDMANAGER->addSound("ArgoniteKallashGuardLeader����", "./Music/ArgoniteKallashGuardLeader(Attack).ogg", false, false);
	SOUNDMANAGER->addSound("ArgoniteKallashGuardLeader����", "./Music/ArgoniteKallashGuardLeader(Death).ogg", false, false);

	SOUNDMANAGER->addSound("Pixie����", "./Music/Pixie(Attack).ogg", false, false);
	SOUNDMANAGER->addSound("Pixie����", "./Music/Pixie(Death).ogg", false, false);

	SOUNDMANAGER->addSound("Madmad����", "./Music/Madman(Attack).ogg", false, false);
	SOUNDMANAGER->addSound("Madmad����", "./Music/Madman(Death).ogg", false, false);

	SOUNDMANAGER->play("������", 1.f);


	// A ����
	m_monsterRepo.push_back( new cMadmadDuo );
	m_monsterRepo.back( )->SetPosition({ -700.8f, 400.f, -773.2f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	m_monsterRepo.push_back( new cArgoniteFemaleMagician );
	m_monsterRepo.back( )->SetPosition({ -609.16f, 400.f, -890.23f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	m_monsterRepo.push_back( new cArgoniteKallashGuardLeader );
	m_monsterRepo.back( )->SetPosition({ -571.92f, 400.f, -758.72f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	
	// B ����
	m_monsterRepo.push_back( new cArgoniteFemaleMagician );
	m_monsterRepo.back( )->SetPosition({ -106.92f, 400.f, 702.17f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	m_monsterRepo.push_back( new cArgoniteKallashGuardLeader );
	m_monsterRepo.back( )->SetPosition({ -260.9f, 400.f, 630.6f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	m_monsterRepo.push_back( new cPixie );
	m_monsterRepo.back( )->SetPosition({ -160.1f, 400.f, 523.64f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	
	// C ����
	m_monsterRepo.push_back( new cArgoniteKallashGuardLeader );
	m_monsterRepo.back( )->SetPosition({ 483.17f, 400.f, -676.85f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	m_monsterRepo.push_back( new cPixie );
	m_monsterRepo.back( )->SetPosition({ 550.137f, 400.f, -557.44f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	m_monsterRepo.push_back( new cMadmadDuo );
	m_monsterRepo.back( )->SetPosition({ 388.45f, 400.f, -548.3f });
	m_monsterRepo.back( )->SetEnemyOrigin( &m_monsterRepo.back( )->GetPosition() );
	
	cGameObjectManager::Get( )->AddObject( "SkyBox", new cSkyBox( 1 ));
}

DesertScene::~DesertScene( )
{
	m_loadThread.join( );
	SAFE_DELETE( m_loadingSprite );
	SAFE_DELETE( m_oasis );
	SAFE_DELETE( m_plane );

	for ( auto enemyElem : m_monsterRepo )
	{
		SAFE_DELETE( enemyElem );
	}

	SOUNDMANAGER->stop("������");
}

void DesertScene::Update( )
{
	if ( !m_loadSuccess )
	{
		if ( m_loadingSprite )
		{
			m_loadingSprite->Update( );
		}
		return;
	}

	if ( m_oasis )
	{
		m_oasis->Update( );
	}

	if ( m_plane )
	{
		m_plane->Update( );
	}

	if ( g_player )
	{
		g_player->Update( );

		if ( m_plane )
		{
			g_player->SetPosition({
				g_player->GetPosition( ).x,
				m_plane->GetHeight( g_player ),
				g_player->GetPosition( ).z
			});
		}
	}

	for ( auto enemyElem : m_monsterRepo )
	{
		enemyElem->Update( );

		if ( !enemyElem->IsCulled( ) )
		{

			//���Ͱ� �ױ� �������� Height�� ���� ���� ���� Y���� ����
			if (enemyElem->GetEnemyState() != ENEMY_DEATH)
			{
				enemyElem->SetPosition({ enemyElem->GetPosition().x, m_plane->GetHeight(enemyElem),
					enemyElem->GetPosition().z });
			}
		}
		else
		{
			/*static int a = 1;
			Log( a++, "\n" );

			if ( a > 6 )
			{
				a = 1;
			}*/
		}
	}
}

void DesertScene::Render( )
{
	if ( !m_loadSuccess )
	{
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

	if ( m_oasis )
	{
		m_oasis->Render( );
	}

	//�÷��̾� ����
	if ( g_player )
	{
		g_player->Render( );
	}

	for ( auto enemyElem : m_monsterRepo )
	{
		enemyElem->Render( );
	}
}

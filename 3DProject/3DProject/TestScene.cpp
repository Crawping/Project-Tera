#include "stdafx.h"
#include "TestScene.h"

#include "TXMLReader.h"
#include "cBuildingObject.h"
#include "cBoundingBox.h"	

#include "cGrid.h"
#include "cPlayer.h"
#include "cBoundingBox.h"
#include "cCollisionManager.h"
#include "cGameObjectManager.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "cNpcManager.h"
#include "cSceneManager.h"
#include "cArgoniteKallashGuardLeader.h"
#include "cSkyBox.h"

TestScene::TestScene( 
	const std::string& xmlPath )
	: m_pGrid( nullptr )
	, m_pLoader( nullptr )
	, m_pNpc( nullptr )
	, m_pMonster( nullptr )
	, m_pSkyBox( nullptr )
{
	m_pGrid = new cGrid;
	m_pMonster = new cArgoniteKallashGuardLeader;
	m_pMonster2 = new cArgoniteKallashGuardLeader;
	m_pMonster2->SetPosition( { 0.f, 0.f, 400.f } );

	m_pSkyBox = new cSkyBox;
	m_pSkyBox->Setup();

	/*D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	m_pLoader = new cObjLoader;
	m_pLoader->Load("./Map/Height.obj", m_vecGroup, &mat);*/

	this->ReadXML( xmlPath );
}

TestScene::~TestScene( )
{
	SAFE_DELETE( m_pGrid );
	SAFE_DELETE( g_player );
	SAFE_DELETE( m_pLoader );
	SAFE_DELETE( m_pNpc );
	SAFE_DELETE( m_pMonster );
	SAFE_DELETE( m_pSkyBox );

	for ( auto buildObj : m_buildingObjectRepo )
	{
		SAFE_DELETE( buildObj );
	}
}

void TestScene::Render( )
{
	//��ī�̹ڽ� ����
	if (m_pSkyBox)
	{
		m_pSkyBox->Render();
	}

	//�׸��� ����
	if (m_pGrid)
	{
		m_pGrid->Render();
	}

	
	//�÷��̾� ����
	if (g_player)
	{
		g_player->Render();
	}

	//Npc ����
	if (m_pNpc)
	{
		m_pNpc->Render();
	}

	//���� ����
	if (m_pMonster)
	{
		m_pMonster->Render();
	}
	if ( m_pMonster2 )
		m_pMonster2->Render( );

	// �ǹ� ����
	for ( auto elem : m_buildingObjectRepo )
	{
		elem->Render( );
	}
}

void TestScene::Update( )
{
	if ( g_player )
	{
		g_player->Update( );
	}

	if ( m_pNpc )
	{
		m_pNpc->Update( );
	}

	if ( m_pMonster )
	{
		m_pMonster->Update( );
	}

	if ( m_pMonster2 )
	{
		m_pMonster2->Update( );
	}
	
	if (m_pSkyBox)
	{
		m_pSkyBox->Update();
	}

}

void TestScene::ReadXML( const std::string& xmlPath )
{
	// Begin reading XML
	tgon::TXMLReader xmlReader( xmlPath.c_str( ));
	if ( xmlReader.fail( ))
	{
		MessageBox( GetFocus( ),
			"XML �ε��� �����߽��ϴ�.",
			"WARNING!",
			MB_OK | MB_ICONEXCLAMATION
		);
	}
		
	ICollider* collider = nullptr;
	std::string objName;
	std::string modelPath;
	D3DXVECTOR3 pos{ 0,0,0 }, rot{ 0,0,0 }, scale{ 0,0,0 };

	for ( const auto& xmlNodeElem : xmlReader )
	{
		if ( !strcmp( "End", xmlNodeElem->Value( )))
		{
			cBuildingObject* newObject = new cBuildingObject( objName, modelPath );
			m_buildingObjectRepo.push_back( newObject );

			if ( collider )
			{
				newObject->AddCollider( collider );
			}
			newObject->SetPosition( pos );
			newObject->SetAngle( rot );
			newObject->SetScale( scale );

			// Reset Items
			newObject = nullptr;
			collider = nullptr;
		}
		else if ( !strcmp( "ObjectName", xmlNodeElem->Value( )))
		{
			const char* objCharName = xmlNodeElem->FirstChild( )->Value( );
			objName = objCharName;
		}
		else if ( !strcmp( "ModelPath", xmlNodeElem->Value( )))
		{
			modelPath = xmlNodeElem->FirstChild( )
				->Value( );
		}
		else if ( !strcmp( "BoundingBox", xmlNodeElem->Value( )))
		{
			tinyxml2::XMLElement* elem = xmlNodeElem->ToElement( );
			if ( elem )
			{
				const tinyxml2::XMLAttribute* minXAttr =
					elem->FirstAttribute( );
				const tinyxml2::XMLAttribute* minYAttr =
					minXAttr->Next( );
				const tinyxml2::XMLAttribute* minZAttr =
					minYAttr->Next( );

				const tinyxml2::XMLAttribute* maxXAttr =
					minZAttr->Next( );
				const tinyxml2::XMLAttribute* maxYAttr =
					maxXAttr->Next( );
				const tinyxml2::XMLAttribute* maxZAttr =
					maxYAttr->Next( );

				collider = new cBoundingBox(
					{ minXAttr->FloatValue( ),
					  minYAttr->FloatValue( ),
					  minZAttr->FloatValue( )},
					{ maxXAttr->FloatValue( ),
					  maxYAttr->FloatValue( ),
					  maxZAttr->FloatValue( )});
			}
		}
		else if ( !strcmp( "Position", xmlNodeElem->Value( )))
		{
			tinyxml2::XMLElement* elem = xmlNodeElem->ToElement( );
			if ( elem )
			{
				const tinyxml2::XMLAttribute* xAttr = 
					elem->FirstAttribute( );
				const tinyxml2::XMLAttribute* yAttr = 
					xAttr->Next( );
				const tinyxml2::XMLAttribute* zAttr = 
					yAttr->Next( );

				pos = { xAttr->FloatValue( ),
					yAttr->FloatValue( ),
					zAttr->FloatValue( )
				};
			}
			else
			{
				MessageBox( GetFocus( ),
					"�� �� ���� Element �����Դϴ�.",
					"WARNING!",
					MB_OK | MB_ICONEXCLAMATION
				);
			}
		}
		else if ( !strcmp( "Rotation", xmlNodeElem->Value( )))
		{
			tinyxml2::XMLElement* elem = xmlNodeElem->ToElement( );
			if ( elem )
			{
				const tinyxml2::XMLAttribute* xAttr = 
					elem->FirstAttribute( );
				const tinyxml2::XMLAttribute* yAttr = 
					xAttr->Next( );
				const tinyxml2::XMLAttribute* zAttr = 
					yAttr->Next( );

				rot = { xAttr->FloatValue( ),
					yAttr->FloatValue( ),
					zAttr->FloatValue( )
				};
			}
			else
			{
				MessageBox( GetFocus( ),
					"�� �� ���� Element �����Դϴ�.",
					"WARNING!",
					MB_OK | MB_ICONEXCLAMATION
				);
			}
		}
		else if ( !strcmp( "Scale", xmlNodeElem->Value( )))
		{
			tinyxml2::XMLElement* elem = xmlNodeElem->ToElement( );
			if ( elem )
			{
				const tinyxml2::XMLAttribute* xAttr =
					elem->FirstAttribute( );
				const tinyxml2::XMLAttribute* yAttr =
					xAttr->Next( );
				const tinyxml2::XMLAttribute* zAttr =
					yAttr->Next( );

				scale = { xAttr->FloatValue( ),
					yAttr->FloatValue( ),
					zAttr->FloatValue( )
				};
			}
			else
			{
				MessageBox( GetFocus( ),
					"�� �� ���� Element �����Դϴ�.",
					"WARNING!",
					MB_OK | MB_ICONEXCLAMATION
				);
			}
		}
	}
}

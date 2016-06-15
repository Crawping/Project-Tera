#include "stdafx.h"
#include "cGameObject.h"

#include "cGameObjectManager.h"
#include "ICollider.h"


cGameObject::cGameObject( ) :
	m_isActive( true ),
	m_pos( 0.f, 0.f, 0.f ),
	m_angle( 0.f, 0.f, 0.f ),
	m_scale( 1.f, 1.f, 1.f )
{
	D3DXMatrixIdentity( &m_matWorld );
}

cGameObject::~cGameObject( )
{
}

void cGameObject::Render( )
{
}

void cGameObject::Update( )
{
}

void cGameObject::SetName(
	const std::string& newObjName )
{
	// ó������ ������ object�� �ƴ� ���
	if ( !m_objName.empty( ))
	{
		// ���� ������Ʈ ���� key�� ���ο� key�� �����ϴ� 
		// ó���� �߰��˴ϴ�.

		cGameObject* object = 
			cGameObjectManager::Get( )->FindObject( m_objName );
		if ( object )
		{
			// ������ key�� erase�� ��
			cGameObjectManager::Get( )->EraseObject( 
				m_objName );

			// ���ο� key�� object�� ����մϴ�.
			cGameObjectManager::Get( )->AddObject(
				newObjName, this
			);
		}
	}

	m_objName = newObjName;
}
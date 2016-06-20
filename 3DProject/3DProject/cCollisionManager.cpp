#include "stdafx.h"
#include "cCollisionManager.h"
#include "cCollision.h"
#include "cBoundingBox.h"
#include "Console.h"
#include "cBoundingSphere.h"
#include "ICollider.h"
#include "cCollisionObject.h"


cCollisionManager::cCollisionManager( ) :
	m_isAutoUpdatable( false )
{
}

cCollisionManager::~cCollisionManager( )
{
}

void cCollisionManager::Update( )
{
	if ( !m_isAutoUpdatable )
	{
		return;
	}

	for ( auto& elemOp1 : m_collisionMap )
	{
		if ( elemOp1.second->GetColliderRepo( ).size( ) == 0 )
		{
			continue;
		}
		
		bool isCollisedAtLeastOnce = false;
		cCollisionObject* op1sCollidedObject = nullptr;

		for ( auto& elemOp2 : m_collisionMap )
		{
			// Compare target is same as elemOp1?
			// Or elemOp2 has no collider? 
			if ( elemOp1 == elemOp2 ||
				 elemOp2.second->GetColliderRepo( ).size( ) == 0 )
			{
				continue;
			}

			// Then, elemOp1 and elemOp2 is different with,
			// And both have colliders.
			// op1Collider�� elemOp2�� ���� �ݶ��̴�( op2Collider )�� �浹�ߴ��� �˾Ƴ���.
			// op1 �ݶ��̴� ����
			auto& op1Collider = elemOp1.second->GetColliderRepo()
				[0];
			// op2 �ݶ��̴� ����
			auto& op2Collider = 
				elemOp2.second->GetColliderRepo( )[0];

			eColliderType op1CollType =
				op1Collider->GetColliderType( );
			eColliderType op2CollType =
				op2Collider->GetColliderType( );

			bool isCollided = false;

			// TODO : Optimize here
			// Ÿ�Կ� �°� �浹 üũ�� �����Ѵ�.
			if ( op1CollType == eColliderType::kBox &&
				op2CollType == eColliderType::kBox )
			{
				isCollided = cCollision::IsBoxToBox(
					static_cast<const cBoundingBox&>(
						*op1Collider ),
					static_cast<const cBoundingBox&>(
						*op2Collider )
				);
			}
			else if ( op1CollType == eColliderType::kSphere &&
				op2CollType == eColliderType::kSphere )
			{
				isCollided = cCollision::IsSphereToSphere(
					static_cast<const cBoundingSphere&>(
						*op1Collider ),
					static_cast<const cBoundingSphere&>(
						*op2Collider )
				);
			}
			else if ( op1CollType == eColliderType::kBox &&
				op2CollType == eColliderType::kSphere )
			{
				isCollided = cCollision::IsBoxToSphere(
					static_cast<const cBoundingBox&>(
						*op1Collider ),
					static_cast<const cBoundingSphere&>(
						*op2Collider )
				);
			}
			else if ( op1CollType == eColliderType::kSphere &&
				op2CollType == eColliderType::kBox )
			{
				isCollided = cCollision::IsSphereToBox(
					static_cast<const cBoundingSphere&>(
						*op1Collider ),
					static_cast<const cBoundingBox&>(
						*op2Collider )
				);
			}

			if ( isCollided )
			{
				isCollisedAtLeastOnce = true;
				op1sCollidedObject = elemOp2.second;
				break;
			}
		}

		// elemOp1�� elemOp2�� �浹���ΰ�?
		if ( isCollisedAtLeastOnce )
		{
			// A �浹ü�� ���� collised ���°� �ƴϾ��°�?
			if ( !elemOp1.second->GetCollision( 0 ) )
			{
				// A�� �浹���·� ��ȯ����.
				elemOp1.second->SetCollision( 0, true );
				// �׷� A�� �浹 ���� �Լ��� ȣ���Ѵ�.
				elemOp1.second->OnCollisionEnter( 0, 
					op1sCollidedObject );
			}
			// A���� ���� �浹 ��ʰ� �־��µ� ���ݵ� �浹���̴�
			else
			{
				elemOp1.second->OnCollisionStay( 0,
					op1sCollidedObject );
			}

		}
		// A,B �� ��ü�� �浹���� �ʾҴ°�?
		else
		{
			// A �浹ü�� ���� collised ���¿��°�?
			if ( elemOp1.second->GetCollision( 0 ) )
			{
				// A�� �浹�������·� ��ȯ����.
				elemOp1.second->SetCollision( 0,
					false );
				// �׷� A�� �浹 ���� �Լ��� ȣ���Ѵ�.
				elemOp1.second->OnCollisionEnd( 0,
					op1sCollidedObject );
			}
		}
	}
}

void cCollisionManager::AddObject(
	cCollisionObject* target )
{
	std::unique_lock<std::mutex> m_lock( m_mutex );

	const uintptr_t key =
		reinterpret_cast<uintptr_t>( target );

	auto iter = m_collisionMap.find( key );
	if ( iter == m_collisionMap.end( ) )
	{
		m_collisionMap[key] = target;
	}
}

void cCollisionManager::EraseObject( 
	cCollisionObject* target )
{
	if ( target )
	{
		const uintptr_t key =
			reinterpret_cast<uintptr_t>( target );
		
		m_collisionMap.erase( key );
	}
}

void cCollisionManager::ClearAllObject( )
{
	m_collisionMap.clear( );
}

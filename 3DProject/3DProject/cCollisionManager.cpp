#include "stdafx.h"
#include "cCollisionManager.h"
#include "cCollision.h"
#include "cBoundingBox.h"
#include "Console.h"
#include "cBoundingSphere.h"
#include "ICollider.h"
#include "cCollisionObject.h"


cCollisionManager::cCollisionManager( )
{
}

cCollisionManager::~cCollisionManager( )
{
}

void cCollisionManager::Update( )
{
	for ( auto& elemOp1 : m_collisionMap )
	{
		if ( elemOp1.second->GetColliderRepo( ).size( ) == 0 )
		{
			continue;
		}
		
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
			// op1Collider�� elemOp2�� ���� �ݶ��̴���( op2Collider )�� �浹�ߴ��� �˾Ƴ���.
			bool isCollisedAtLeastOnce = false;
			for ( int op1ColliderIndex = 0; 
				op1ColliderIndex < elemOp1.second->GetColliderRepo( ).size( );
				  ++op1ColliderIndex )
			{
				// op1 �ݶ��̴� ����
				auto& op1Collider = elemOp1.second->GetColliderRepo()[op1ColliderIndex];
				
				for ( int op2ColliderIndex = 0;
					op2ColliderIndex < elemOp2.second->GetColliderRepo().size( );
					++op2ColliderIndex )
				{
					// op2 �ݶ��̴� ����
					auto& op2Collider = 
						elemOp2.second->GetColliderRepo( )[op2ColliderIndex];

					eColliderType op1CollType =
						op1Collider->GetColliderType( );
					eColliderType op2CollType =
						op2Collider->GetColliderType( );

					bool isCollised = false;

					// TODO : Optimize here
					// Ÿ�Կ� �°� �浹 üũ�� �����Ѵ�.
					if ( op1CollType == eColliderType::kBox &&
						op2CollType == eColliderType::kBox )
					{
						isCollised = cCollision::IsBoxToBox(
							static_cast<const cBoundingBox&>(
								*op1Collider ),
							static_cast<const cBoundingBox&>(
								*op2Collider )
						);
					}
					else if ( op1CollType == eColliderType::kSphere &&
						op2CollType == eColliderType::kSphere )
					{
						isCollised = cCollision::IsSphereToSphere(
							static_cast<const cBoundingSphere&>(
								*op1Collider ),
							static_cast<const cBoundingSphere&>(
								*op2Collider )
						);
					}
					else if ( op1CollType == eColliderType::kBox &&
						op2CollType == eColliderType::kSphere )
					{
						isCollised = cCollision::IsBoxToSphere(
							static_cast<const cBoundingBox&>(
								*op1Collider ),
							static_cast<const cBoundingSphere&>(
								*op2Collider )
						);
					}
					else if ( op1CollType == eColliderType::kSphere &&
						op2CollType == eColliderType::kBox )
					{
						isCollised = cCollision::IsBoxToSphere(
							static_cast<const cBoundingBox&>(
								*op1Collider ),
							static_cast<const cBoundingSphere&>(
								*op2Collider )
						);

						if ( isCollised )
						{
							int n = 3;
						}
					}

					// A,B �� ��ü�� �浹�ߴ°�?
					if ( isCollised )
					{
						isCollisedAtLeastOnce = true;
						break;
					}
				}


				// �׷� ���� op1�� ��� �浹ü�� ���ؼ� �ϳ��� �浹������
				// �ƴ����� ���� �÷��װ� ���´�. ( isCollisedAtLeastOnce )
				// op1Collider�� elemOp2�� �ݶ��̴��� �� �ϳ��� �浹���ΰ�?
				if ( isCollisedAtLeastOnce )
				{
					// A �浹ü�� ���� collised ���°� �ƴϾ��°�?
					if ( !elemOp1.second->GetCollision( op1ColliderIndex ))
					{
						if ( elemOp1.second->GetName( ) == "Player" )
						{
							int n = 3;
						}

						// A�� �浹���·� ��ȯ����.
						elemOp1.second->SetCollision( op1ColliderIndex, true );
						// �׷� A�� �浹 ���� �Լ��� ȣ���Ѵ�.
						elemOp1.second->OnCollisionEnter( op1ColliderIndex, elemOp2.second );
					}
					// A���� ���� �浹 ��ʰ� �־��µ� ���ݵ� �浹���̴�
					else
					{
						if ( elemOp1.second->GetName( ) == "Player" )
						{
							int n = 3;
						}

						elemOp1.second->OnCollisionStay( op1ColliderIndex,
							elemOp2.second );
					}

				}
				// A,B �� ��ü�� �浹���� �ʾҴ°�?
				else
				{
					// A �浹ü�� ���� collised ���¿��°�?
					if ( elemOp1.second->GetCollision( op1ColliderIndex ) )
					{
						if ( elemOp1.second->GetName( ) == "Player" )
						{
							int n = 3;
						}

						// A�� �浹�������·� ��ȯ����.
						elemOp1.second->SetCollision( op1ColliderIndex,
							false );
						// �׷� A�� �浹 ���� �Լ��� ȣ���Ѵ�.
						elemOp1.second->OnCollisionEnd( op1ColliderIndex,
							elemOp2.second );
					}
				}

			}
		}
	
	}
}

void cCollisionManager::AddObject(
	cCollisionObject* target )
{
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
	const uintptr_t key =
		reinterpret_cast<uintptr_t>( target );
	m_collisionMap.erase( key );
}

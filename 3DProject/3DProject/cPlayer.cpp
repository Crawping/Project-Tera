#include "stdafx.h"
#include "cPlayer.h"
#include "cBody.h"
#include "cHair.h"
#include "cFace.h"
#include "cTail.h"
#include "cWeaponMesh.h"
#include "cBoundingSphere.h"

cPlayer::cPlayer( ) :
	cCollisionObject( "Player" )
	, m_vDirection(D3DXVECTOR3(0, 0, 1))
	, m_fSpeed(1.f)
	, m_fAngle(0.f)
	, m_fPassTime(0.f)
	, m_fPeriod(0.f)
	, m_bAlive(true)
	, m_bIsAction(false)
	, m_bPushBehind(false)
	, n(0)
{
	SetPlayerState(PLAYER_BATTLEIDLE);

	//������
	m_pBody = new cBody;
	m_pBody->Setup("CH/Player", "Player_Body.X");

	m_pFace = new cFace;
	m_pFace->SetNeckTM(&m_pBody->GetNeckTM());
	m_pFace->Setup("CH/Player", "Player_Head.X");

	m_pHair = new cHair;
	m_pHair->SetHairTM(&m_pBody->GetHairTM());
	m_pHair->Setup("CH/Player", "Player_Hair.X");

	m_pTail = new cTail;
	m_pTail->SetTailTM(&m_pBody->GetTailTM());
	m_pTail->Setup("CH/Player", "Player_Tail.X");

	m_pHand = new cWeaponMesh;
	m_pHand->SetWeapon(&m_pBody->GetWeaponBack());
	m_pHand->Setup("CH/Player", "Lance.X");

	this->SetCollider(new cBoundingSphere(D3DXVECTOR3(0, 0, 0), 9.f));

	SetAniTrack(PLAYER_BATTLEIDLE);

	this->SetCollisionType(CollisionType::ePlayer);

	D3DXMATRIXA16 matLocal;
	D3DXMatrixTranslation(&matLocal, 0, 15, 0);
	this->GetColliderRepo()[0]->SetLocal(&matLocal);
}

cPlayer::~cPlayer( )
{
	SAFE_DELETE(m_pBody);
	SAFE_DELETE(m_pHair);
	SAFE_DELETE(m_pFace);
	SAFE_DELETE(m_pTail);
	SAFE_DELETE(m_pHand);
}

void cPlayer::Update( )
{
	__super::Update( );

	SetUpdateState();

	this->GetColliderRepo()[0]->SetWorld(&m_matWorld);
	
	/*if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		SetAniTrack(++n);
		int q = 0;
	}*/

	KeyControl();

	SetFSMState();
}

void cPlayer::Render( )
{
	//__super::Render( );

	SetRenderState();
}

void cPlayer::KeyControl()
{
	D3DXMATRIXA16 matR, matT;

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		m_fAngle -= 0.1f;
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		m_fAngle += 0.1f;
	}

	m_vDirection = D3DXVECTOR3(1, 0, 0);
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		//�޸��⺸�� �켱������ ���� �ൿ�� �ϰ� ���� ��� �� �ൿ�� �޽��� ���
		if (GetPlayerState() == PLAYER_BATTLEIDLE)
		{
			m_bIsAction = false;
		}

		//�켱������ ���� �ൿ�� �� �ൿ���� �Ұ�
		if (GetPlayerState() == PLAYER_TUMBLING || GetPlayerState() == PLAYER_SKILL1)
		{
			m_bIsAction = true;
		
		}
		else
		{
			SetPlayerState(PLAYER_RUN);
			SetPosition(GetPosition() + m_vDirection * m_fSpeed);
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		SetPlayerState(PLAYER_BATTLEIDLE);
		SetAniTrack(PLAYER_BATTLEIDLE);
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		//�޸��⺸�� �켱������ ���� �ൿ�� �ϰ� ���� ��� �� �ൿ�� �޽��� ���
		if (GetPlayerState() == PLAYER_BATTLEIDLE)
		{
			m_bIsAction = false;
		}

		//�켱������ ���� �ൿ�� �� �ൿ���� �Ұ�

		if (GetPlayerState() == PLAYER_TUMBLING || GetPlayerState() == PLAYER_SKILL1)
		{
			SetPlayerState(PLAYER_RUN);
			SetPosition(GetPosition() - m_vDirection * m_fSpeed);
		}
		//����Ű �ڸ� ������ �� �ѹ��� �ڸ� �� ��
		if (!m_bPushBehind)
		{
			m_fAngle = -m_fAngle;
			m_bPushBehind = true;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		SetPlayerState(PLAYER_BATTLEIDLE);

		if (m_bPushBehind)
		{
			m_bPushBehind = false;
		}
	}

	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		//�Һ��� ��� �ൿ�� ���ؼ� �켱������ ���� ����.
		if (m_bIsAction)
		{
			m_bIsAction = false;
		}
		SetPlayerState(PLAYER_TUMBLING);
	}

	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		if (GetPlayerState() != PLAYER_TUMBLING)
		{
			m_bIsAction = false;
		}
		SetPlayerState(PLAYER_SKILL1);
	}

	D3DXMatrixTranslation(&matT, GetPosition().x, GetPosition().y - 4.f, GetPosition().z);
	m_matWorld = matR * matT;
}

void cPlayer::OnCollisionStay(cCollisionObject* rhs)
{
	//�ѹ��� �浹üũ�ϰ�
	if (rhs->GetCollisionType() == CollisionType::eBuilding && !this->GetCollision())
	{
		this->SetCollision(true);
	}

	if (rhs->GetCollisionType() == CollisionType::eMonster && !this->GetCollision())
	{
		this->SetCollision(true);
		rhs->SetCurrHp(rhs->GetCurrHp() - 100);
		int a = 0;
	}
}

void cPlayer::SetFSMState()
{
	switch (GetPlayerState())
	{
	case PLAYER_BATTLEIDLE:
	{
		if (!m_bIsAction)
		{
			SetAniTrack(PLAYER_BATTLEIDLE);
			m_bIsAction = true;
			m_fPeriod = m_pBody->GetAniTrackPeriod(PLAYER_BATTLEIDLE) ;
		}

		else if (m_bIsAction)
		{
			m_fPassTime += g_pTimeManager->GetDeltaTime();

			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
			}
		}
	}
		break;

	case PLAYER_RUN:
	{
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_RUN);
			m_fPeriod = m_pBody->GetAniTrackPeriod(PLAYER_RUN);
		}

		else if (m_bIsAction)
		{
			m_fPassTime += g_pTimeManager->GetDeltaTime();

			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
			}
		}
	}
		break;
	
	case PLAYER_ATTACK:
		break;

	case PLAYER_TUMBLING:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_TUMBLING);
			m_fPeriod = m_pBody->GetAniTrackPeriod(PLAYER_TUMBLING);
		}

		else if (m_bIsAction)
		{
			m_fPassTime += g_pTimeManager->GetDeltaTime();

			if (m_fPassTime < m_fPeriod - 0.75f)
			{
				SetPosition(GetPosition() + m_vDirection * (m_fPeriod - 1));
			}

			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}
		}
		break;

		//���ϰ� ����ġ��
	case PLAYER_SKILL1:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_SKILL1);
			m_fPeriod = m_pBody->GetAniTrackPeriod(PLAYER_SKILL1);
		}

		else if (m_bIsAction)
		{
			m_fPassTime += g_pTimeManager->GetDeltaTime();

			if (m_fPassTime < m_fPeriod - 1.0f)
			{
				SetPosition(GetPosition() + m_vDirection * (m_fPeriod - 1.f));
			}

			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}
		}
		break;
	}
}

void cPlayer::SetUpdateState( )
{
	if (m_pBody)
	{
		m_pBody->Update();
		m_pBody->SetWorld(&m_matWorld);
	}
	if (m_pHair)
	{
		m_pHair->SetHairTM(&m_pBody->GetHairTM());
		m_pHair->Update();
	}
	if (m_pFace)
	{
		m_pFace->SetNeckTM(&m_pBody->GetNeckTM());
		m_pFace->Update();
	}

	if (m_pTail)
	{
		m_pTail->SetTailTM(&m_pBody->GetTailTM());
		m_pTail->Update();
	}

	if (m_pHand)
	{
		m_pHand->SetWeapon(&m_pBody->GetWeaponHand());
		m_pHand->Update();
	}
}

void cPlayer::SetRenderState()
{
	if (m_pBody)
	{
		m_pBody->Render();
	}
	if (m_pHair)
	{
		m_pHair->Render();
	}
	if (m_pFace)
	{
		m_pFace->Render();
	}

	if (m_pTail)
	{
		m_pTail->Render();
	}

	if (m_pHand)
	{
		m_pHand->Render(NULL);
	}
}

void cPlayer::SetAniTrack(int nIndex)
{
	m_pBody->SetAnimationIndex(nIndex);
	m_pHair->SetAnimationIndex(nIndex);
	m_pFace->SetAnimationIndex(nIndex);
	m_pTail->SetAnimationIndex(nIndex);
}